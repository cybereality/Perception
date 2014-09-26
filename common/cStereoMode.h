#pragma once
#include "VireIO.h"

class cStereoMode {
public:
	QString      name;
	QString      shader;
	bool         isHmd;
	unsigned int resolutionWidth;
	unsigned int resolutionHeight;
	float        physicalWidth;
	float        physicalHeight;
	float        distortionCoefficients[4];
	float        chromaCoefficients[4];
	float        eyeToScreenDistance;
	float        physicalLensSeparation;
	float        lensYCenterOffset;
	float        lensIPDCenterOffset;
	float        minDistortionScale;
	bool         chromaticAberrationCorrection;
	
	//calculated values
	float        screenAspectRatio;
	float        scaleToFillHorizontal;
	float        lensXCenterOffset;

	cStereoMode();
	~cStereoMode();

	static void                 loadAll( );
	static QList<cStereoMode*>& all    ( );
	static cStereoMode*         find   ( QString name );

private:
	static void loadOculus();
	void calculateValues();
};
