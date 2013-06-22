/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

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

#ifndef D3DPROXYDEVICE_H_INCLUDED
#define D3DPROXYDEVICE_H_INCLUDED

#include "Direct3DDevice9.h"

#include "D3D9ProxySurface.h"
#include "D3D9ProxyTexture.h"
#include "D3D9ProxyVolumeTexture.h"
#include "D3D9ProxyCubeTexture.h"
#include "D3D9ProxyStateBlock.h"
#include "D3D9ProxySwapChain.h"
#include "D3D9ProxyVertexShader.h"

#include "Direct3DVertexBuffer9.h"
#include "Direct3DIndexBuffer9.h"
#include "Direct3DPixelShader9.h"
#include "Direct3DVertexShader9.h"
#include "Direct3DVertexDeclaration9.h"
#include "Direct3DQuery9.h"

#include "ProxyHelper.h"
#include "StereoView.h"
#include "MotionTracker.h"
#include <d3dx9.h>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <ctime>
#include "Vireio.h"
#include "StereoShaderConstant.h"
#include "StereoBackBuffer.h"
#include "GameHandler.h"
#include "ShaderRegisters.h"
#include "ViewAdjustment.h"




#define _SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; } 

class StereoView;
class D3D9ProxySwapChain;
class ShaderRegisters;
class GameHandler;

class D3DProxyDevice : public BaseDirect3DDevice9
{
public:


	D3DProxyDevice(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy);
	virtual ~D3DProxyDevice();

	friend class D3D9ProxyStateBlock;

	virtual void Init(ProxyHelper::ProxyConfig& cfg);
	
	virtual HRESULT WINAPI Reset(D3DPRESENT_PARAMETERS* pPresentationParameters);
	void SetupText();
	void HandleControls(void);
	void HandleTracking(void);


	virtual HRESULT WINAPI TestCooperativeLevel();	
	virtual HRESULT WINAPI BeginScene();
	virtual HRESULT WINAPI EndScene();
	virtual HRESULT WINAPI CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle);
	HRESULT WINAPI CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle, bool isBackBufferOfPrimarySwapChain);
	virtual HRESULT WINAPI CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader);
	virtual HRESULT WINAPI CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader);
	virtual HRESULT WINAPI CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl);
	virtual HRESULT WINAPI CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery);
	virtual HRESULT WINAPI CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB);
	virtual HRESULT WINAPI Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil);
	virtual HRESULT WINAPI ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color);
	virtual HRESULT WINAPI DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
	virtual HRESULT WINAPI DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
	virtual HRESULT WINAPI DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	virtual HRESULT WINAPI DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	virtual HRESULT WINAPI DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo);
	virtual HRESULT WINAPI DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo);
	virtual HRESULT WINAPI ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags);
	virtual HRESULT WINAPI SetIndices(IDirect3DIndexBuffer9* pIndexData);
	virtual HRESULT WINAPI GetIndices(IDirect3DIndexBuffer9** ppIndexData);
	virtual HRESULT WINAPI SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride);
	virtual HRESULT WINAPI GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride);
	virtual HRESULT WINAPI SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget);
	virtual HRESULT WINAPI GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget);
	virtual HRESULT WINAPI SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil);
	virtual HRESULT WINAPI GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface);
	virtual HRESULT WINAPI SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture);
	virtual HRESULT WINAPI GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture);	
	virtual HRESULT WINAPI SetPixelShader(IDirect3DPixelShader9* pShader);
	virtual HRESULT WINAPI GetPixelShader(IDirect3DPixelShader9** ppShader);
	virtual HRESULT WINAPI SetVertexShader(IDirect3DVertexShader9* pShader);
	virtual HRESULT WINAPI GetVertexShader(IDirect3DVertexShader9** ppShader);
	virtual HRESULT WINAPI SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	virtual HRESULT WINAPI GetVertexShaderConstantF(UINT StartRegister,float* pData, UINT Vector4fCount);
	virtual HRESULT WINAPI SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl);
	virtual HRESULT WINAPI GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl);
	virtual HRESULT WINAPI BeginStateBlock();
	virtual HRESULT WINAPI EndStateBlock(IDirect3DStateBlock9** ppSB);
	virtual HRESULT WINAPI Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
	virtual HRESULT WINAPI GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer);
	virtual HRESULT WINAPI CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain);
	virtual HRESULT WINAPI GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain);
	virtual HRESULT WINAPI GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface);
	virtual HRESULT WINAPI GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface);
	virtual HRESULT WINAPI SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap);
	virtual HRESULT WINAPI StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter);
	virtual HRESULT WINAPI UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint);
	virtual HRESULT WINAPI UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture);
	virtual HRESULT WINAPI SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
	virtual HRESULT WINAPI MultiplyTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
	virtual HRESULT WINAPI SetViewport(CONST D3DVIEWPORT9* pViewport);



	


	float* currentMatrix;

	// view translation settings
	int yaw_mode;			// 0 disabled, 1 enabled
	int pitch_mode;			// 0 disabled, 1 enabled
	//int roll_mode;			// 0 disabled, 1 enabled
	int translation_mode;	// for head translation


	bool trackingOn;

	ProxyHelper::ProxyConfig config;

	int eyeShutter;

	float aspectRatio;
	
	
	bool saveDebugFile;
	std::ofstream debugFile;
	StereoView* stereoView;
	ID3DXFont *hudFont;

	time_t lastInputTime;

	MotionTracker* tracker;
	bool trackerInitialized;
	bool *m_keys;
	int SHOCT_mode;

	// These types are largely unused with the changes to game specific config.
	// TODO Some changes are still needed before they can be removed. Data gathering should
	// probably become part of a debug/diagnostic mode.
	// TODO SOURCE_L4D is used in StereoView to change behaviour of state save/load to prevent
	// issues in hl2 (and probably other source games)
	enum ProxyTypes
	{
		MONO = 0,
		FIXED = 10,
		DATA_GATHERER = 11,
		SOURCE = 100,
		SOURCE_L4D = 101,
		UNREAL = 200,
		UNREAL_MIRROR = 201,
		UNREAL_UT3 = 202,
		UNREAL_BIOSHOCK = 203,
		EGO = 300,
		EGO_DIRT = 301,
		REALV = 400,
		REALV_ARMA = 401,
		UNITY = 500,
		UNITY_SLENDER = 501,
		ADVANCED = 600,
		ADVANCED_SKYRIM = 601
	};

	

protected:
	

	
	virtual void OnCreateOrRestore();

	// Use to specify the side that you want to draw to
	// Overriding classes should call the base implementation first and then makes any extra needed changes
	// based on the result of the base implementation (if the base class doesn't change side then derived shouldn't 
	// change either)
	virtual bool setDrawingSide(vireio::RenderPosition side);

	// Try and toggle to other drawing side. Returns false if changes fails due to the current render target being mono.
	bool switchDrawingSide();

	vireio::RenderPosition m_currentRenderingSide;

	D3DXMATRIX* m_pCurrentMatViewTransform;
	
	D3D9ProxyVertexShader* m_pActiveVertexShader;

	D3D9ProxyStateBlock* m_pCapturingStateTo;


private:


	void ReleaseEverything();
	bool isViewportDefaultForMainRT(CONST D3DVIEWPORT9* pViewport);

	HRESULT SetStereoViewTransform(D3DXMATRIX pLeftMatrix, D3DXMATRIX pRightMatrix, bool apply);
	HRESULT SetStereoProjectionTransform(D3DXMATRIX pLeftMatrix, D3DXMATRIX pRightMatrix, bool apply);

	std::shared_ptr<ShaderRegisters> m_spManagedShaderRegisters;
	GameHandler* m_pGameHandler;

	std::shared_ptr<ViewAdjustment> m_spShaderViewAdjustment;
	
	bool m_bActiveViewportIsDefault;
	D3DVIEWPORT9 m_LastViewportSet;

	bool m_isFirstBeginSceneOfFrame;
	
	D3D9ProxySurface* m_pActiveStereoDepthStencil;
	BaseDirect3DIndexBuffer9* m_pActiveIndicies;
	BaseDirect3DPixelShader9* m_pActivePixelShader;
	BaseDirect3DVertexDeclaration9* m_pActiveVertexDeclaration;

	// The swap chains have to be released and then forcibly destroyed on reset or device destruction.
	// This should be the very last thing done in both cases.
	std::vector<D3D9ProxySwapChain*> m_activeSwapChains;

	// The render targets that are currently in use.
	std::vector<D3D9ProxySurface*> m_activeRenderTargets;
	
	// Textures assigned to stages. See (Get/Set)Texture. (NULL is a valid entry in these containers. It indicates that the application
	// has specifically cleared that stream/sampler. It is important that this information is available to the proxy StateBlock)
	std::unordered_map<DWORD, IDirect3DBaseTexture9*> m_activeTextureStages;
	std::unordered_map<UINT, BaseDirect3DVertexBuffer9*> m_activeVertexBuffers;



	bool m_bInBeginEndStateBlock;

	bool m_bViewTransformSet;
	bool m_bProjectionTransformSet;


	D3DXMATRIX m_leftView;
	D3DXMATRIX m_rightView;

	D3DXMATRIX m_leftProjection;
	D3DXMATRIX m_rightProjection;

	D3DXMATRIX* m_pCurrentView;
	D3DXMATRIX* m_pCurrentProjection;


	const float m_keyRepeatRate;
	clock_t startTime;
	bool keyWait;


	// To calculate world scale build with this value set to true in the constructor.
	// Set the IPD in the user cfg to match the lens separation of the hmd.
	// In-game find something that appears correctly scaled that you can measure against.
	// Example:
	// I use NPC eye separation whenever possible (I assume they have average IPD, if they have obviously messed up features do some guesstimating)
	// Adjust so that the redline on the left half of the screen aligns with the leftmost eye (your left) of the NPC and the redline on the right 
	// half of the screen aligns with the rightmost eye (your right) of the NPC.
	// The aim is to get a rough estimate of the world scale so that the starting separation isn't to far off for users and the scale is appropriate
	// so that adjustments they make happen at a sensible (and somewhat consistent between games) rate.
	// You can use other objects if they are to scale and you can make a reasonable guess (example - the steering wheel in F1 2010 is to scale and makes
	// for a good measuring stick as it's stright infront of the camera)
	bool worldScaleCalculationMode;
	// Note: Make sure the game fov is set correctly before doing the above. When view is scaled to just fill the horizontal (normal behaviour)
	// 92.568 (use 92 if only whole numbers can be used) if horizontal, 66 if vertical (16:10 resolution), 61 if vertical (16:9 resolution)
};

#endif