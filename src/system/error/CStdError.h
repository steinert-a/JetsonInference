#pragma once

#include "BaseError.h"

namespace sys
{
class CStdError : public BaseError
{
public:
    CStdError(const int &);
    ~CStdError() = default;
private:
    // non-copyable
    CStdError(const CStdError &) = delete;
    CStdError &operator=(const CStdError &) = delete;

public:
    int errno_;

public:
	SysErrSrc source() noexcept override;
	QString detailed() noexcept override;
};

}
