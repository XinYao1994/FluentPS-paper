#!/usr/bin/env sh

TOOLS=/local_stuff/caffe/build/tools

$TOOLS/caffe train \
  --solver=network/cifar10_quick_solver.prototxt # the output of this training is cifar10_quick_iter_4000.solverstate


# reduce learning rate by factor of 10 after 8 epochs
$TOOLS/caffe train \
  --solver=network/cifar10_quick_solver_lr1.prototxt \
  --snapshot=network/cifar10_quick_iter_4000.solverstate ## THIS IS WHAT WE NEED FOR WARM START
