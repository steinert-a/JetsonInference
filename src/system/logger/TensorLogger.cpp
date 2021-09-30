#include "TensorLogger.h"
#include "SystemLogger.h"

namespace sys
{
void TensorLogger::log(Severity severity, const char* msg) TRTNOEXCEPT
{
    switch (severity)
    {
    case Severity::kINTERNAL_ERROR:
        sys::SystemLogger::Instance()->log(sys::SysLogLevel::Fatal, nullptr, nullptr, 0, msg);
        break;
    case Severity::kERROR:
        sys::SystemLogger::Instance()->log(sys::SysLogLevel::Error, nullptr, nullptr, 0, msg);
        break;
    case Severity::kINFO:
    case Severity::kVERBOSE:
        sys::SystemLogger::Instance()->log(sys::SysLogLevel::Info, nullptr, nullptr, 0, msg);
        break;
    case Severity::kWARNING:
    default:
        sys::SystemLogger::Instance()->log(sys::SysLogLevel::Warning, nullptr, nullptr, 0, msg);
        break;
    }
}

}
