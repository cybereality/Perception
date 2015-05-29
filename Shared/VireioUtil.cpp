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
		OutputDebugString(buf);
	}
	
	/// Returns the difference between two angles in [-180,180)
	float AngleDifferenceDeg(float a, float b)
	{
		float result = a-b;
		while(result<-180) result += 360;
		while(result>=180) result -= 360;
		return result;
	}
	
	/// Returns the difference between two angles in [-pi,pi)
	float AngleDifferenceRad(float a, float b)
	{
		float result = a-b;
		while(result<-PI) result += (float)(2*PI);
		while(result>=PI) result -= (float)(2*PI);
		return result;
	}
}