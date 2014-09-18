#pragma once
#include <string>
#include <list>

struct cStereoMode {
	int          uid;
	std::string  name;
	std::string  shader;
	unsigned int resolutionWidth;
	unsigned int resolutionHeight;
	float        physicalWidth;
	float        physicalHeight;
	float        distortionCoefficients[4];
	float        eyeToScreenDistance;
	float        physicalLensSeparation;
	float        lensYCenterOffset;
	float        lensIPDCenterOffset;
	float        minDistortionScale;

	//calculated values
	float        screenAspectRatio;
	float        scaleToFillHorizontal;
	float        lensXCenterOffset;
};

std::list<cStereoMode> GetStereoModes();
cStereoMode*           FindStereoMode( int uid );
