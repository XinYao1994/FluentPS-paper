#ifdef WITH_PYTHON_LAYER
#include <boost/python.hpp>
namespace bp = boost::python;
#endif

#include <gflags/gflags.h>
#include <glog/logging.h>
#include <map>
#include <boost/algorithm/string.hpp>

#include "caffe/caffe.hpp"
#include "caffe/util/signal_handler.h"

#include "ps/ps.h"

using caffe::TBlob;
using caffe::Blob;
using caffe::Caffe;
using caffe::Net;
using caffe::LayerBase;
using caffe::Solver;
using caffe::shared_ptr;
using caffe::string;
using caffe::Timer;
using caffe::vector;
using std::ostringstream;

DEFINE_string(gpu, "",
    "Optional; run in GPU mode on given device IDs separated by ', '."
    "Use '-gpu all' to run on all available GPUs. The effective training "
    "batch size is multiplied by the number of devices.");
DEFINE_string(solver, "",
    "The solver definition protocol buffer text file.");
DEFINE_string(model, "",
    "The model definition protocol buffer text file.");
DEFINE_string(snapshot, "",
    "Optional; the snapshot solver state to resume training.");
DEFINE_string(weights, "",
    "Optional; the pretrained weights to initialize finetuning, "
    "separated by ', '. Cannot be set simultaneously with snapshot.");
DEFINE_int32(iterations, 50,
    "The number of iterations to run.");
DEFINE_string(sigint_effect, "stop",
             "Optional; action to take when a SIGINT signal is received: "
              "snapshot, stop or none.");
DEFINE_string(sighup_effect, "snapshot",
             "Optional; action to take when a SIGHUP signal is received: "
             "snapshot, stop or none.");
DEFINE_double(delay_mean, 0,
    "The mean of delay to run.");
DEFINE_double(delay_sdev, 0,
    "The std dev of delay to run.");

// A simple registry for caffe commands.
typedef int (*BrewFunction)();
typedef std::map<caffe::string, BrewFunction> BrewMap;
BrewMap g_brew_map;

#define RegisterBrewFunction(func) \
namespace { \
class __Registerer_##func { \
 public: /* NOLINT */ \
  __Registerer_##func() { \
    g_brew_map[#func] = &func; \
  } \
}; \
__Registerer_##func g_registerer_##func; \
}

static BrewFunction GetBrewFunction(const caffe::string& name) {
  if (g_brew_map.count(name)) {
    return g_brew_map[name];
  } else {
    LOG(ERROR) << "Available caffe actions:";
    for (BrewMap::iterator it = g_brew_map.begin();
         it != g_brew_map.end(); ++it) {
      LOG(ERROR) << "\t" << it->first;
    }
    LOG(FATAL) << "Unknown action: " << name;
    return NULL;  // not reachable, just to suppress old compiler warnings.
  }
}

// Parse GPU ids or use all available devices
static void get_gpus(vector<int>* gpus) {
  if (FLAGS_gpu == "all") {
    int count = 0;
#ifndef CPU_ONLY
    CUDA_CHECK(cudaGetDeviceCount(&count));
#else
    NO_GPU;
#endif
    for (int i = 0; i < count; ++i) {
      gpus->push_back(i);
    }
  } else if (FLAGS_gpu.size()) {
    vector<string> strings;
    boost::split(strings, FLAGS_gpu, boost::is_any_of(", "));
    for (int i = 0; i < strings.size(); ++i) {
      gpus->push_back(boost::lexical_cast<int>(strings[i]));
    }
  } else {
    CHECK_EQ(gpus->size(), 0);
  }
}

// caffe commands to call by
//     caffe <command> <args>
//
// To add a command, define a function "int command()" and register it with
// RegisterBrewFunction(action);

// Device Query: show diagnostic information for a GPU device.
int device_query() {
  LOG(INFO) << "Querying GPUs " << FLAGS_gpu;
  vector<int> gpus;
  get_gpus(&gpus);
  for (int i = 0; i < gpus.size(); ++i) {
    caffe::Caffe::SetDevice(gpus[i]);
    std::cout << caffe::Caffe::DeviceQuery();
  }
  return 0;
}
RegisterBrewFunction(device_query);

// Load the weights from the specified caffemodel(s) into the train and
// test nets.
void CopyLayers(caffe::Solver* solver, const std::string& model_list) {
  std::vector<std::string> model_names;
  boost::split(model_names, model_list, boost::is_any_of(", ") );
  for (int i = 0; i < model_names.size(); ++i) {
    LOG(INFO) << "Finetuning from " << model_names[i];
    solver->net()->CopyTrainedLayersFrom(model_names[i]);
    for (int j = 0; j < solver->test_nets().size(); ++j) {
      solver->test_nets()[j]->CopyTrainedLayersFrom(model_names[i]);
    }
  }
}

// Translate the signal effect the user specified on the command-line to the
// corresponding enumeration.
caffe::SolverAction::Enum GetRequestedAction(
    const std::string& flag_value) {
  if (flag_value == "stop") {
    return caffe::SolverAction::STOP;
  }
  if (flag_value == "snapshot") {
    return caffe::SolverAction::SNAPSHOT;
  }
  if (flag_value == "none") {
    return caffe::SolverAction::NONE;
  }
  LOG(FATAL) << "Invalid signal effect \""<< flag_value << "\" was specified";
  return caffe::SolverAction::NONE;
}

// Train / Finetune a model.
int train() {
  CHECK_GT(FLAGS_solver.size(), 0) << "Need a solver definition to train.";
  CHECK(!FLAGS_snapshot.size() || !FLAGS_weights.size())
      << "Give a snapshot to resume training or weights to finetune "
      "but not both.";

  caffe::SolverParameter solver_param = caffe::ReadSolverParamsFromTextFileOrDie(FLAGS_solver);

  // If the gpus flag is not provided, allow the mode and device to be set
  // in the solver prototxt.
  if (FLAGS_gpu.size() == 0
      && solver_param.solver_mode() == caffe::SolverParameter_SolverMode_GPU) {
      if (solver_param.has_device_id()) {
          FLAGS_gpu = "" +
              boost::lexical_cast<string>(solver_param.device_id());
      } else {  // Set default GPU if unspecified
          FLAGS_gpu = "" + boost::lexical_cast<string>(0);
      }
  }

  // Read flags for list of GPUs
  vector<int> gpus;
  get_gpus(&gpus);
#ifndef CPU_ONLY
  caffe::GPUMemory::Scope gpu_memory_scope(gpus);
#endif
  // Set mode and device id[s]
  if (gpus.size() == 0) {
    LOG(INFO) << "Use CPU.";
    Caffe::set_mode(Caffe::CPU);
  } else {
    ostringstream s;
    for (int i = 0; i < gpus.size(); ++i) {
      s << (i ? ", " : "") << gpus[i];
    }

    LOG(INFO) << "Using GPUs " << s.str();
    int dev_id = 0;
#ifndef CPU_ONLY
    cudaDeviceProp device_prop;
    for (int i = 0; i < gpus.size(); ++i) {
      cudaGetDeviceProperties(&device_prop, gpus[i]);
      LOG(INFO) << "GPU " << gpus[i] << ": " << device_prop.name;
      if (solver_param.has_device_id() && gpus[i] == solver_param.device_id()) {
        dev_id = i;
      }
    }
    CUDA_CHECK(cudaSetDevice(gpus[dev_id]));
    Caffe::SetDevice(gpus[dev_id]);
#endif
    solver_param.set_device_id(gpus[dev_id]);
    Caffe::set_mode(Caffe::GPU);
    Caffe::set_gpus(gpus);
    Caffe::set_solver_count(gpus.size());
    CHECK_EQ(gpus.size(), Caffe::solver_count());
  }

  caffe::SignalHandler signal_handler(
        GetRequestedAction(FLAGS_sigint_effect),
        GetRequestedAction(FLAGS_sighup_effect));

  shared_ptr<caffe::Solver> solver(caffe::SolverRegistry::CreateSolver(solver_param, nullptr, 0));
  solver->SetActionFunction(signal_handler.GetActionFunction());

  if (FLAGS_snapshot.size()) {
    LOG(INFO) << "Resuming from " << FLAGS_snapshot;
    solver->Restore(FLAGS_snapshot.c_str());
  } else if (FLAGS_weights.size()) {
    CopyLayers(solver.get(), FLAGS_weights);
  }

  if (gpus.size() > 1) {
    caffe::P2PManager p2p_mgr(solver, gpus.size(), solver->param());
    p2p_mgr.Run(gpus);
  } else {
    LOG(INFO) << "Starting Optimization";

    solver->Solve();

    if (gpus.size() == 1) {
      std::ostringstream os;
      os.precision(4);
      solver->perf_report(os, gpus[0]);
      LOG(INFO) << os.str();
    }
  }
  LOG(INFO) << "Optimization Done in " << Caffe::time_from_init();
  return 0;
}
RegisterBrewFunction(train);


// Test: score a model.
int test() {
  CHECK_GT(FLAGS_model.size(), 0) << "Need a model definition to score.";
  CHECK_GT(FLAGS_weights.size(), 0) << "Need model weights to score.";

  // Read flags for list of GPUs
  vector<int> gpus;
  get_gpus(&gpus);
  while (gpus.size() > 1) {
    // Only use one GPU
    LOG(INFO) << "Not using GPU #" << gpus.back() << " for single-GPU function";
    gpus.pop_back();
  }
#ifndef CPU_ONLY
  if (gpus.size() > 0) {
    Caffe::SetDevice(gpus[0]);
  }
  caffe::GPUMemory::Scope gpu_memory_scope(gpus);
#endif

  // Set mode and device id
  if (gpus.size() != 0) {
    LOG(INFO) << "Use GPU with device ID " << gpus[0];
#ifndef CPU_ONLY
    cudaDeviceProp device_prop;
    cudaGetDeviceProperties(&device_prop, gpus[0]);
    LOG(INFO) << "GPU device name: " << device_prop.name;
#endif
    Caffe::set_mode(Caffe::GPU);
  } else {
    LOG(INFO) << "Use CPU.";
    Caffe::set_mode(Caffe::CPU);
  }

  // Instantiate the caffe net.
  Net caffe_net(FLAGS_model, caffe::TEST, 0U);
  caffe_net.CopyTrainedLayersFrom(FLAGS_weights);
  LOG(INFO) << "Running for " << FLAGS_iterations << " iterations.";

  vector<int> test_score_output_id;
  vector<float> test_score;
  float loss = 0;
  for (int i = 0; i < FLAGS_iterations; ++i) {
    float iter_loss;
    const vector<Blob*>& result =
        caffe_net.Forward(&iter_loss);
    loss += iter_loss;
    int idx = 0;
    for (int j = 0; j < result.size(); ++j) {
      const float* result_vec = result[j]->cpu_data<float>();
      for (int k = 0; k < result[j]->count(); ++k, ++idx) {
        const float score = result_vec[k];
        if (i == 0) {
          test_score.push_back(score);
          test_score_output_id.push_back(j);
        } else {
          test_score[idx] += score;
        }
        const std::string& output_name = caffe_net.blob_names()[
            caffe_net.output_blob_indices()[j]];
        LOG(INFO) << "Batch " << i << ", " << output_name << " = " << score;
      }
    }
  }
  loss /= FLAGS_iterations;
  LOG(INFO) << "Loss: " << loss;
  for (int i = 0; i < test_score.size(); ++i) {
    const std::string& output_name = caffe_net.blob_names()[
        caffe_net.output_blob_indices()[test_score_output_id[i]]];
    const float loss_weight = caffe_net.blob_loss_weights()[
        caffe_net.output_blob_indices()[test_score_output_id[i]]];
    std::ostringstream loss_msg_stream;
    const float mean_score = test_score[i] / FLAGS_iterations;
    if (loss_weight) {
      loss_msg_stream << " (* " << loss_weight
                      << " = " << (loss_weight * mean_score) << " loss)";
    }
    LOG(INFO) << output_name << " = " << mean_score << loss_msg_stream.str();
  }
  return 0;
}
RegisterBrewFunction(test);

// Time: benchmark the execution time of a model.
int time() {
  CHECK_GT(FLAGS_model.size(), 0) << "Need a model definition to time.";
  vector<int> gpus;
#ifndef CPU_ONLY
  // Read flags for list of GPUs
  get_gpus(&gpus);
  while (gpus.size() > 1) {
    // Only use one GPU
    LOG(INFO) << "Not using GPU #" << gpus.back() << " for single-GPU function";
    gpus.pop_back();
  }
  if (gpus.size() > 0) {
    Caffe::SetDevice(gpus[0]);
  }
  caffe::GPUMemory::Scope gpu_memory_scope(gpus);
#endif
  // Set mode and device_id
  if (gpus.size() != 0) {
    LOG(INFO) << "Use GPU with device ID " << gpus[0];
#ifndef CPU_ONLY
    cudaDeviceProp device_prop;
    cudaGetDeviceProperties(&device_prop, gpus[0]);
    LOG(INFO) << "GPU " << gpus[0] << ": " << device_prop.name;
#endif
    Caffe::set_mode(Caffe::GPU);
  } else {
    LOG(INFO) << "Use CPU.";
    Caffe::set_mode(Caffe::CPU);
  }
  const int kInitIterations = 5;

  caffe::SolverParameter solver_param;
  caffe::ReadNetParamsFromTextFileOrDie(FLAGS_model, solver_param.mutable_net_param());
  solver_param.set_max_iter(kInitIterations);
  solver_param.set_lr_policy("fixed");
  solver_param.set_snapshot_after_train(false);
  solver_param.set_base_lr(0.01F);
  solver_param.set_random_seed(1371LL);
  solver_param.set_test_interval(FLAGS_iterations + 1);
  solver_param.set_display(0);
  shared_ptr<Solver> solver(caffe::SolverRegistry::CreateSolver(solver_param));
  shared_ptr<Net> caffe_net = solver->net();

  // Do a number of clean forward and backward pass,
  // so that memory allocation are done,
  // and future iterations will be more stable.
  Timer init_timer;
  Timer forward_timer;
  Timer backward_timer;
  double forward_time = 0.0;
  double backward_time = 0.0;
  LOG(INFO) << "Initialization for " << kInitIterations << " iterations.";
  // Note that for the speed benchmark, we will assume that the network does
  // not take any input blobs.
  LOG(INFO) << "Performing initial Forward/Backward";
  const vector<shared_ptr<LayerBase> >& layers = caffe_net->layers();
  const vector<vector<Blob*> >& bottom_vecs = caffe_net->bottom_vecs();
  const vector<vector<Blob*> >& top_vecs = caffe_net->top_vecs();
  const vector<vector<bool> >& bottom_need_backward = caffe_net->bottom_need_backward();
  init_timer.Start();
  solver->Step(kInitIterations);
  double init_time = init_timer.MilliSeconds();
  LOG(INFO) << "Initial Forward/Backward complete";
  LOG(INFO) << "Average Initialization Forward/Backward pass: "
            << init_time / kInitIterations << " ms.";

  LOG(INFO) << "*** Benchmark begins ***";
  LOG(INFO) << "Testing for " << FLAGS_iterations << " iterations.";
  Timer total_timer;
  total_timer.Start();
  Timer timer;
  std::vector<double> forward_time_per_layer(layers.size(), 0.0);
  std::vector<double> backward_time_per_layer(layers.size(), 0.0);
  forward_time = 0.0;
  backward_time = 0.0;
  for (int j = 0; j < FLAGS_iterations; ++j) {
    Timer iter_timer;
    iter_timer.Start();
    forward_timer.Start();
    for (int i = 0; i < layers.size(); ++i) {
      timer.Start();
      layers[i]->Forward(bottom_vecs[i], top_vecs[i]);
      forward_time_per_layer[i] += timer.MicroSeconds();
    }
    forward_time += forward_timer.MicroSeconds();
    backward_timer.Start();
    for (int i = layers.size() - 1; i >= 0; --i) {
      timer.Start();
      layers[i]->Backward(top_vecs[i], bottom_need_backward[i],
                          bottom_vecs[i]);
      backward_time_per_layer[i] += timer.MicroSeconds();
    }
    backward_time += backward_timer.MicroSeconds();
    LOG(INFO) << "Iteration: " << j + 1 << " forward-backward time: "
      << iter_timer.MilliSeconds() << " ms.";
  }
  LOG(INFO) << "Average time per layer: ";
  for (int i = 0; i < layers.size(); ++i) {
    const caffe::string& layername = layers[i]->layer_param().name();
    LOG(INFO) << std::setfill(' ') << std::setw(10) << layername <<
      "\tforward: " << forward_time_per_layer[i] / 1000 /
      FLAGS_iterations << " ms.";
    LOG(INFO) << std::setfill(' ') << std::setw(10) << layername  <<
      "\tbackward: " << backward_time_per_layer[i] / 1000 /
      FLAGS_iterations << " ms.";
  }
  total_timer.Stop();
  LOG(INFO) << "Average Forward pass: " << forward_time / 1000 /
    FLAGS_iterations << " ms.";
  LOG(INFO) << "Average Backward pass: " << backward_time / 1000 /
    FLAGS_iterations << " ms.";
  LOG(INFO) << "Average Forward-Backward: " << total_timer.MilliSeconds() /
    FLAGS_iterations << " ms.";
  LOG(INFO) << "Total Time: " << total_timer.MilliSeconds() << " ms.";
  LOG(INFO) << "*** Benchmark ends ***";
  return 0;
}
RegisterBrewFunction(time);

int main(int argc, char** argv) {
  // Print output to stderr (while still logging).
  FLAGS_alsologtostderr = 1;
  // Set version
  gflags::SetVersionString(AS_STRING(CAFFE_VERSION));
  // Usage message.
  gflags::SetUsageMessage("command line brew\n"
      "usage: caffe <command> <args>\n\n"
      "commands:\n"
      "  train           train or finetune a model\n"
      "  test            score a model\n"
      "  device_query    show GPU diagnostic information\n"
      "  time            benchmark model execution time");
  // Run tool or show usage.
  // PS INIT
  ps::Start(0); //start with setting timeout
  caffe::GlobalInit(&argc, &argv);

  vector<int> gpus;
  get_gpus(&gpus);
#ifndef CPU_ONLY
  if (gpus.size() > 0) {
    Caffe::SetDevice(gpus[0]);
  }
#endif

  LOG(INFO) << "This is NVCaffe " << Caffe::caffe_version()
            << " started at " << Caffe::start_time();
#ifndef CPU_ONLY
  LOG(INFO) << "CuDNN version: " << Caffe::cudnn_version();
  LOG(INFO) << "CuBLAS version: " << Caffe::cublas_version();
  LOG(INFO) << "CUDA version: " << Caffe::cuda_version();
  LOG(INFO) << "CUDA driver version: " << Caffe::cuda_driver_version();
#endif
  int ret = 0;
  if (argc == 2) {
#ifdef WITH_PYTHON_LAYER
    try {
#endif
      ret = GetBrewFunction(caffe::string(argv[1]))();
#ifdef WITH_PYTHON_LAYER
    } catch (bp::error_already_set) {
      PyErr_Print();
      return 1;
    }
#endif
  } else {
    gflags::ShowUsageWithFlagsRestrict(argv[0], "tools/caffe");
  }
  // PS DISPOSE
  ps::Finalize(0, true);
  return ret;
}
