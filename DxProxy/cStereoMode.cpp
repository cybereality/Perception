#include "cStereoMode.h"

static std::list<cStereoMode> g_stereo_modes;

static void Add( int uid , std::string name , std::string shader , cStereoMode m ){
	m.uid    = uid;
	m.name   = name;
	m.shader = shader;

	m.screenAspectRatio = m.resolutionWidth / (float)m.resolutionHeight;

	float physicalViewCenter = m.physicalWidth * 0.25f; 
	float physicalOffset     = physicalViewCenter - m.physicalLensSeparation * 0.5f;	
	
	// Range at this point would be -0.25 to 0.25 units. So multiply the last step by 4 to get the offset in a -1 to 1  range
	m.lensXCenterOffset = 4.0f * physicalOffset / m.physicalWidth;

	float radius       = -1 - m.lensXCenterOffset;
	float radiusSqared = radius * radius;
	float distort      = radius * (m.distortionCoefficients[0] + m.distortionCoefficients[1] * radiusSqared + m.distortionCoefficients[2] * radiusSqared * radiusSqared + m.distortionCoefficients[3] * radiusSqared * radiusSqared * radiusSqared);

	m.scaleToFillHorizontal = distort / radius;

	g_stereo_modes.push_back( m );
}



std::list<cStereoMode> GetStereoModes(){

	if( g_stereo_modes.empty() ){
		cStereoMode dk1;
		dk1.distortionCoefficients[0] = 1.0f;
		dk1.distortionCoefficients[1] = 0.22f;
		dk1.distortionCoefficients[2] = 0.24f;
		dk1.distortionCoefficients[3] = 0.0f;
		dk1.resolutionWidth           = 1280;
		dk1.resolutionHeight          = 800;
		dk1.physicalWidth             = 0.14976f;
		dk1.physicalHeight            = 0.0935f;
		dk1.eyeToScreenDistance       = 0.041f;
		dk1.physicalLensSeparation    = 0.064f;
		dk1.lensYCenterOffset         = 0.5f;
		dk1.lensIPDCenterOffset       = 0.0f;
		dk1.minDistortionScale        = -1.0f;

		cStereoMode dk2;
		dk1.distortionCoefficients[0] = 1.0f;
		dk1.distortionCoefficients[1] = 0.15f;
		dk1.distortionCoefficients[2] = 0.05f;
		dk1.distortionCoefficients[3] = 0.0f;
		dk1.resolutionWidth           = 1920;
		dk1.resolutionHeight          = 1080;
		dk1.physicalWidth             = 0.12577f;
		dk1.physicalHeight            = 0.07074f;
		dk1.eyeToScreenDistance       = 0.041f;
		dk1.physicalLensSeparation    = 0.064f;
		dk1.lensYCenterOffset         = 0.5f;
		dk1.lensIPDCenterOffset       = 0.0f;
		dk1.minDistortionScale        = -0.5f;

		cStereoMode rfu;
		dk1.distortionCoefficients[0] = 1.0f;
		dk1.distortionCoefficients[1] = 0.18f;
		dk1.distortionCoefficients[2] = 0.115f;
		dk1.distortionCoefficients[3] = 0.0f;
		dk1.resolutionWidth           = 1920;
		dk1.resolutionHeight          = 1080;
		dk1.physicalWidth             = 0.1296f;
		dk1.physicalHeight            = 0.0729f;
		dk1.eyeToScreenDistance       = 0.041f;
		dk1.physicalLensSeparation    = 0.064f;
		dk1.lensYCenterOffset         = 0.5f;
		dk1.lensIPDCenterOffset       = 0.0f;
		dk1.minDistortionScale        = -0.5f;

		Add( 0    , "Disabled"                     , ""                            , dk1 );
		Add( 1001 , "Anaglyph (Red/Cyan)"          , "AnaglyphRedCyan.fx"          , dk1 );
		Add( 1002 , "Anaglyph (Red/Cyan) B+W"      , "AnaglyphRedCyanGray.fx"      , dk1 );
		Add( 1003 , "Anaglyph (Yellow/Blue)"       , "AnaglyphYellowBlue.fx"       , dk1 );
		Add( 1004 , "Anaglyph (Yellow/Blue) B+W"   , "AnaglyphYellowBlueGray.fx"   , dk1 );
		Add( 1005 , "Anaglyph (Green/Magenta)"     , "AnaglyphGreenMagenta.fx"     , dk1 );
		Add( 1006 , "Anaglyph (Green/Magenta) B+W" , "AnaglyphGreenMagentaGray.fx" , dk1 );
		Add( 2001 , "Side by Side"                 , "SideBySide.fx"               , dk1 );
		Add( 2002 , "Over Under"                   , "OverUnder.fx"                , dk1 );
		Add( 3001 , "Horizontal Interleave"        , "InterleaveHorz.fx"           , dk1 );
		Add( 3002 , "Vertical Interleave"          , "InterleaveVert.fx"           , dk1 );
		Add( 3003 , "Checkerboard"                 , "Checkerboard.fx"             , dk1 );
		Add( 4001 , "DIY Rift"                     , "SideBySideRift.fx"           , dk1 );
		Add( 4002 , "RiftUp!"                      , "OculusRift.fx"               , rfu );
		Add( 4003 , "Oculus Rift DK1"              , "OculusRift.fx"               , dk1 );
		Add( 4004 , "Oculus Rift DK1 Cropped"      , "OculusRiftCropped.fx"        , dk1 );
		Add( 4005 , "Oculus Rift DK2"              , "OculusRiftDK2.fx"            , dk2 );
		Add( 4006 , "Oculus Rift DK2 Cropped"      , "OculusRiftDK2Cropped.fx"     , dk2 );
	}

	return g_stereo_modes;
}


cStereoMode* FindStereoMode( int uid ){
	GetStereoModes();

	for( cStereoMode& m : g_stereo_modes ){
		if( m.uid == uid ){
			return &m;
		}
	}

	return 0;
}
