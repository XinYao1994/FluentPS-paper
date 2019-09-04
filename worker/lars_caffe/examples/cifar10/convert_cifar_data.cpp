//
// This script converts the CIFAR dataset to the leveldb format used
// by caffe to perform classification.
// Usage:
//    convert_cifar_data input_folder output_db_file
// The CIFAR dataset could be downloaded at
//    http://www.cs.toronto.edu/~kriz/cifar.html

#include <fstream>  // NOLINT(readability/streams)
#include <string>

#include "boost/scoped_ptr.hpp"
#include "glog/logging.h"
#include "google/protobuf/text_format.h"
#include "stdint.h"

#include "caffe/proto/caffe.pb.h"
#include "caffe/util/db.hpp"
#include "caffe/util/format.hpp"

//Yaodong
#include <random> // std::default_random_engine
#include <ctime>
//Xin YAO
#include <algorithm>    // std::shuffle
#include <array>        // std::array

using caffe::Datum;
using boost::scoped_ptr;
using std::string;
namespace db = caffe::db;

const int kCIFARSize = 32;
const int kCIFARImageNBytes = 3072;
const int kCIFARBatchSize = 10000;
const int kCIFARTrainBatches = 5;

void read_image(std::ifstream* file, int* label, char* buffer) {
  char label_char;
  file->read(&label_char, 1);
  *label = label_char;
  file->read(buffer, kCIFARImageNBytes);
  return;
}

void convert_dataset(const string& input_folder, const string& output_folder,
    const string& db_type) {
  scoped_ptr<db::DB> train_db(db::GetDB(db_type));
  train_db->Open(output_folder + "/cifar10_train_" + db_type, db::NEW);
  scoped_ptr<db::Transaction> txn(train_db->NewTransaction());
  // Data buffer
  int label;
  char str_buffer[kCIFARImageNBytes];
  Datum datum;
  datum.set_channels(3);
  datum.set_height(kCIFARSize);
  datum.set_width(kCIFARSize);

  //Yaodong
  // int *arr = new int(kCIFARBatchSize);
  //int arr[kCIFARBatchSize];
  
  // Xin YAO: You can directly use std::shuffle, refer to http://www.cplusplus.com/reference/algorithm/shuffle/
  std::array<int, kCIFARBatchSize> arr;

  LOG(INFO) << "Writing Training data";
  for (int fileid = 0; fileid < kCIFARTrainBatches; ++fileid) {
    // Open files
    LOG(INFO) << "Training Batch " << fileid + 1;
    string batchFileName = input_folder + "/data_batch_"
      + caffe::format_int(fileid+1) + ".bin";
    std::ifstream data_file(batchFileName.c_str(),
        std::ios::in | std::ios::binary);

    //Yaodong shuffle data
    for (int i = 0; i < kCIFARBatchSize; i++) {
      arr[i] = i;
    }

    CHECK(data_file) << "Unable to open train file #" << fileid + 1;
    // Xin YAO
    std::shuffle (arr.begin(), arr.end(), std::default_random_engine(time(NULL)));
    for (int itemid = 0; itemid < kCIFARBatchSize; ++itemid) {
      read_image(&data_file, &label, str_buffer);
      datum.set_label(label);
      datum.set_data(str_buffer, kCIFARImageNBytes);
      string out;
      CHECK(datum.SerializeToString(&out));

      // Yaodong shuffle data
      /*
      static std::default_random_engine e(time(NULL));
      static std::uniform_int_distribution<int> u(itemid, kCIFARBatchSize-1);
      int tmp_id = u(e);
      int tmp = arr[itemid];
      arr[itemid] = arr[tmp_id];
      arr[tmp_id] = tmp;*/

      // update
      txn->Put(caffe::format_int(fileid * kCIFARBatchSize + arr[itemid], 5), out);
    }

  }
  
  txn->Commit();
  train_db->Close();

  LOG(INFO) << "Writing Testing data";
  scoped_ptr<db::DB> test_db(db::GetDB(db_type));
  test_db->Open(output_folder + "/cifar10_test_" + db_type, db::NEW);
  txn.reset(test_db->NewTransaction());
  // Open files
  std::ifstream data_file((input_folder + "/test_batch.bin").c_str(),
      std::ios::in | std::ios::binary);
  CHECK(data_file) << "Unable to open test file.";
  // shuffle
  // std::shuffle (arr.begin(), arr.end(), std::default_random_engine(time(NULL)));
  // Xin YAO, do not need shuffle for test data
  for (int i = 0; i < kCIFARBatchSize; i++) {
    arr[i] = i;
  }
  for (int itemid = 0; itemid < kCIFARBatchSize; ++itemid) {
    read_image(&data_file, &label, str_buffer);
    datum.set_label(label);
    datum.set_data(str_buffer, kCIFARImageNBytes);
    string out;
    CHECK(datum.SerializeToString(&out));

    //Yaodong
    /*
    static std::default_random_engine e(time(NULL));
    static std::uniform_int_distribution<int> u(itemid, kCIFARBatchSize-1);

    int tmp_id = u(e);
    int tmp = arr[itemid];
    arr[itemid] = arr[tmp_id];
    arr[tmp_id] = tmp;*/

    // update
    txn->Put(caffe::format_int(arr[itemid], 5), out);
  }
  txn->Commit();
  test_db->Close();
}

int main(int argc, char** argv) {
  FLAGS_alsologtostderr = 1;

  if (argc != 4) {
    printf("This script converts the CIFAR dataset to the leveldb format used\n"
           "by caffe to perform classification.\n"
           "Usage:\n"
           "    convert_cifar_data input_folder output_folder db_type\n"
           "Where the input folder should contain the binary batch files.\n"
           "The CIFAR dataset could be downloaded at\n"
           "    http://www.cs.toronto.edu/~kriz/cifar.html\n"
           "You should gunzip them after downloading.\n");
  } else {
    google::InitGoogleLogging(argv[0]);
    convert_dataset(string(argv[1]), string(argv[2]), string(argv[3]));
  }
  return 0;
}
