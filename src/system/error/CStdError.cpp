#include "CStdError.h"

#include <QString>

using namespace std;

namespace sys
{
CStdError::CStdError(const int& error)
{
	errno_ = error;
}

SysErrSrc CStdError::source() noexcept
{
	return SysErrSrc::CStd; 
}

QString CStdError::detailed() noexcept
{	
    QString details;
	details.reserve(1024);

	details += GetErrorSourceString(source());
	details += ": ";
	details += QString::number(errno_);
	details += " - ";
	details += strerror(errno_);

	return details;
}
}
