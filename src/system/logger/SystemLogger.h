#pragma once

#include <QString>
#include <QFile>

#include <optional>
#include <mutex>

namespace sys
{
// System Logger Levels
enum class SysLogLevel : std::uint32_t
{
    Info = 0,
    Warning,
    Error,
    Fatal,
};

class SystemLogger
{
private:
	SystemLogger();
	~SystemLogger();	

    // non-copyable
	SystemLogger(const SystemLogger&) = delete;
	SystemLogger& operator=(const SystemLogger&) = delete;

public:
	static SystemLogger* Instance();

private:
	std::mutex mutex_;
    QFile *logfile_;
    
private:
    QStringList getAbsoluteLogFileList();
    QString getLoggerDirectory();
    QString getNextLogFileBaseName();
    QString getNextLogFileAbsolute();
    unsigned getMaxLogFileByteSize();
    unsigned getMaxLogFileCount();
    void writeLineLogFile(const QString& message);
    void deleteOutdatedLogFiles();
    
    QString getTimestamp();
    const char* getSysLogLevelText(const SysLogLevel& level) noexcept;
    QString getMessageText(const char* format, va_list args);
    std::optional<QString> getDebugText(const char* file, const char* func, int line);

    QString formatLogMessage(const SysLogLevel& level, const char* file, const char* func, int line, const char* format, va_list args);
    void logMessage(const SysLogLevel& level, const char* file, const char* func, int line, const char* format, va_list args);

public: // thread safe functions
    void log(const SysLogLevel& level, const char* file, const char* func, int line, const char* format, ...);
};

}
