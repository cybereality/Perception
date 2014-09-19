/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <Main.cpp> :
Copyright (C) 2012 Andres Hernandez

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#include <windows.h>
#include <d3d9.h>
#include "D3D9ProxyDirect3D.h"
#include <qcoreapplication.h>
#include <qdebug.h>

#include <cConfig.h>
#include <qfileinfo.h>

QString vireioDir;
static cConfig config;
static bool    loaded = false;

BOOL APIENTRY DllMain( HINSTANCE dll , DWORD fdwReason, LPVOID ){
	if( fdwReason == DLL_PROCESS_ATTACH ){
		char exe_path[MAX_PATH];
		char dll_path[MAX_PATH];

		GetModuleFileNameA( 0   , exe_path , MAX_PATH );
		GetModuleFileNameA( dll , dll_path , MAX_PATH );

		vireioDir = QFileInfo(dll_path).absolutePath() + "/../";

		printf( "vireio: path %s\n" , vireioDir.toLocal8Bit().data() );

		cStereoMode ::loadAll();
		cGameProfile::loadAll();
		cGame       ::loadAll();

		loaded = config.load( QFileInfo(exe_path).absoluteFilePath() );

		if( loaded ){
			printf("vireio: profile \"%s\"\n" , config.profileName.toLocal8Bit().data() );

			printf( "vireio: stereoMode=%s\n" , config.stereoMode.toLocal8Bit().data() );
			printf( "vireio: trackerMode=%d\n" , config.trackerMode);
			printf( "vireio: streamingEnable=%d\n" , config.streamingEnable);
			printf( "vireio: streamingAddress=%s\n" , config.streamingAddress.toLocal8Bit().data() );
			printf( "vireio: streamingPort=%d\n" , config.streamingPort);
			printf( "vireio: streamingCodec=%s\n" , config.streamingCodec.toLocal8Bit().data() );
			printf( "vireio: streamingBitrate=%d\n" , config.streamingBitrate);
			printf( "vireio: profileName=%s\n" , config.profileName.toLocal8Bit().data() );
			printf( "vireio: exeName=%s\n" , config.exeName.toLocal8Bit().data() );
			printf( "vireio: shaderRulePath=%s\n" , config.shaderRulePath.toLocal8Bit().data() );
			printf( "vireio: VRboostPath=%s\n" , config.VRboostPath.toLocal8Bit().data() );
			printf( "vireio: VRboostMinShaderCount=%d\n" , config.VRboostMinShaderCount);
			printf( "vireio: VRboostMaxShaderCount=%d\n" , config.VRboostMaxShaderCount);
			printf( "vireio: game_type=%d\n" , config.game_type);
			printf( "vireio: rollEnabled=%d\n" , config.rollEnabled);
			printf( "vireio: worldScaleFactor=%f\n" , config.worldScaleFactor);
			printf( "vireio: convergence=%f\n" , config.convergence);
			printf( "vireio: swap_eyes=%d\n" , config.swap_eyes);
			printf( "vireio: yaw_multiplier=%f\n" , config.yaw_multiplier);
			printf( "vireio: pitch_multiplier=%f\n" , config.pitch_multiplier);
			printf( "vireio: roll_multiplier=%f\n" , config.roll_multiplier);
			printf( "vireio: position_multiplier=%f\n" , config.position_multiplier);
			printf( "vireio: DistortionScale=%f\n" , config.DistortionScale);
			printf( "vireio: YOffset=%f\n" , config.YOffset);
			printf( "vireio: IPDOffset=%f\n" , config.IPDOffset);
			printf( "vireio: hud3DDepthMode=%d\n" , config.hud3DDepthMode);
			printf( "vireio: hud3DDepthPresets=%f\n" , config.hud3DDepthPresets[0]);
			printf( "vireio: hudDistancePresets=%f\n" , config.hudDistancePresets[0]);
			printf( "vireio: hudHotkeys=%d\n" , config.hudHotkeys[0]);
			printf( "vireio: gui3DDepthMode=%d\n" , config.gui3DDepthMode);
			printf( "vireio: gui3DDepthPresets=%f\n" , config.gui3DDepthPresets[0]);
			printf( "vireio: guiSquishPresets=%f\n" , config.guiSquishPresets[0]);
			printf( "vireio: guiHotkeys=%d\n" , config.guiHotkeys[0]);
			printf( "vireio: VRBoostResetHotkey=%d\n" , config.VRBoostResetHotkey);
			printf( "vireio: WorldFOV=%f\n" , config.WorldFOV);
			printf( "vireio: PlayerFOV=%f\n" , config.PlayerFOV);
			printf( "vireio: FarPlaneFOV=%f\n" , config.FarPlaneFOV);
			printf( "vireio: CameraTranslateX=%f\n" , config.CameraTranslateX);
			printf( "vireio: CameraTranslateY=%f\n" , config.CameraTranslateY);
			printf( "vireio: CameraTranslateZ=%f\n" , config.CameraTranslateZ);
			printf( "vireio: CameraDistance=%f\n" , config.CameraDistance);
			printf( "vireio: CameraZoom=%f\n" , config.CameraZoom);
			printf( "vireio: CameraHorizonAdjustment=%f\n" , config.CameraHorizonAdjustment);
			printf( "vireio: ConstantValue1=%f\n" , config.ConstantValue1);
			printf( "vireio: ConstantValue2=%f\n" , config.ConstantValue2);
			printf( "vireio: ConstantValue3=%f\n" , config.ConstantValue3);
			printf( "vireio: name=%s\n" , config.name.toLocal8Bit().data() );
			printf( "vireio: shader=%s\n" , config.shader.toLocal8Bit().data() );
			printf( "vireio: isHmd=%d\n" , config.isHmd);
			printf( "vireio: resolutionWidth=%d\n" , config.resolutionWidth);
			printf( "vireio: resolutionHeight=%d\n" , config.resolutionHeight);
			printf( "vireio: physicalWidth=%f\n" , config.physicalWidth);
			printf( "vireio: physicalHeight=%f\n" , config.physicalHeight);
			printf( "vireio: distortionCoefficients=%f\n" , config.distortionCoefficients[0]);
			printf( "vireio: eyeToScreenDistance=%f\n" , config.eyeToScreenDistance);
			printf( "vireio: physicalLensSeparation=%f\n" , config.physicalLensSeparation);
			printf( "vireio: lensYCenterOffset=%f\n" , config.lensYCenterOffset);
			printf( "vireio: lensIPDCenterOffset=%f\n" , config.lensIPDCenterOffset);
			printf( "vireio: minDistortionScale=%f\n" , config.minDistortionScale);
			printf( "vireio: screenAspectRatio=%f\n" , config.screenAspectRatio);
			printf( "vireio: scaleToFillHorizontal=%f\n" , config.scaleToFillHorizontal);
			printf( "vireio: lensXCenterOffset=%f\n" , config.lensXCenterOffset);
			printf( "vireio: userName=%s\n" , config.userName.toLocal8Bit().data() );
			printf( "vireio: gender=%s\n" , config.gender.toLocal8Bit().data() );
			printf( "vireio: PlayerHeight=%f\n" , config.PlayerHeight);
			printf( "vireio: PlayerIPD=%f\n" , config.PlayerIPD);
			printf( "vireio: RiftVersion=%s\n" , config.RiftVersion.toLocal8Bit().data() );
		}

	}
	return TRUE;
}



extern "C" __declspec(dllexport) IDirect3D9Ex* ProxyDirect3DCreate9( IDirect3D9* base , IDirect3D9Ex* baseEx ) {
	if( !loaded ){
		return 0;
	}

	if( baseEx ){
		printf("vireio: creating Direct3D\n");
	}else{
		printf("vireio: creating Direct3DEx\n");
	}

	return new D3D9ProxyDirect3D( base , baseEx , config );
}
