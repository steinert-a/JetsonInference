#include "JetsonInference.h"

#include "System.h"
#include "QtLoggerHandler.h"

#include <cuda_runtime_api.h>
#include <NvInferVersion.h>
#include <NvOnnxParser.h>

#include <QtCore/QCoreApplication>

#include <opencv2/opencv.hpp>

using namespace std;

void LogVersionInformation()
{
    LOG_INFO("Jetson Nano inference v%s", VERSION_STRING);
    LOG_INFO("Qt v%s", qVersion());
    LOG_INFO("OpenCV v%s", CV_VERSION);
    LOG_INFO("TensorRT v%i.%i.%i", NV_TENSORRT_MAJOR, NV_TENSORRT_MINOR, NV_TENSORRT_PATCH);
    int cudaRuntimeVersion = 0;
    cudaError error = cudaRuntimeGetVersion(&cudaRuntimeVersion);
    if(error == cudaError::cudaSuccess)
    { // (1000 major + 10 minor)
        LOG_INFO("CUDA RT v%i.%i", cudaRuntimeVersion / 1000, (cudaRuntimeVersion % 1000) / 10);        
    }
    LOG_INFO("NVIDIA ONNX Parser v%i.%i.%i", NV_ONNX_PARSER_MAJOR, NV_ONNX_PARSER_MINOR, NV_ONNX_PARSER_PATCH);
}

int main(int argc, char *argv[])
{
    qInstallMessageHandler(sys::QtLoggerHandler);

    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Jetson Inference");
    QCoreApplication::setApplicationVersion(VERSION_STRING);

    GLOBAL_CONFIG->processCommandLine(app.arguments());
    LogVersionInformation();

    JetsonInference* inference = new JetsonInference(&app);
    QObject::connect(inference, SIGNAL(finished()), &app, SLOT(quit()));
    QTimer::singleShot(0, inference, SLOT(run()));

    return app.exec();
}
