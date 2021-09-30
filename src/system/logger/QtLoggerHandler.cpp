#include "QtLoggerHandler.h"

#include "SystemLogger.h"

namespace sys
{
void QtLoggerHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    
    switch (type) {
    case QtDebugMsg:
    case QtInfoMsg:
        SystemLogger::Instance()->log(SysLogLevel::Info, context.file, context.function, context.line, localMsg.constData());
        break;
    case QtCriticalMsg:
        SystemLogger::Instance()->log(SysLogLevel::Error, context.file, context.function, context.line, localMsg.constData());
        break;
    case QtFatalMsg:
        SystemLogger::Instance()->log(SysLogLevel::Fatal, context.file, context.function, context.line, localMsg.constData());
        break;
    case QtWarningMsg:
    default:
        SystemLogger::Instance()->log(SysLogLevel::Warning, context.file, context.function, context.line, localMsg.constData());
        break;
    }
}

}
