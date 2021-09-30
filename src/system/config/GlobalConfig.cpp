#include "GlobalConfig.h"

#include "Macros.h"

#include <QCommandLineParser>

namespace sys
{
GlobalConfig::GlobalConfig()
{
	// assert default constructor is thread safe
}

GlobalConfig::~GlobalConfig()
{
    DELETE_POINTER(settingsIni_);
}


GlobalConfig* GlobalConfig::Instance()
{
	static GlobalConfig GlobalCfg;
	return &GlobalCfg;
}

QSettings* GlobalConfig::getIniSettings()
{
    if (!settingsIni_)
    {
        settingsIni_ = new QSettings(QCoreApplication::applicationDirPath() + "/JetsonInference.ini", QSettings::IniFormat);
    }

    return settingsIni_;
}

void GlobalConfig::processCommandLine(const QStringList &arguments)
{
    std::lock_guard<std::mutex> guard(mutex_);

    QCommandLineParser parser;
    parser.setApplicationDescription("Jetson Nano Inference with TensorRT");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption onnxOption(QStringList() << "n" << "onnx",
            "Neural Network ONNX model <file>.", "file");
    parser.addOption(onnxOption);

    parser.process(arguments);

    onnxFileName_ = parser.value(onnxOption);
}

QString GlobalConfig::getApplicationDirectory()
{
    std::lock_guard<std::mutex> guard(mutex_);

    return QCoreApplication::applicationDirPath();
}

QString GlobalConfig::getOnnxFileName()
{
    std::lock_guard<std::mutex> guard(mutex_);

    return onnxFileName_;
}

void GlobalConfig::setIniValue(const QString &key, const QVariant &value)
{
    std::lock_guard<std::mutex> guard(mutex_);
    // setValue is already thread save
    getIniSettings()->setValue(key,value);
}

QVariant GlobalConfig::getIniValue(const QString &key, const QVariant &defaultValue)
{
    std::lock_guard<std::mutex> guard(mutex_);
    // value is already thread save
    return getIniSettings()->value(key, defaultValue);
}

}
