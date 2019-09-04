scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate SSP 0
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate SSP 10000
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate SSP 3
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 1.0
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 0.8
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 0.5
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 0.3
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 0.1
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 0.01
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 0.0
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate SSP 5
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 1.0
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 0.8
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 0.5
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 0.3
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 0.1
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 0.01
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_stream build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 0.0
cp -rf ~/ddl/logs ~/cifar_logs_stream
cp -rf /localfs/xywu/ddl/logs /localfs/xywu/cifar_logs_stream 
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate SSP 0
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate SSP 10000
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate SSP 3
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 1.0
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 0.8
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 0.5
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 0.3
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 0.1
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 0.01
scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 3 0.0
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate SSP 5
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 1.0
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 0.8
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 0.5
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 0.3
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 0.1
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 0.01
#scripts/mrun.sh 1 64 worker/caffe examples/ps/test_ssp_block build/tools/caffe examples/cifar10/cifar10_quick_solver.prototxt examples/cifar10/cifar10_quick_iter_1.solverstate PSSP 5 0.0
cp -rf ~/ddl/logs ~/cifar_logs_block
cp -rf /localfs/xywu/ddl/logs /localfs/xywu/cifar_logs_block
