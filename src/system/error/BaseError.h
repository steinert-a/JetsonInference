#pragma once

#include <QString>
#include <cstdint>

namespace sys
{

// System Error Source
enum class SysErrSrc : std::uint32_t
{
    None = 0,
    System,
    Math,
    CStd,
};

const char* GetErrorSourceString(const SysErrSrc&) noexcept;


struct BaseError
{
public:
	BaseError() = default;
	~BaseError() = default;

private:
    // non-copyable	
	BaseError(const BaseError&) = delete;
	BaseError& operator=(const BaseError&) = delete;

public:
	virtual SysErrSrc source() noexcept = 0;
	virtual QString detailed() noexcept = 0;
};

}
