#pragma once

#include <QString>

namespace sys
{
void QtLoggerHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);

}
