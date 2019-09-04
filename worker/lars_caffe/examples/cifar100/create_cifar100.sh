#!/usr/bin/env sh
# This script converts the cifar data into leveldb format.

EXAMPLE=examples/cifar100/
#DATA=data/cifar10
DBTYPE=lmdb

echo "Creating $DBTYPE..."

cd $EXAMPLE/scripts/
python convert_cifar100_lmdb.py

cd ../../../

echo "Computing image mean..."

./build/tools/compute_image_mean -backend=$DBTYPE \
  $EXAMPLE/scripts/cifar100_train_$DBTYPE $EXAMPLE/mean.binaryproto

echo "Done."
