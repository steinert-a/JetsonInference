#pragma once

#include "NvInferRuntimeCommon.h"

namespace sys
{
class TensorLogger: public nvinfer1::ILogger
{
public:
    TensorLogger() = default;
    ~TensorLogger() = default;

public:
    void log(Severity severity, const char* msg) TRTNOEXCEPT override;
};

}
