#include "SystemError.h"

using namespace std;

namespace sys
{

    
const char* GetSystemErrorCodeString(const SysErrCode& code) noexcept
{	
	const char* error = nullptr;

	switch(code)
	{	
	case SysErrCode::None:
		error = "Succeed / No Error";
		break;	
	case SysErrCode::Assertion:
		error = "Assertion Failed";
		break;	

	case SysErrCode::Conversion:
		error = "Conversion Failed";
		break;
	case SysErrCode::Version:
		error = "Version Mismatch";
		break;
	case SysErrCode::Permission:
		error = "Permission Denied";
		break;
	case SysErrCode::Initialization:
		error = "Initialisation Undone";
		break;
	case SysErrCode::Validation:
		error = "Validation Failed";
		break;
	case SysErrCode::Access:
		error = "Access Failed";
		break;
	case SysErrCode::Timeout:
		error = "Timeout";
		break;

	case SysErrCode::Pointer:
		error = "Unexpected Pointer";
		break;	
	case SysErrCode::Buffer:
		error = "Unexpected Buffer";
		break;
	case SysErrCode::Array:
		error = "Unexpected Array";
		break;	
	case SysErrCode::Iterator:
		error = "Unexpected Iterator";
		break;	
	case SysErrCode::Flag:
		error = "Unexpected Flag";
		break;	
	case SysErrCode::Thread:
		error = "Unexpected Thread Kill";
		break;
	case SysErrCode::Path:
		error = "Unexpected Path";
		break;
	case SysErrCode::Type:
		error = "Unexpected Type";
		break;
	case SysErrCode::Exception:
		error = "Unexpected Exception";
		break;
	case SysErrCode::Index:
		error = "Unexpected Index";
		break;
	case SysErrCode::Enumeration:
		error = "Unexpected Enumeration";
		break;
	case SysErrCode::Parameter:
		error = "Unexpected Parameter";
		break;
	case SysErrCode::Stream:
		error = "Unexpected Stream";
		break;
	case SysErrCode::Size:
		error = "Unexpected Size";
		break;
	case SysErrCode::NeuralNetwork:
		error = "Unexpected Neural Network";
		break;

	case SysErrCode::Ini:
		error = "Input Initialisation File (INI)";
		break;
	case SysErrCode::Xml:
		error = "Input Extensible Markup Language (XML)";
		break;
	case SysErrCode::Cmd:
		error = "Input Command Line (CMD)";
		break;
	case SysErrCode::Tga:
		error = "Input Targa Image File (TGA)";
		break;
	case SysErrCode::Bmp:
		error = "Input Windows Bitmap (BMP)";
		break;
	case SysErrCode::Pgm:
		error = "Portable Gray Map Image (PGM)";
		break;
	case SysErrCode::Cf:
		error = "Input Container File (CF)";
		break;
	case SysErrCode::Cfg:
		error = "Input Configuration File (CFG)";
		break;
	case SysErrCode::Onnx:
		error = "Open Neural Network Exchange (ONNX)";
		break;

	case SysErrCode::Printer:
		error = "Printer";
		break;
	case SysErrCode::Camera:
		error = "Camera";
		break;

	case SysErrCode::Com:
		error = "Component Object Model (COM)";
		break;
	case SysErrCode::Singleton:
		error = "Singleton Software Pattern";
		break;
	case SysErrCode::Bridge:
		error = "Bridge Software Pattern";
		break;

	default:
		error = "Unknown Error Code";
		break;
	}

	return error;
}

SystemError::SystemError(const SysErrCode &code, const char* text)
{	
	code_= code;
	message_ = text;
}

SysErrSrc SystemError::source() noexcept
{
	return SysErrSrc::System;
}

QString SystemError::detailed() noexcept
{
	QString details;
	details.reserve(1024);

	details += GetErrorSourceString(source());
	details += ": ";
	details += QString::number((uint64_t)code_);
	details += " - ";
	details += GetSystemErrorCodeString(code_);

	if(message_.length())
	{
		details += " - ";
		details += message_;
	}

	return details;
}

}
