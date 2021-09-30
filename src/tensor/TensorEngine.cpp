#include "TensorEngine.h"

#include <cuda_runtime_api.h>
#include <NvOnnxParser.h>

using namespace nvinfer1;
using namespace nvonnxparser;

namespace tensor {

TensorEngine::TensorEngine()
{
    engine_ = nullptr;
    sizeBuffers_ = nullptr;
    deviceBuffers_ = nullptr;
    memoryBuffers_ = nullptr;
}

TensorEngine::~TensorEngine()
{
    deinit();
}

void TensorEngine::deinit()
{
    deallocateEngineBuffers();
    DESTROY_POINTER(engine_);
}

ERROR TensorEngine::build(const QString& onnx)
{
    deinit();
    ERROR err = buildEngine(onnx);
    if(!err) err = allocateEngineBuffers();

    return err;
}

ERROR TensorEngine::buildEngine(const QString& onnxFileName)
{
    ERROR err = ERROR_NONE;
    TRT_LOGGER trtLogger;

    DESTROY_POINTER(engine_);
    IBuilder* builder = createInferBuilder(trtLogger);
    err  = ASSERTION_POINTER(builder);

    INetworkDefinition* network = nullptr;
    if(!err)
    {
        network = builder->createNetworkV2((int)NetworkDefinitionCreationFlag::kEXPLICIT_PRECISION);
        err  = ASSERTION_POINTER(network);
    }

    IBuilderConfig* config = nullptr;
    if(!err)
    {
        config = builder->createBuilderConfig();
        err  = ASSERTION_POINTER(config);
    }

    IParser* parser = nullptr;
    if(!err)
    {
        parser = createParser(*network, trtLogger);
        err  = ASSERTION_POINTER(parser);
    }

    if(!err && !onnxFileName.length()) err = ERROR_SYSTEM(Onnx, "Missing ONNX file name.");
    if(!err && !QFile::exists(onnxFileName)) err = ERROR_SYSTEM(Onnx, "ONNX file does not exist.");

    if(!err)
    {
        QByteArray onnxFileNameLocal = onnxFileName.toLocal8Bit();
        bool success = parser->parseFromFile(onnxFileNameLocal.constData(), 0);
        if(!success) err = ERROR_SYSTEM(Onnx, "ONNX file can not be parsed.");
    }

    if(!err)
    {
        config->setFlag(BuilderFlag::kGPU_FALLBACK);
        engine_ = builder->buildEngineWithConfig(*network, *config);
        err  = ASSERTION_POINTER(engine_);
    }

    DESTROY_POINTER(parser);
    DESTROY_POINTER(config);
    DESTROY_POINTER(network);
    DESTROY_POINTER(builder);
    
    return err;
}

ERROR TensorEngine::allocateEngineBuffers()
{
    ERROR err = ERROR_NONE;

    if(!err && sizeBuffers_) err = ERROR_SYSTEM(Pointer, "Size pointer is not null, can't create new device buffer.");
    if(!err && deviceBuffers_) err = ERROR_SYSTEM(Pointer, "Device pointer is not null, can't create new device buffer.");
    if(!err && memoryBuffers_) err = ERROR_SYSTEM(Pointer, "Memory pointer is not null, can't create new device buffer.");
    if(!err && !engine_) err = ERROR_SYSTEM(Initialization, "TensorRT engine is not initialized, can't create new device buffer.");

    int bindingCount = 0;
    if(!err)
    {
        bindingCount = engine_->getNbBindings();
        if(!bindingCount) err = ERROR_SYSTEM(Size, "TensorRT engine has no bindings, can't create new device buffer.");
    }

    if(!err)
    {
        sizeBuffers_ = new int[bindingCount];
        deviceBuffers_ = new void*[bindingCount];
        memoryBuffers_ = new float*[bindingCount];
        for(int idx=0; idx < bindingCount; ++idx)
        {
            Dims dim = engine_->getBindingDimensions(idx);
            sizeBuffers_[idx] =  1; // batch size is always 1
            for(int d=0; d < dim.nbDims; ++d) sizeBuffers_[idx] *= dim.d[d];

            int totalByteSize = sizeof(float) * sizeBuffers_[idx];
            cudaMalloc(&deviceBuffers_[idx], totalByteSize);

            memoryBuffers_[idx] = new float[sizeBuffers_[idx]];
        }
    }

    return err;
}

void TensorEngine::deallocateEngineBuffers()
{
    if(engine_)
    {
        if(deviceBuffers_)
        {
            for(int idx=0; idx < engine_->getNbBindings(); ++idx)
            {
                cudaFree(deviceBuffers_[idx]);
            }
            DELETE_ARRAY(deviceBuffers_);
        }
        
        if(memoryBuffers_)
        {
            for(int idx=0; idx < engine_->getNbBindings(); ++idx)
            {
                DELETE_ARRAY(memoryBuffers_[idx]);
            }
            DELETE_ARRAY(memoryBuffers_);
        }   
    }
    DELETE_ARRAY(sizeBuffers_);
}

ERROR TensorEngine::copyMemoryToDevice()
{
    ERROR err = ERROR_NONE;

    if(!err && !sizeBuffers_) err = ERROR_SYSTEM(Size, "Missin size information.");
    if(!err && !deviceBuffers_) err = ERROR_SYSTEM(Buffer, "Device is not initialized.");
    if(!err && !memoryBuffers_) err = ERROR_SYSTEM(Buffer, "Memory is not initialized.");
    if(!err && !engine_) err = ERROR_SYSTEM(Initialization, "Missing TensorRT engine.");

    for(int idx=0; idx < engine_->getNbBindings(); ++idx)
    {
        if(engine_->bindingIsInput(idx))
        {
            cudaMemcpy(deviceBuffers_[idx], memoryBuffers_[idx], sizeof(float) * sizeBuffers_[idx], cudaMemcpyHostToDevice);
        } 
    }
        
    return err;
}

ERROR TensorEngine::copyDeviceToMemory()
{    
    ERROR err = ERROR_NONE;

    if(!err && !sizeBuffers_) err = ERROR_SYSTEM(Size, "Missin size information.");
    if(!err && !deviceBuffers_) err = ERROR_SYSTEM(Buffer, "Device is not initialized.");
    if(!err && !memoryBuffers_) err = ERROR_SYSTEM(Buffer, "Memory is not initialized.");
    if(!err && !engine_) err = ERROR_SYSTEM(Initialization, "Missing TensorRT engine.");

    for(int idx=0; idx < engine_->getNbBindings(); ++idx)
    {
        if(!engine_->bindingIsInput(idx))
        {
            cudaMemcpy(memoryBuffers_[idx], deviceBuffers_[idx], sizeof(float) * sizeBuffers_[idx], cudaMemcpyDeviceToHost);
        } 
    }

    return err;
}

ERROR TensorEngine::getInputBuffer(float*& buffer, int& size)
{
    ERROR err = ERROR_NONE;

    if(!err && !sizeBuffers_) err = ERROR_SYSTEM(Size, "Missin size information.");
    if(!err && !memoryBuffers_) err = ERROR_SYSTEM(Buffer, "Memory is not initialized.");
    if(!err && !engine_) err = ERROR_SYSTEM(Initialization, "Missing TensorRT engine.");

    int inputCount = 0;
    int inputIdx = -1;

    if(!err)
    { 
        for(int idx=0; idx < engine_->getNbBindings(); ++idx)
        {
            if(engine_->bindingIsInput(idx))
            {
                inputCount++;
                inputIdx = idx;
            } 
        }
    }

    if(!err && inputIdx < 0) err = ERROR_SYSTEM(NeuralNetwork, "Missing input tensor.");
    if(!err && inputCount != 1) err = ERROR_SYSTEM(NeuralNetwork, "More then one input tensor.");

    if(!err)
    { 
        buffer = memoryBuffers_[inputIdx];
        size = sizeBuffers_[inputIdx];
    }

    return err;
}

ERROR TensorEngine::getOutputBuffer(float*& buffer, int& size)
{
    ERROR err = ERROR_NONE;

    if(!err && !sizeBuffers_) err = ERROR_SYSTEM(Size, "Missin size information.");
    if(!err && !memoryBuffers_) err = ERROR_SYSTEM(Buffer, "Memory is not initialized.");
    if(!err && !engine_) err = ERROR_SYSTEM(Initialization, "Missing TensorRT engine.");

    int outputCount = 0;
    int outputIdx = -1;

    if(!err)
    { 
        for(int idx=0; idx < engine_->getNbBindings(); ++idx)
        {
            if(!engine_->bindingIsInput(idx))
            {
                outputCount++;
                outputIdx = idx;
            } 
        }
    }

    if(!err && outputIdx < 0) err = ERROR_SYSTEM(NeuralNetwork, "Missing output tensor.");
    if(!err && outputCount != 1) err = ERROR_SYSTEM(NeuralNetwork, "More then one output tensor.");

    if(!err)
    { 
        buffer = memoryBuffers_[outputIdx];
        size = sizeBuffers_[outputIdx];
    }

    return err;
}

ERROR TensorEngine::setInputBuffer(cv::Mat& image)
{
    ERROR err = ERROR_NONE;
        
    float* inputBuffer = nullptr;
    int inputSize = 0;

    if(!err && !image.data) err = ERROR_SYSTEM(Buffer, "Image argument without data.");
    if(!err) err = getInputBuffer(inputBuffer, inputSize);

    int imageSizeTotal = 0;

    if(!err)
    {
        imageSizeTotal = image.rows * image.cols * image.channels();
        if(imageSizeTotal != inputSize) err = ERROR_SYSTEM(Size, "Buffer size missmatch.");
    }
    

    if(!err)
    { 
        for(int idx = 0; idx < imageSizeTotal; ++idx)
        {
            inputBuffer[idx] = float(image.data[idx]);
        }
    }

    return err;
}

ERROR TensorEngine::execute()
{
    ERROR err = ASSERTION_POINTER(engine_);
    IExecutionContext* context = engine_->createExecutionContext();
    if(!err) err = ASSERTION_POINTER(context);

    if(!err) err = copyMemoryToDevice();
    if(!err) context->execute(1, deviceBuffers_);
    if(!err) err = copyDeviceToMemory();
        
     DESTROY_POINTER(context);

     return err;
}

}
