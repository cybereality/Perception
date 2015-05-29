#ifndef VIREIO_UTIL_H_INCLUDED
#define VIREIO_UTIL_H_INCLUDED

#include <string>

#define PI 3.141592654
#define RADIANS_TO_DEGREES(rad) ((float)(rad) * (float) (180.0 / PI))
#define DEGREES_TO_RADIANS(deg) ((float)(deg) * (float) (PI / 180.0))

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
	
	float AngleDifferenceDeg(float a, float b);
	float AngleDifferenceRad(float a, float b);
}

#endif