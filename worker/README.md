# Projects as worker

## caffe

Refer to the [caffe](https://github.com/XinYao1994/caffe)

- Basic dependencies:
``` 
    sudo apt-get install -y build-essential git libopenblas-dev liblapack-dev libopencv-dev python-dev python-setuptools python-pip libgfortran3 graphviz cmake
    sudo apt-get install -y --no-install-recommends libboost-all-dev
    sudo apt-get install -y libtool autoconf libcurl4-openssl-dev curl zip
    sudo apt-get install -y libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libhdf5-serial-dev protobuf-compiler
    sudo apt-get install -y libgflags-dev libgoogle-glog-dev liblmdb-dev
    sudo apt-get remove -y libprotobuf-dev protobuf-compiler
``` 
install ZMQ and protobuf-3.5.0

``` 
mkdir build
cd build
cmake -DBLAS=Open .. # you can use other blas libs
make all -j $(nproc)
``` 

## nvcaffe

```
git clone https://github.com/libjpeg-turbo/libjpeg-turbo.git
mkdir build
cd build
cmake ..
make -j6
sudo make install
```

cmake

```
./bootstrap --system-curl
```
