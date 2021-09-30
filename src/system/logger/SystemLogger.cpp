#include "SystemLogger.h"
#include "GlobalConfig.h"
#include "Macros.h"

#include <QDateTime>
#include <QVariant>
#include <QDir>
#include <QTextStream>

#include <vector>
#include <algorithm>

using namespace std;

namespace sys
{
SystemLogger::SystemLogger()
{
    logfile_ = nullptr;
}

SystemLogger::~SystemLogger()
{
    DELETE_POINTER(logfile_);
}

SystemLogger* SystemLogger::Instance()
{
	static SystemLogger SystemLog;
	return &SystemLog;
}

QString SystemLogger::getTimestamp()
{
    QDateTime current = QDateTime::currentDateTime().toUTC();
    QString timestamp = current.toString(Qt::ISODateWithMs);

    return timestamp;
}

QString SystemLogger::getLoggerDirectory()
{
    QString logDirectory;
    QVariant settingLogDir = GlobalConfig::Instance()->getIniValue(CONFIG_KEY_LOG_DIRECTORY);

    logDirectory = settingLogDir.toString();
    if(!logDirectory.length() || !QDir(logDirectory).exists())
    {
        logDirectory = GlobalConfig::Instance()->getApplicationDirectory() + QString("/logger");
        QDir().mkdir(logDirectory);
        GlobalConfig::Instance()->setIniValue(CONFIG_KEY_LOG_DIRECTORY, logDirectory);
    }

    return logDirectory;
}

unsigned SystemLogger::getMaxLogFileByteSize()
{
    unsigned maxLogFileSize = 0;
    QVariant settingMaxLogFileSize = GlobalConfig::Instance()->getIniValue(CONFIG_KEY_LOG_MAX_FILE_BYTE_SIZE);

    maxLogFileSize = settingMaxLogFileSize.toUInt();
    if(!maxLogFileSize)
    {
        maxLogFileSize = 1000000; // 1 MB
        GlobalConfig::Instance()->setIniValue(CONFIG_KEY_LOG_MAX_FILE_BYTE_SIZE, maxLogFileSize);
    }

    return maxLogFileSize;
}

unsigned SystemLogger::getMaxLogFileCount()
{
    unsigned maxLogFileCount = 0;
    QVariant settingMaxLogFileSize = GlobalConfig::Instance()->getIniValue(CONFIG_KEY_LOG_MAX_FILE_COUNT);

    maxLogFileCount = settingMaxLogFileSize.toUInt();
    if(!maxLogFileCount)
    {
        maxLogFileCount = 10;
        GlobalConfig::Instance()->setIniValue(CONFIG_KEY_LOG_MAX_FILE_COUNT, maxLogFileCount);
    }

    return maxLogFileCount;
}

const char* SystemLogger::getSysLogLevelText(const SysLogLevel& level) noexcept
{
    const char*  levelText = nullptr;
 
    switch(level)
    {
    case SysLogLevel::Info:
        levelText = "INFO"; 
        break;
    case SysLogLevel::Warning:
        levelText = "WARN"; 
        break;
    case SysLogLevel::Error:
        levelText = "FAIL";
        break;
    case SysLogLevel::Fatal:
        levelText = "KILL";
        break;
    default:
        levelText = "NONE";
        break;
    }

    return levelText;
}

std::optional<QString> SystemLogger::getDebugText(const char* file, const char* func, int line)
{
    std::optional<QString> debugText = std::nullopt;

    if(file && func && line)
    {
        QString text;
        text.reserve(1024);
        text += "[";
        text += file;
        text += ":";
        text += QString::number(line);
        text += "> ";
        text += func;
        text += "]";

        debugText = text;
    }

    return debugText;
}

QString SystemLogger::getMessageText(const char* format, va_list args)
{
    char acBuffer[2048] = {0};
    vsnprintf(acBuffer,2048,format,args);
    return QString(acBuffer);
}

QString SystemLogger::formatLogMessage(const SysLogLevel& level, const char* file, const char* func, int line, const char* format, va_list args)
{
    QString timestamp = getTimestamp();
    QString levelText = getSysLogLevelText(level);
    QString messageText = getMessageText(format, args);
    optional<QString>  debugText = getDebugText(file, func, line);

    QString message;
    message.reserve(1024);
    message += timestamp;
    message += " ";
    message += levelText;
    message += " ";
    message += messageText;

    if(debugText)
    {
        message += " ";
        message += debugText.value();
    }
    
    return message;
}

QString SystemLogger::getNextLogFileBaseName()
{
    QDateTime current = QDateTime::currentDateTime().toUTC();
    QString timestamp = current.toString("yyyy-MM-dd-hh-mm-ss-zzzZ") + QString(".log");

    return timestamp;  
}

QString SystemLogger::getNextLogFileAbsolute()
{
    return getLoggerDirectory() + QString("/") + getNextLogFileBaseName();
}

QStringList SystemLogger::getAbsoluteLogFileList()
{
    QStringList filters;
    filters << "*.log";
    QDir targetDir(getLoggerDirectory());

    
    QStringList logFiles = targetDir.entryList(filters, QDir::Files);
    QStringList logFilesAbsolute;

    foreach(QString filename, logFiles) {
        logFilesAbsolute << getLoggerDirectory() + QString("/") + filename; 
    }

    return logFilesAbsolute;
}

void SystemLogger::deleteOutdatedLogFiles()
{
    QStringList logFileList = getAbsoluteLogFileList();
    unsigned maxFileCount = getMaxLogFileCount();

    if(logFileList.size() && logFileList.size() > maxFileCount)
    {
        vector<QFileInfo> fileInofList(logFileList.size());
        for (int i = 0; i < logFileList.size(); ++i)
        {
            fileInofList[i].setFile(logFileList.at(i));
        }

        sort(fileInofList.begin(), fileInofList.end(), [](const QFileInfo& infoA, const QFileInfo& infoB)->bool {
            return infoA.lastModified() > infoB.lastModified();
        });

        for (int i = maxFileCount; i < fileInofList.size(); ++i)
        {
            QFile file (fileInofList[i].absoluteFilePath());
            file.remove();
        }
    }
}

void SystemLogger::writeLineLogFile(const QString& message)
{
    if(!logfile_ || logfile_->size() > getMaxLogFileByteSize())
    {
        DELETE_POINTER(logfile_);
        logfile_ = new QFile(getNextLogFileAbsolute());
        logfile_->open(QIODevice::Append | QIODevice::Text);
        deleteOutdatedLogFiles();
    }

    QTextStream textStream(logfile_);
    textStream.setCodec("UTF-8");
    textStream << message << "\n";
    textStream.flush();
}

void SystemLogger::logMessage(const SysLogLevel& level, const char* file, const char* func, int line, const char* format, va_list args)
{
    QString message = formatLogMessage(level, file, func, line, format, args);

    writeLineLogFile(message);
    printf("%s\n",message.toStdString().c_str());
}

void SystemLogger::log(const SysLogLevel& level, const char* file, const char* func, int line, const char* format, ...)
{
    std::lock_guard<std::mutex> guard(mutex_);

    va_list args;
    va_start(args, format);
    logMessage(level, file, func, line, format, args); 
	va_end(args);
}

}
