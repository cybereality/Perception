#ifndef VIREIO_UTIL_H_INCLUDED
#define VIREIO_UTIL_H_INCLUDED

#include <string>

namespace vireio
{
	/**
	* Simple left, right enumeration.
	***/
	enum RenderPosition
	{
		// probably need an 'Original' here
		Left = 1,
		Right = 2		
	};
	
	std::string retprintf(const char *fmt, ...);
	void debugf(const char *fmt, ...);
}

#endif