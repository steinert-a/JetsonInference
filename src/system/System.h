#pragma once

#include "GlobalConfig.h"
#include "BaseError.h"
#include "SystemError.h"
#include "CStdError.h"
#include "SystemLogger.h"
#include "TensorLogger.h"
#include "Macros.h"

#include <memory>

#define GLOBAL_CONFIG sys::GlobalConfig::Instance()

typedef std::shared_ptr<sys::BaseError> ERROR;

#define ERROR_NONE nullptr
#define ERROR_SYSTEM(code, msg) ERROR(new sys::SystemError(sys::SysErrCode::code, msg))
#define ERROR_CSTD(en) ERROR(new sys::CStdError(en))
#define ASSERTION_POINTER(exp) ((exp)?ERROR_NONE : ERROR(new sys::SystemError(sys::SysErrCode::Pointer, "NULL-Pointer")))

#define TRT_LOGGER sys::TensorLogger
#define LOG_INFO(format, ...) sys::SystemLogger::Instance()->log(sys::SysLogLevel::Info, nullptr, nullptr, 0, format, ##__VA_ARGS__)
#ifdef DEBUG
#define LOG_WARN(format, ...) sys::SystemLogger::Instance()->log(sys::SysLogLevel::Warning, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) sys::SystemLogger::Instance()->log(sys::SysLogLevel::Error, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) sys::SystemLogger::Instance()->log(sys::SysLogLevel::Fatal, __FILE__, __FUNCTION__, __LINE__, format, ##__VA_ARGS__)
#else
#define LOG_WARN(format, ...) sys::SystemLogger::Instance()->log(sys::SysLogLevel::Warning, nullptr, nullptr, 0, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) sys::SystemLogger::Instance()->log(sys::SysLogLevel::Error, nullptr, nullptr, 0, format, ##__VA_ARGS__)
#define LOG_FATAL(format, ...) sys::SystemLogger::Instance()->log(sys::SysLogLevel::Fatal, nullptr, nullptr, 0, format, ##__VA_ARGS__)
#endif
