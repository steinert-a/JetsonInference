#pragma once

#include "BaseError.h"

#include <cstdint>
#include <QString>

namespace sys
{

// System Error Code
enum class SysErrCode : std::uint64_t
{
    None = 0,
    
    // procedure
    Assertion,
    Permission,
    Initialization,	
    Conversion,
    Version,
    Validation,
    Access,
    Timeout,

    // files
    Ini,
    Xml,
    Cmd,
    Tga,
    Bmp,
    Pgm,
    Cf,
    Cfg,
    Onnx,

    // device
    Printer,
    Camera,

    // unexpected
    Pointer,
    Buffer,
    Array,
    Iterator,
    Flag,
    Thread,
    Path,
    Type,
    Exception,
    Index,
    Enumeration,
    Parameter,
    Stream,
    Size,
    NeuralNetwork,

    // pattern 
    Com,
    Singleton,
    Bridge,
};

const char* GetSystemErrorCodeString(const SysErrCode&) noexcept;


struct SystemError : public BaseError
{
public:
	SystemError(const SysErrCode&, const char*);
	~SystemError() = default;

private:
    // non-copyable	
	SystemError(const SystemError&) = delete;
	SystemError& operator=(const SystemError&) = delete;

private:
	SysErrCode	code_;
	QString message_;

public:
	SysErrSrc source() noexcept override;
	QString detailed() noexcept override;
};

}
