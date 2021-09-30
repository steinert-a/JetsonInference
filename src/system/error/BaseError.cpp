#include "BaseError.h"

namespace sys
{

const char* GetErrorSourceString(const SysErrSrc& src) noexcept
{
    const char* source = nullptr;

	switch(src)
	{
	case SysErrSrc::System:
		source = "System (SYS)";
		break;
	case SysErrSrc::Math:
		source = "Mathematics";
		break;
	case SysErrSrc::CStd:
		source = "Standard C++ Library";
		break;
	default:
		source = "Unknown Source";
		break;
	}

	return source;
}

}
