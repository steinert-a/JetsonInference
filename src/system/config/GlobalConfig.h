#pragma once

#include <QtCore/QCoreApplication>

#include <mutex>

#include <QSettings>

#define CONFIG_KEY_LOG_DIRECTORY "LOGGER/DIRECTORY"
#define CONFIG_KEY_LOG_MAX_FILE_BYTE_SIZE "LOGGER/MAX_FILE_BYTE_SIZE"
#define CONFIG_KEY_LOG_MAX_FILE_COUNT "LOGGER/MAX_FILE_COUNT"

namespace sys
{
class GlobalConfig
{
private:
	GlobalConfig();
	~GlobalConfig();	

    // non-copyable
	GlobalConfig(const GlobalConfig&) = delete;
	GlobalConfig& operator=(const GlobalConfig&) = delete;

public:
	static GlobalConfig* Instance();

private:
	std::mutex mutex_;
    QSettings* settingsIni_;
    QString onnxFileName_;

private:
    QSettings* getIniSettings();

public: // thread safe functions
    QString getApplicationDirectory();
    QString getOnnxFileName();
    void setIniValue(const QString &key, const QVariant &value);
    QVariant getIniValue(const QString &key, const QVariant &defaultValue = QVariant());
    void processCommandLine(const QStringList &arguments);
};
}
