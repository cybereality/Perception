#include <windows.h>
#include "VireioUtil.h"
#include <stdarg.h>

namespace vireio
{
	std::string retprintf(const char *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		char buf[8192];
		vsnprintf_s(buf, 8192, fmt, args);
		va_end(args);
		return std::string(buf);
	}
	
	void debugf(const char *fmt, ...)
	{
		va_list args;
		va_start(args, fmt);
		char buf[8192];
		vsnprintf_s(buf, 8192, fmt, args);
		va_end(args);
		OutputDebugStringA(buf);
	}
}