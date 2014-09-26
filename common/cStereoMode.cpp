#include "cStereoMode.h"
#include "cPropsFile.h"
#include <qdir.h>


cStereoMode::cStereoMode(){
	all().append( this );
}


cStereoMode::~cStereoMode(){
	all().removeAll( this );
}

void cStereoMode::calculateValues(){
	screenAspectRatio = resolutionWidth / (float)resolutionHeight;

	float physicalViewCenter = physicalWidth * 0.25f;
	float physicalOffset     = physicalViewCenter - physicalLensSeparation * 0.5f;

	// Range at this point would be -0.25 to 0.25 units. So multiply the last step by 4 to get the offset in a -1 to 1  range
	lensXCenterOffset = 4.0f * physicalOffset / physicalWidth;

	float radius       = -1 - lensXCenterOffset;
	float radiusSqared = radius * radius;
	float distort      = radius * (distortionCoefficients[0] + distortionCoefficients[1] * radiusSqared + distortionCoefficients[2] * radiusSqared * radiusSqared + distortionCoefficients[3] * radiusSqared * radiusSqared * radiusSqared);

	scaleToFillHorizontal = distort / radius;
}


void cStereoMode::loadAll(){
	loadOculus();

	for( QFileInfo info : QDir( vireioDir+"modes" ).entryInfoList(QDir::Files) ){
		cPropsFile props;
		if( props.load( info.filePath() ) ){
			cStereoMode* mode = new cStereoMode;
			mode->name                          = info.baseName();
			mode->shader                        = props.getString( "shader"                   );
			mode->isHmd                         = props.getBool  ( "isHmd"                    );
			mode->resolutionWidth               = props.getInt   ( "resolutionWidth"          );
			mode->resolutionHeight              = props.getInt   ( "resolutionHeight"         );
			mode->physicalWidth                 = props.getFloat ( "physicalWidth"            );
			mode->physicalHeight                = props.getFloat ( "physicalHeight"           );
			mode->distortionCoefficients[0]     = props.getFloat ( "distortionCoefficients_0" );
			mode->distortionCoefficients[1]     = props.getFloat ( "distortionCoefficients_1" );
			mode->distortionCoefficients[2]     = props.getFloat ( "distortionCoefficients_2" );
			mode->distortionCoefficients[3]     = props.getFloat ( "distortionCoefficients_3" );
			mode->chromaCoefficients[0]         = props.getFloat ( "chromaCoefficients_0"     );
			mode->chromaCoefficients[1]         = props.getFloat ( "chromaCoefficients_1"     );
			mode->chromaCoefficients[2]         = props.getFloat ( "chromaCoefficients_2"     );
			mode->chromaCoefficients[3]         = props.getFloat ( "chromaCoefficients_3"     );
			mode->eyeToScreenDistance           = props.getFloat ( "eyeToScreenDistance"      );
			mode->physicalLensSeparation        = props.getFloat ( "physicalLensSeparation"   );
			mode->lensYCenterOffset             = props.getFloat ( "lensYCenterOffset"        );
			mode->lensIPDCenterOffset           = props.getFloat ( "lensIPDCenterOffset"      );
			mode->minDistortionScale            = props.getFloat ( "minDistortionScale"       );
			mode->chromaticAberrationCorrection = props.getBool  ( "chromaticAberrationCorrection" , true );

			mode->calculateValues();
		}
	}

	printf( "vireio: loaded %d stereo modes\n" , all().count() );
}


QList<cStereoMode*>& cStereoMode::all( ){
	static QList<cStereoMode*> list;
	return list;
}


cStereoMode* cStereoMode::find( QString name ){
	for( cStereoMode* m : all() ){
		if( m->name == name ){
			return m;
		}
	}
	return 0;
}




