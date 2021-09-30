#pragma once

#include "System.h"

#include <opencv2/opencv.hpp>
#include "NvInfer.h"

namespace tensor {

class TensorEngine {
public:
    TensorEngine();
    ~TensorEngine();

private:
    // non-copyable	
    TensorEngine(const TensorEngine&) = delete;
    TensorEngine& operator=(const TensorEngine&) = delete;

private:
    nvinfer1::ICudaEngine* engine_;

    int* sizeBuffers_;
    void** deviceBuffers_;
    float** memoryBuffers_;

private:
    ERROR buildEngine(const QString& onnxFileName);
    ERROR allocateEngineBuffers();
    void deallocateEngineBuffers();
    ERROR copyMemoryToDevice();
    ERROR copyDeviceToMemory();

public:
    void deinit();
    ERROR build(const QString& onnxFileName);
    ERROR getInputBuffer(float*& buffer, int& size);
    ERROR getOutputBuffer(float*& buffer, int& size);
    ERROR setInputBuffer(cv::Mat& image);
    ERROR execute();
};

}
