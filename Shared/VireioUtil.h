#ifndef VIREIO_UTIL_H_INCLUDED
#define VIREIO_UTIL_H_INCLUDED

#include <string>

namespace vireio
{
	std::string retprintf(const char *fmt, ...);
	void debugf(const char *fmt, ...);
}

#endif