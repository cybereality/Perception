/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <StereoView.h> and
Class <StereoView> :
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

#ifndef STEREOVIEW_H_INCLUDED
#define STEREOVIEW_H_INCLUDED

#include "ProxyHelper.h"
#include "D3DProxyDevice.h"
#include <d3d9.h>
#include <d3dx9.h>
#include <map>
#include <string.h>
#include <assert.h>
#include "cStereoMode.h"


class Streamer;


/**
* Stereo-view render class.
* Basic class to render in stereo.
*/
class StereoView
{
public:
	StereoView(ProxyHelper::ProxyConfig& config , cStereoMode *hmd );
	virtual ~StereoView();

	/*** StereoView public methods ***/
	virtual void Init(IDirect3DDevice9* pActualDevice);
	virtual void ReleaseEverything();
	virtual void Draw(D3D9ProxySurface* stereoCapableSurface);
	virtual void SaveScreen();
	virtual void PostReset();


	/**
	* Left and right enumeration.
	***/
	static enum Eyes
	{
		LEFT_EYE,
		RIGHT_EYE
	};
	/**
	* Current Direct3D Viewport.
	***/
	D3DVIEWPORT9 viewport;
	/**
	* Option to swap the eye output for different 3D systems.
	***/	
	bool swapEyes;
	/**
	* True if class is initialized. Needed since initialization is not done in constructor.
	***/
	bool initialized;
	/**
	* Type of the game or engine as configured in cfg file.
	***/
	int game_type;
	/**
	* Should be used by OculusRiftView and D3DProxyDevice.
	***/
	float DistortionScale;	
	/**
	* Offset the screen vertically
	***/
	float YOffset;	
	/**
	* Floaty Screen Y Offset
	***/
	float HeadYOffset;	
	/**
	* Offset the screen horizontally
	***/
	float XOffset;	
	/**
	* Offset the IPD
	***/
	float IPDOffset;	


	/**
	* Lens center position, Oculus Rift vertex shader constant.
	***/
	float LensCenter[2];
	/**
	* XOffset
	***/
	float ViewportXOffset;
	float ViewportYOffset;

	/**
	* Scales image, Oculus Rift vertex shader constant.
	***/
	float Scale[2];
	/**
	* Maps texture coordinates, Oculus Rift vertex shader constant.
	* ScaleIn maps texture coordinates to Scales to ([-1, 1]), although top/bottom will be larger 
	* due to aspect ratio.
	***/
	float ScaleIn[2];
	/**
	* Predefined Oculus Rift Head Mounted Display info.
	* Contains distortionCoefficients, needed as vertex shader constants
	***/
	cStereoMode *hmdInfo;

	/**
	* The streamer.
	* @see Streamer
	**/
	Streamer* m_pStreamer;


protected:
	/*** StereoView protected methods ***/
	virtual void InitTextureBuffers();
	virtual void InitVertexBuffers();
	virtual void InitShaderEffects();
	virtual void SetViewEffectInitialValues(); 
	virtual void CalculateShaderVariables();
	virtual void SaveState();
	virtual void SetState();
	virtual void RestoreState();
	virtual void SaveAllRenderStates(LPDIRECT3DDEVICE9 pDevice);
	virtual void SetAllRenderStatesDefault(LPDIRECT3DDEVICE9 pDevice);
	virtual void RestoreAllRenderStates(LPDIRECT3DDEVICE9 pDevice);

	/**
	* The actual, unwrapped Direct3D Device. 
	* Class cannot be initialized with wrapped device.
	***/
	IDirect3DDevice9* m_pActualDevice;
	/**
	* Saved game vertex shader to be restored after drawing stereoscopic.
	***/
	IDirect3DVertexShader9* lastVertexShader;
	/**
	* Saved game pixel shader to be restored after drawing stereoscopic.
	***/
	IDirect3DPixelShader9* lastPixelShader;
	/**
	* Saved game texture 0 to be restored after drawing stereoscopic.
	***/
	IDirect3DBaseTexture9* lastTexture;
	/**
	* Saved game texture 1 to be restored after drawing stereoscopic.
	***/
	IDirect3DBaseTexture9* lastTexture1;
	/**
	* Saved game vertex declaration to be restored after drawing stereoscopic.
	***/
	IDirect3DVertexDeclaration9* lastVertexDeclaration;
	/**
	* Saved game render target 0 to be restored after drawing stereoscopic.
	***/
	IDirect3DSurface9* lastRenderTarget0;
	/**
	* Saved game render target 1 to be restored after drawing stereoscopic.
	***/
	IDirect3DSurface9* lastRenderTarget1;
	/**
	* Left eye (or upper) target texture buffer.
	* Surface data from D3D9ProxySurface is copied on that. To be swapped with right texture if swap_eyes set to true.
	***/
	IDirect3DTexture9* leftTexture;
	/**
	* Right eye (or lower) target texture buffer.
	* Surface data from D3D9ProxySurface is copied on that. To be swapped with left texture if swap_eyes set to true.
	***/
	IDirect3DTexture9* rightTexture;
	/**
	* Current render target.
	***/
	IDirect3DSurface9* backBuffer;
	/**
	* Surface of the left eye texture.
	***/
	IDirect3DSurface9* leftSurface;
	/**
	* Surface of the right eye texture.
	***/
	IDirect3DSurface9* rightSurface;
	/**
	* Full screen render vertex buffer containing 4 vertices.
	***/
	IDirect3DVertexBuffer9* screenVertexBuffer;
	/**
	* Render state block.
	* Stores all render states to be restored after drawing stereoscopic.
	* To be renamed.
	***/
	IDirect3DStateBlock9* sb;
	/**
	* Stores render states.
	* For games (Half Life 2?) that do not work with direct 3d state block for some reason.
	***/
	DWORD renderStates[256];
	/**
	* View effect according to the stereo mode preset in stereo_mode.
	***/
	ID3DXEffect* viewEffect;

	DWORD tssColorOp;            /**< Various states. */
	DWORD tssColorArg1;          /**< Various states. */
	DWORD tssAlphaOp;            /**< Various states. */
	DWORD tssAlphaArg1;          /**< Various states. */
	DWORD tssConstant;           /**< Various states. */
	DWORD rsAlphaEnable;         /**< Various states. */
	DWORD rsZEnable;             /**< Various states. */
	DWORD rsZWriteEnable;        /**< Various states. */
	DWORD rsDepthBias;           /**< Various states. */
	DWORD rsSlopeScaleDepthBias; /**< Various states. */
	DWORD rsSrgbEnable;          /**< Various states. */
	DWORD ssSrgb;                /**< Various states. */  
	DWORD ssSrgb1;               /**< Various states. */
	DWORD ssAddressU;            /**< Various states. */
	DWORD ssAddressV;            /**< Various states. */
	DWORD ssAddressW;            /**< Various states. */
	DWORD ssMag0;                /**< Various states. */
	DWORD ssMag1;                /**< Various states. */
	DWORD ssMin0;                /**< Various states. */
	DWORD ssMin1;                /**< Various states. */
	DWORD ssMip0;                /**< Various states. */
	DWORD ssMip1;                /**< Various states. */
	/**
	* Determines how to save render states for stereo view output.
	***/
	enum HowToSaveRenderStates
	{
		STATE_BLOCK,
		SELECTED_STATES_MANUALLY,
		ALL_STATES_MANUALLY,
		DO_NOT_SAVE_AND_RESTORE,
	} howToSaveRenderStates;
};

/**
* Declaration of texture vertex used for full screen render.
***/
const DWORD D3DFVF_TEXVERTEX = D3DFVF_XYZRHW | D3DFVF_TEX1;
/**
* Texture vertex used for full screen render.
***/
struct TEXVERTEX
{
	float x;
	float y;
	float z;
	float rhw;
	float u;
	float v;
};

#endif
