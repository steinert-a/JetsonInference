# Jetson Inference
The `Jetson Inference` project is used to deploy a pre-trained model with the `Jetson TensorFlow Run Time` (`TensorRT`).


## Development
This project does use some complex sofware technics. In order to simplify the understanding some general hints are given.

### `CMake` variables
|variable|description|
|-|-|
|CMAKE_SOURCE_DIR|Directory of the top-level CMakeLists.txt file.|
|PROJECT_SOURCE_DIR|Directory of the most recent `project()` call.|

### `C++` Style Guide
The style guide used here is based on the [Google C++ Style Guide](https://google.github.io/styleguide/cppguide.html).
* MyClassName.cpp / MyClassName.h
* class MyClass / myClassMember_ / myClassMemberFunction
* struct MyStruct
* GlobalFunction / parameter
* #define MY_MACRO 123
* enum class EnumTypeName {EnumValue = 0, ... };

### `TensorRT` terminology
|term|description|
|-|-|
|Deep Learning Accelerator (DLA)|DLA also called AI Accelerator is a class of hardware acceleration for machine learning algorithms.|
|Open Neural Network Exchange (ONNX)|ONNX is a standard developed to exchange models between frameworks.|


## Dependencies
For the build of the `Jetson Inference` project different dependencies are needed. This section does describe how to make the requirements available.

### Qt
`Qt` library is used for basic `C++` tasks under the `LGPL` license, like parameter line parsing or logging.

* [Qt](https://www.qt.io)
* [Qt Offline Installer](https://www.qt.io/offline-installers)

`Qt` library can be installed manually by compiling the `Qt` source on the target system.

```bash
wget https://download.qt.io/official_releases/qt/5.15/5.15.2/single/qt-everywhere-src-5.15.2.tar.xz
tar -xpf qt-everywhere-src-5.15.2.tar.xz
cd qt-everywhere-src-5.15.2/
make
make install
```

The build takes several hours/days. For this it is helpful to run the build in the bakground with the `&` symbol at the end of the command. This wil run the build in a sub-terminal and can be detached from the current terminal.

```bash
cd qt-everywhere-src-5.15.2/
make -j4 > build_stream.log &
# [1] <your-PID>
disown -h <your-PID>
# view log file
tail -f ./build_stream.log
```

### OpenCV
Useful links with information about `OpenCV`:
* [OpenCV Official Homepage](https://opencv.org/)
* [OpenCV Tutorials Point](https://www.tutorialspoint.com/opencv/index.htm)
* [OpenCV GitHub](https://github.com/opencv/opencv)

`OpenCV` installation instruction:
```bash
git clone https://github.com/opencv/opencv.git
cd opencv
mkdir build
cd build
cmake -D CMAKE_BUILD_TYPE=RELEASE -D BUILD_EXAMPLES=OFF -D BUILD_TESTS=OFF -D BUILD_PERF_TESTS=OFF -D BUILD_EXAMPLES=OFF ..
make
make install
```

### TensorRT
Useful links with information about `TensorRT`:
* https://docs.nvidia.com/deeplearning/tensorrt/api/c_api/namespacenvinfer1.html


## Build
Building the `Jetson Inference` project can be done by executing the known commands.

```bash
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
```
