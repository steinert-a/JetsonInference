#include "JetsonInference.h"

#include <opencv2/opencv.hpp>

JetsonInference::JetsonInference(QObject *parent) : QObject(parent)
{}

JetsonInference::~JetsonInference()
{
    engine_.deinit();
}

void JetsonInference::run()
{
    ERROR err = ERROR_NONE;
    err = engine_.build(GLOBAL_CONFIG->getOnnxFileName());

    cv::Mat img;
    if(!err)
    {
        img = cv::imread("<your-absolute-dir>/JetsonInference/res/4.pgm", cv::IMREAD_UNCHANGED);
        if (img.empty()) err = ERROR_SYSTEM(Pgm, "Can't load file.");
    }
    if(!err) err = engine_.setInputBuffer(img);
    if(!err) err = engine_.execute();

    float* outBuffer = nullptr;
    int outSize = 0;
    if(!err) err = engine_.getOutputBuffer(outBuffer, outSize);
    
    if(!err)
    {
        std::string message = "Inference:";
        message.reserve(1024);
        for(int i=0; i<outSize; ++i){
            message += " [";
            message += std::to_string(i);
            message += "]";
            message += std::to_string(outBuffer[i]);
        }
        LOG_INFO(message.c_str());
    }

    if(err) LOG_ERROR(err->detailed().toStdString().c_str());
    emit finished();
}
