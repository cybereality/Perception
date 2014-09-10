#include "StereoMode.h"
#include "StereoView.h"


static std::list<StereoMode> g_stereo_modes;

static void Add( int type , std::string name , std::string shader , StereoMode m ){
	m.type   = type;
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



std::list<StereoMode> GetStereoModes(){

	if( g_stereo_modes.empty() ){
		StereoMode dk1;
		dk1.distortionCoefficients[0] = 1.0;
		dk1.distortionCoefficients[1] = 0.22;
		dk1.distortionCoefficients[2] = 0.24;
		dk1.distortionCoefficients[3] = 0.0;
		dk1.resolutionWidth           = 1280;
		dk1.resolutionHeight          = 800;
		dk1.physicalWidth             = 0.14976;
		dk1.physicalHeight            = 0.0935;
		dk1.eyeToScreenDistance       = 0.041;
		dk1.physicalLensSeparation    = 0.064; 
		dk1.lensYCenterOffset         = 0.5;
		dk1.lensIPDCenterOffset       = 0.0;
		dk1.minDistortionScale        = -1.0;

		StereoMode dk2;
		dk1.distortionCoefficients[0] = 1.0;
		dk1.distortionCoefficients[1] = 0.15;
		dk1.distortionCoefficients[2] = 0.05;
		dk1.distortionCoefficients[3] = 0.0;
		dk1.resolutionWidth           = 1920;
		dk1.resolutionHeight          = 1080;
		dk1.physicalWidth             = 0.12577;
		dk1.physicalHeight            = 0.07074;
		dk1.eyeToScreenDistance       = 0.041;
		dk1.physicalLensSeparation    = 0.064; 
		dk1.lensYCenterOffset         = 0.5;
		dk1.lensIPDCenterOffset       = 0.0;
		dk1.minDistortionScale        = -0.5;

		StereoMode rfu;
		dk1.distortionCoefficients[0] = 1.0;
		dk1.distortionCoefficients[1] = 0.18;
		dk1.distortionCoefficients[2] = 0.115;
		dk1.distortionCoefficients[3] = 0.0;
		dk1.resolutionWidth           = 1920;
		dk1.resolutionHeight          = 1080;
		dk1.physicalWidth             = 0.1296;
		dk1.physicalHeight            = 0.0729;
		dk1.eyeToScreenDistance       = 0.041;
		dk1.physicalLensSeparation    = 0.064; 
		dk1.lensYCenterOffset         = 0.5;
		dk1.lensIPDCenterOffset       = 0.0;
		dk1.minDistortionScale        = -0.5;

		Add( StereoView::DISABLED                    , "Disabled"                     , ""                            , dk1 );
		Add( StereoView::ANAGLYPH_RED_CYAN           , "Anaglyph (Red/Cyan)"          , "AnaglyphRedCyan.fx"          , dk1 );
		Add( StereoView::ANAGLYPH_RED_CYAN_GRAY      , "Anaglyph (Red/Cyan) B+W"      , "AnaglyphRedCyanGray.fx"      , dk1 );
		Add( StereoView::ANAGLYPH_YELLOW_BLUE        , "Anaglyph (Yellow/Blue)"       , "AnaglyphYellowBlue.fx"       , dk1 );
		Add( StereoView::ANAGLYPH_YELLOW_BLUE_GRAY   , "Anaglyph (Yellow/Blue) B+W"   , "AnaglyphYellowBlueGray.fx"   , dk1 );
		Add( StereoView::ANAGLYPH_GREEN_MAGENTA      , "Anaglyph (Green/Magenta)"     , "AnaglyphGreenMagenta.fx"     , dk1 );
		Add( StereoView::ANAGLYPH_GREEN_MAGENTA_GRAY , "Anaglyph (Green/Magenta) B+W" , "AnaglyphGreenMagentaGray.fx" , dk1 );
		Add( StereoView::SIDE_BY_SIDE                , "Side by Side"                 , "SideBySide.fx"               , dk1 );
		Add( StereoView::DIY_RIFT                    , "DIY Rift"                     , "SideBySideRift.fx"           , dk1 );
		Add( StereoView::OVER_UNDER                  , "Over Under"                   , "OverUnder.fx"                , dk1 );
		Add( StereoView::INTERLEAVE_HORZ             , "Horizontal Interleave"        , "InterleaveHorz.fx"           , dk1 );
		Add( StereoView::INTERLEAVE_VERT             , "Vertical Interleave"          , "InterleaveVert.fx"           , dk1 );
		Add( StereoView::CHECKERBOARD                , "Checkerboard"                 , "Checkerboard.fx"             , dk1 );
		Add( StereoView::RIFTUP                      , "RiftUp!"                      , "OculusRift.fx"               , rfu );
		Add( StereoView::OCULUS_RIFT_DK1             , "Oculus Rift DK1"              , "OculusRift.fx"               , dk1 );
		Add( StereoView::OCULUS_RIFT_DK1_CROPPED     , "Oculus Rift DK1 Cropped"      , "OculusRiftCropped.fx"        , dk1 );
		Add( StereoView::OCULUS_RIFT_DK2             , "Oculus Rift DK2"              , "OculusRiftDK2.fx"            , dk2 );
		Add( StereoView::OCULUS_RIFT_DK2_CROPPED     , "Oculus Rift DK2 Cropped"      , "OculusRiftDK2Cropped.fx"     , dk2 );
	}

	return g_stereo_modes;
}


StereoMode* FindStereoMode( int type ){
	GetStereoModes();

	for( StereoMode& m : g_stereo_modes ){
		if( m.type == type ){
			return &m;
		}
	}

	return 0;
}