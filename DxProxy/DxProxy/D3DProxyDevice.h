/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3DProxyDevice.h> and
Class <D3DProxyDevice> :
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain

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

#ifndef D3DPROXYDEVICE_H_INCLUDED
#define D3DPROXYDEVICE_H_INCLUDED

#define MAX_VRBOOST_VALUES 256
#define BRASSA_PIXEL_WIDTH 1920
#define BRASSA_PIXEL_HEIGHT 1080

#include "Direct3DDevice9.h"

#include "D3D9ProxySurface.h"
#include "D3D9ProxyTexture.h"
#include "D3D9ProxyVolumeTexture.h"
#include "D3D9ProxyCubeTexture.h"
#include "D3D9ProxyStateBlock.h"
#include "D3D9ProxySwapChain.h"
#include "D3D9ProxyVertexShader.h"
#include "D3D9ProxyPixelShader.h"

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
#include <XInput.h>
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
#include "InputControls.h"
#include "VRBoostEnums.h"
#include "DirectInput.h"

#define _SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; } 
// Define SHOW_CALLS to have each method output a debug string when it is invoked
//#define SHOW_CALLS
class StereoView;
class D3D9ProxySwapChain;
class ShaderRegisters;
class GameHandler;

/**
* Direct 3D proxy device class. 
* Basically this class hosts all proxy wrappers accordingly to the methods called by the game.
*/
class D3DProxyDevice : public BaseDirect3DDevice9
{
public:
	D3DProxyDevice(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy);
	virtual ~D3DProxyDevice();

	friend class D3D9ProxyStateBlock;

	/*** IUnknown methods ***/
	virtual HRESULT WINAPI QueryInterface(REFIID riid, LPVOID* ppv);

	/*** IDirect3DDevice9 methods ***/
	virtual HRESULT WINAPI TestCooperativeLevel();
	virtual HRESULT WINAPI SetCursorProperties(UINT XHotSpot,UINT YHotSpot,IDirect3DSurface9* pCursorBitmap);
	virtual HRESULT WINAPI CreateAdditionalSwapChain(D3DPRESENT_PARAMETERS* pPresentationParameters,IDirect3DSwapChain9** pSwapChain);
	virtual HRESULT WINAPI GetSwapChain(UINT iSwapChain,IDirect3DSwapChain9** pSwapChain);
	virtual HRESULT WINAPI Reset(D3DPRESENT_PARAMETERS* pPresentationParameters);
	virtual HRESULT WINAPI Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
	virtual HRESULT WINAPI GetBackBuffer(UINT iSwapChain,UINT iBackBuffer,D3DBACKBUFFER_TYPE Type,IDirect3DSurface9** ppBackBuffer);
	virtual HRESULT WINAPI CreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateVolumeTexture(UINT Width,UINT Height,UINT Depth,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DVolumeTexture9** ppVolumeTexture,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateCubeTexture(UINT EdgeLength,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DCubeTexture9** ppCubeTexture,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateVertexBuffer(UINT Length,DWORD Usage,DWORD FVF,D3DPOOL Pool,IDirect3DVertexBuffer9** ppVertexBuffer,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateIndexBuffer(UINT Length,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DIndexBuffer9** ppIndexBuffer,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateRenderTarget(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI CreateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI UpdateSurface(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestinationSurface,CONST POINT* pDestPoint);
	virtual HRESULT WINAPI UpdateTexture(IDirect3DBaseTexture9* pSourceTexture,IDirect3DBaseTexture9* pDestinationTexture);
	virtual HRESULT WINAPI GetRenderTargetData(IDirect3DSurface9* pRenderTarget,IDirect3DSurface9* pDestSurface);
	virtual HRESULT WINAPI GetFrontBufferData(UINT iSwapChain,IDirect3DSurface9* pDestSurface);
	virtual HRESULT WINAPI StretchRect(IDirect3DSurface9* pSourceSurface,CONST RECT* pSourceRect,IDirect3DSurface9* pDestSurface,CONST RECT* pDestRect,D3DTEXTUREFILTERTYPE Filter);
	virtual HRESULT WINAPI ColorFill(IDirect3DSurface9* pSurface,CONST RECT* pRect,D3DCOLOR color);
	virtual HRESULT WINAPI CreateOffscreenPlainSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DPOOL Pool,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle);
	virtual HRESULT WINAPI SetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9* pRenderTarget);
	virtual HRESULT WINAPI GetRenderTarget(DWORD RenderTargetIndex,IDirect3DSurface9** ppRenderTarget);
	virtual HRESULT WINAPI SetDepthStencilSurface(IDirect3DSurface9* pNewZStencil);
	virtual HRESULT WINAPI GetDepthStencilSurface(IDirect3DSurface9** ppZStencilSurface);
	virtual HRESULT WINAPI BeginScene();
	virtual HRESULT WINAPI EndScene();
	virtual HRESULT WINAPI Clear(DWORD Count,CONST D3DRECT* pRects,DWORD Flags,D3DCOLOR Color,float Z,DWORD Stencil);
	virtual HRESULT WINAPI SetTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
	virtual HRESULT WINAPI MultiplyTransform(D3DTRANSFORMSTATETYPE State,CONST D3DMATRIX* pMatrix);
	virtual HRESULT WINAPI SetViewport(CONST D3DVIEWPORT9* pViewport);
	virtual HRESULT WINAPI CreateStateBlock(D3DSTATEBLOCKTYPE Type,IDirect3DStateBlock9** ppSB);
	virtual HRESULT WINAPI BeginStateBlock();
	virtual HRESULT WINAPI EndStateBlock(IDirect3DStateBlock9** ppSB);
	virtual HRESULT WINAPI GetTexture(DWORD Stage,IDirect3DBaseTexture9** ppTexture);	
	virtual HRESULT WINAPI SetTexture(DWORD Stage,IDirect3DBaseTexture9* pTexture);
	virtual HRESULT WINAPI DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
	virtual HRESULT WINAPI DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
	virtual HRESULT WINAPI DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	virtual HRESULT WINAPI DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	virtual HRESULT WINAPI ProcessVertices(UINT SrcStartIndex,UINT DestIndex,UINT VertexCount,IDirect3DVertexBuffer9* pDestBuffer,IDirect3DVertexDeclaration9* pVertexDecl,DWORD Flags);
	virtual HRESULT WINAPI CreateVertexDeclaration(CONST D3DVERTEXELEMENT9* pVertexElements,IDirect3DVertexDeclaration9** ppDecl);
	virtual HRESULT WINAPI SetVertexDeclaration(IDirect3DVertexDeclaration9* pDecl);
	virtual HRESULT WINAPI GetVertexDeclaration(IDirect3DVertexDeclaration9** ppDecl);
	virtual HRESULT WINAPI CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader);
	virtual HRESULT WINAPI SetVertexShader(IDirect3DVertexShader9* pShader);
	virtual HRESULT WINAPI GetVertexShader(IDirect3DVertexShader9** ppShader);
	virtual HRESULT WINAPI SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	virtual HRESULT WINAPI GetVertexShaderConstantF(UINT StartRegister,float* pData, UINT Vector4fCount);
	virtual HRESULT WINAPI SetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9* pStreamData,UINT OffsetInBytes,UINT Stride);
	virtual HRESULT WINAPI GetStreamSource(UINT StreamNumber,IDirect3DVertexBuffer9** ppStreamData,UINT* pOffsetInBytes,UINT* pStride);
	virtual HRESULT WINAPI SetIndices(IDirect3DIndexBuffer9* pIndexData);
	virtual HRESULT WINAPI GetIndices(IDirect3DIndexBuffer9** ppIndexData);
	virtual HRESULT WINAPI CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader);
	virtual HRESULT WINAPI SetPixelShader(IDirect3DPixelShader9* pShader);
	virtual HRESULT WINAPI GetPixelShader(IDirect3DPixelShader9** ppShader);
	virtual HRESULT WINAPI SetPixelShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	virtual HRESULT WINAPI GetPixelShaderConstantF(UINT StartRegister,float* pData, UINT Vector4fCount);
	virtual HRESULT WINAPI DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo);
	virtual HRESULT WINAPI DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo);
	virtual HRESULT WINAPI CreateQuery(D3DQUERYTYPE Type,IDirect3DQuery9** ppQuery);

	/*** D3DProxyDevice public methods ***/
	HRESULT WINAPI CreateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle, bool isBackBufferOfPrimarySwapChain);
	virtual void   Init(ProxyHelper::ProxyConfig& cfg);
	void           SetupHUD();
	virtual void   HandleControls(void);
	void           HandleTracking(void);
	void           HandleUpdateExtern();

	/**
	* Game Types.
	* We use these ProxyTypes to determine either to draw BRASSA in EndScene() or in Present().
	* Will be also used for any game- or engine-specific things.
	***/
	enum ProxyTypes
	{
		MONO = 0,                  /**<  !! */
		FIXED = 10,                /**< Default driver behavior. */
		SOURCE = 100,              /**< Source is a 3D video game engine developed by Valve Corporation. */
		SOURCE_L4D = 101,          /**<  !! */
		UNREAL = 200,              /**< The Unreal Engine is a game engine developed by Epic Games, first illustrated in the 1998 first-person shooter game Unreal. */
		UNREAL_MIRROR = 201,       /**<  !! */
		UNREAL_UT3 = 202,          /**<  !! */
		UNREAL_BIOSHOCK = 203,     /**<  !! */
		UNREAL_BORDERLANDS = 204,  /**< Borderlands(TM) */
		EGO = 300,                 /**< Ego Game Technology Engine (more commonly referred to as Ego Engine or EGO, stylised ego) is a video game engine developed by Codemasters. */
		EGO_DIRT = 301,            /**<  !! */
		REALV = 400,               /**< Real Virtuality is a proprietary computer game engine developed by Bohemia Interactive (BI), originally called Poseidon. */
		REALV_ARMA = 401,          /**<  !! */
		UNITY = 500,               /**< Unity is a cross-platform game engine with a built-in IDE developed by Unity Technologies. */
		UNITY_SLENDER = 501,       /**<  !! */
		GAMEBRYO = 600,            /**< Gamebryo 3D and LightSpeed engines are owned by Gamebase Co., Ltd. and Gamebase USA and have been used by several video game developers. */
		GAMEBRYO_SKYRIM = 601,     /**< Skyrim’s Creation Engine still has at least some Gamebryo in it. */
		LFS = 700,                 /**< Live for Speed (LFS) is a racing simulator developed by a three person team comprising Scawen Roberts, Eric Bailey, and Victor van Vlaardingen. */
		CDC = 800,                 /**< Proprietary game engine developed by Crystal Dynamics. */
		DEBUG_LOG_FILE = 99999     /**< Debug log file output game type. For development causes. Do not use since slows down game extremely. */
	};
	/**
	* Mode of the BRASSA menu.
	*
	***/
	enum BRASSA_Modes
	{
		INACTIVE = 0,
		MAINMENU = 1,
		WORLD_SCALE_CALIBRATION,
		CONVERGENCE_ADJUSTMENT,
		SHADER_ANALYZER,
		HUD_CALIBRATION,
		GUI_CALIBRATION,
		OVERALL_SETTINGS,
		VRBOOST_VALUES,
		BRASSA_SHADER_ANALYZER_SUBMENU,
		CHANGE_RULES_SCREEN,
		PICK_RULES_SCREEN,
		SHOW_SHADERS_SCREEN,
		BRASSA_ENUM_RANGE
	};
	/**
	* HUD scale enumeration.
	* ENUM_RANGE = range of the enum
	***/
	enum HUD_3D_Depth_Modes
	{
		HUD_DEFAULT = 0,
		HUD_SMALL = 1,
		HUD_LARGE = 2,
		HUD_FULL = 3,
		HUD_ENUM_RANGE = 4
	};
	/**
	* GUI scale enumeration.
	* ENUM_RANGE = range of the enum
	***/
	enum GUI_3D_Depth_Modes
	{
		GUI_DEFAULT = 0,
		GUI_SMALL = 1,
		GUI_LARGE = 2,
		GUI_FULL = 3,
		GUI_ENUM_RANGE = 4
	};

	/**
	* Game-specific proxy configuration.
	**/
	ProxyHelper::ProxyConfig config;
	/**
	* VRBoost values. 
	* Set to public for future use in input device classes.
	***/
	float VRBoostValue[MAX_VRBOOST_VALUES];
	/**
	* Currently not used.
	***/
	float* currentMatrix;
	/**
	* View translation settings (yaw - 0 disabled, 1 enabled).
	**/
	int yaw_mode;			
	/**
	* View translation settings (pitch - 0 disabled, 1 enabled).
	**/
	int pitch_mode;			
	/**
	* Currently not used (For head translation).
	**/
	int translation_mode;	
	/**
	* True if head tracking is enabled.
	**/
	bool trackingOn;
	/**
	* Currently not used, eye shutter side from old code.
	**/
	int eyeShutter;
	/**
	* Currently not used aspect ratio.
	**/
	float aspectRatio;	
	/**
	* The chosen stereo renderer.
	* @see StereoView
	**/
	StereoView* stereoView;
	/**
	* The chosen motion tracker.
	* @see MotionTracker
	**/
	std::unique_ptr<MotionTracker> tracker;
	/**
	* Schneider-Hicks Optical Calibration Tool GUI mode.
	**/
	BRASSA_Modes BRASSA_mode;
	/**
	* Schneider-Hicks Optical Calibration Tool center of right line.
	**/
	float centerlineR;
	/**
	* Schneider-Hicks Optical Calibration Tool center of left line.
	**/
	float centerlineL;
	/**
	* HUD font to be used for SHOCT.
	**/
	ID3DXFont *hudFont;
	/**
	* Timestamp used to adjust the menu velocity independent of game speed.
	**/
	float menuTime;
	/**
	* Timespan of every frame (in seconds).
	***/
	float menuSeconds;
	/**
	* True if head tracking initialized.
	**/
	bool trackerInitialized;
	InputControls controls;
	DirectInput dinput;

protected:
	/*** D3DProxyDevice protected methods ***/
	virtual void OnCreateOrRestore();	
	virtual bool setDrawingSide(vireio::RenderPosition side);
	bool         switchDrawingSide();
	bool         addRule(std::string constantName, bool allowPartialNameMatch, UINT startRegIndex, D3DXPARAMETER_CLASS constantType, UINT operationToApply, bool transpose);
	bool         modifyRule(std::string constantName, UINT operationToApply, bool transpose);
	bool         deleteRule(std::string constantName);
	void         saveShaderRules();
	void         ClearRect(vireio::RenderPosition renderPosition, D3DRECT rect, D3DCOLOR color);
	void         ClearEmptyRect(vireio::RenderPosition renderPosition, D3DRECT rect, D3DCOLOR color, int bw);
	void         DrawSelection(vireio::RenderPosition renderPosition, D3DRECT rect, D3DCOLOR color, int selectionIndex, int selectionRange);
	void         DrawScrollbar(vireio::RenderPosition renderPosition, D3DRECT rect, D3DCOLOR color, float scroll, int scrollbarSize);
	void         DrawTextShadowed(ID3DXFont* font, LPD3DXSPRITE sprite, LPCSTR lpchText, int cchText, LPRECT lprc, UINT format, D3DCOLOR color);
	void         ChangeHUD3DDepthMode(HUD_3D_Depth_Modes newMode);
	void         ChangeGUI3DDepthMode(GUI_3D_Depth_Modes newMode);
	void         BRASSA_NewFrame(UINT &entryID, UINT menuEntryCount);
	virtual void BRASSA_ShaderSubMenu(){}
	virtual void BRASSA_ChangeRules(){}
	virtual void BRASSA_PickRules(){}
	virtual void BRASSA_ShowActiveShaders(){}

	/**
	* The game handler.
	* @see GameHandler
	**/
	GameHandler* m_pGameHandler;
	/**
	* Current drawing side, only changed in setDrawingSide().
	**/
	vireio::RenderPosition m_currentRenderingSide;
	/**
	* Currently not used WorldViewTransform matrix.
	**/
	D3DXMATRIX* m_pCurrentMatViewTransform;
	/**
	* Active stored proxy pixel shader.
	**/
	D3D9ProxyPixelShader* m_pActivePixelShader;
	/**
	* Active stored proxy vertex shader.
	**/
	D3D9ProxyVertexShader* m_pActiveVertexShader;
	/**
	* Proxy state block to capture various states.
	**/
	D3D9ProxyStateBlock* m_pCapturingStateTo;
	/**
	* Main menu sprite.
	***/
	LPD3DXSPRITE hudMainMenu;
	/**
	* Main menu sprite.
	***/
	LPD3DXSPRITE hudTextBox;
	/**
	* Main menu velocity.
	***/
	D3DXVECTOR2 menuVelocity;
	/**
	* Main menu affection.
	***/
	D3DXVECTOR2 menuAttraction;
	/**
	* Main menu border top height.
	***/
	float borderTopHeight;
	/**
	* Main menu top height for scrolling menues.
	***/
	float menuTopHeight;
	/**
	* BRASSA menu value.
	***/
	int viewportWidth;
	/**
	* BRASSA menu value.
	***/
	int viewportHeight;
	/**
	* BRASSA menu value.
	***/
	float menuTop;
	/**
	* BRASSA menu value.
	* Menu entry height, in pixels.
	***/
	float menuEntryHeight;
	/**
	* BRASSA menu helper rectangle.
	***/
	RECT menuHelperRect;
	/**
	* BRASSA menu value.
	* Scales BRASSA menu to current resolution.
	***/
	float fScaleX;
	/**
	* BRASSA menu value.
	* Scales BRASSA menu to current resolution.
	***/
	float fScaleY;
	/**
	* True if BeginScene() is called the first time this frame.
	* @see BeginScene()
	**/
	bool m_isFirstBeginSceneOfFrame;
	/**
	* True if mouse emulation head tracking forced.
	**/
	bool m_bForceMouseEmulation;
	/**
	* True if VRBoost is on.
	**/
	bool m_bVRBoostToggle;
	/**
	* Timespan the VRBoost indicator should be drawn.
	**/
	float m_fVRBoostIndicator;
	/**
 	* Maximum Distortion Scale the Quicklinks will zoom to
 	**/
 	float m_maxDistortionScale;
	/**
	* Counts the current vertex shader set calls.
	* Used for VRboost security.
	***/
	UINT m_VertexShaderCount;
	/**
	* Counts the current vertex shader set calls (last frame).
	* Used for VRboost security.
	***/
	UINT m_VertexShaderCountLastFrame;
	/**
	* Vector contains all possible game projection x scale values.
	* Filled only if BRASSA_mode == WorldScale and SetTransform(>projection<) called by the game.
	***/
	std::vector<float> m_gameXScaleUnits;
	/**
	* Struct commands device behavior.
	***/
	struct DeviceBehavior
	{
		/**
		* Determines when to render the brassa menu for that game profile.
		***/
		enum WhenToDo
		{
			PRESENT,
			BEGIN_SCENE,
			FIRST_BEGIN_SCENE,
			END_SCENE,
		};

		/**
		* Determines when to render the brassa menu for that game profile.
		***/
		WhenToDo whenToRenderBRASSA;
		/**
		* Determines when to handle head tracking for that game profile.
		***/
		WhenToDo whenToHandleHeadTracking;

	} m_deviceBehavior;

private:
	/*** D3DProxyDevice private methods ***/
	void    BRASSA();
	void    BRASSA_MainMenu();
	void    BRASSA_WorldScale();
	void    BRASSA_Convergence();
	void    BRASSA_HUD();
	void    BRASSA_GUI();
	void    BRASSA_Settings();
	void    BRASSA_VRBoostValues();
	void    BRASSA_UpdateBorder();
	void    BRASSA_UpdateConfigSettings();
	void    BRASSA_UpdateDeviceSettings();
	void    BRASSA_AdditionalOutput();
	void    ReleaseEverything();
	bool    isViewportDefaultForMainRT(CONST D3DVIEWPORT9* pViewport);
	HRESULT SetStereoViewTransform(D3DXMATRIX pLeftMatrix, D3DXMATRIX pRightMatrix, bool apply);
	HRESULT SetStereoProjectionTransform(D3DXMATRIX pLeftMatrix, D3DXMATRIX pRightMatrix, bool apply);
	void    SetGUIViewport();
	float   RoundBrassaValue(float val);
	bool	InitBrassa();
	bool	InitVRBoost();
	bool	InitTracker();

	/*** VRboost function pointer typedefs ***/
	typedef HRESULT (WINAPI *LPVRBOOST_LoadMemoryRules)(std::string processName, std::string rulesPath);
	typedef HRESULT (WINAPI *LPVRBOOST_SaveMemoryRules)(std::string rulesPath);
	typedef HRESULT (WINAPI *LPVRBOOST_CreateFloatMemoryRule)(DWORD ruleType, UINT axisIndex, D3DXVECTOR4 constantVector, DWORD pointerAddress, DWORD* offsets, DWORD minValue, DWORD maxValue, DWORD comparisationPointer1, DWORD* comparisationOffsets1, int pointerDifference1, DWORD comparisationPointer2, DWORD* comparisationOffsets2, int pointerDifference2);
	typedef HRESULT (WINAPI *LPVRBOOST_SetProcess)(std::string processName, std::string moduleName);
	typedef HRESULT (WINAPI *LPVRBOOST_ReleaseAllMemoryRules)( void );
	typedef HRESULT (WINAPI *LPVRBOOST_ApplyMemoryRules)(UINT axisNumber, float** axis);

	/**
	* VRboost pointer function to load memory rules for a process.
	***/
	LPVRBOOST_LoadMemoryRules m_pVRboost_LoadMemoryRules;
	/**
	* VRboost pointer function to load memory rules for a process.
	***/
	LPVRBOOST_SaveMemoryRules m_pVRboost_SaveMemoryRules;
	/**
	* VRboost pointer function to create a float memory rule.
	***/
	LPVRBOOST_CreateFloatMemoryRule m_pVRboost_CreateFloatMemoryRule;
	/**
	* VRboost pointer function to load memory rules for a process.
	***/
	LPVRBOOST_SetProcess m_pVRboost_SetProcess;
	/**
	* VRboost pointer function to release all memory rules.
	***/
	LPVRBOOST_ReleaseAllMemoryRules m_pVRboost_ReleaseAllMemoryRules;
	/**
	* VRboost pointer function to apply memory rules to process memory.
	***/
	LPVRBOOST_ApplyMemoryRules m_pVRboost_ApplyMemoryRules;
	/**
	* Handle to VRboost library.
	***/
	HMODULE hmVRboost;
	/**
	* Managed shader register class.
	* @see ShaderRegisters
	**/
	std::shared_ptr<ShaderRegisters> m_spManagedShaderRegisters;
	/**
	* View matrix adjustment class.
	* @see ViewAdjustment
	**/
	std::shared_ptr<ViewAdjustment> m_spShaderViewAdjustment;
	/**
	* True if active viewport is the default one.
	* @see isViewportDefaultForMainRT()
	**/
	bool m_bActiveViewportIsDefault;
	/**
	* True if viewport is currently squished.
	***/
	bool m_bViewportIsSquished;
	/**
	* True if VRboost rules are present.
	**/
	bool m_VRboostRulesPresent;
	/**
	* Last viewport backup.
	**/
	D3DVIEWPORT9 m_LastViewportSet;
	/**
	* The squished viewport.
	***/
	D3DVIEWPORT9 m_ViewportIfSquished;
	/**
	* Active stored proxy depth stencil.
	**/
	D3D9ProxySurface* m_pActiveStereoDepthStencil;
	/**
	* Active stored index buffer.
	**/
	BaseDirect3DIndexBuffer9* m_pActiveIndicies;
	/**
	* Active stored vertex declaration.
	**/
	BaseDirect3DVertexDeclaration9* m_pActiveVertexDeclaration;
	/**
	* Active stored proxy swap chains.
	* The swap chains have to be released and then forcibly destroyed on reset or device destruction.
	* This should be the very last thing done in both cases.
	**/
	std::vector<D3D9ProxySwapChain*> m_activeSwapChains;
	/**
	* Active stored proxy render targets.
	* The render targets that are currently in use.
	***/
	std::vector<D3D9ProxySurface*> m_activeRenderTargets;	
	/**
	* Textures assigned to stages. 
	* (NULL is a valid entry in these containers. It indicates that the application has specifically 
	* cleared that stream/sampler. It is important that this information is available to the proxy 
	* StateBlock)
	* @see SetTexture()
	* @see GetTexture()
	**/
	std::unordered_map<DWORD, IDirect3DBaseTexture9*> m_activeTextureStages;
	/**
	* Active stored vertex buffers.
	**/
	std::unordered_map<UINT, BaseDirect3DVertexBuffer9*> m_activeVertexBuffers;
	/**
	* True if BeginStateBlock() is called, false if EndStateBlock is called.
	* @see BeginStateBlock()
	* @see EndStateBlock()
	**/
	bool m_bInBeginEndStateBlock;
	/**
	* True if view transform is set via SetTransform().
	* @see SetTransform()
	**/
	bool m_bViewTransformSet;
	/**
	* True if projection transform is set via SetTransform().
	* @see SetTransform() 
	**/
	bool m_bProjectionTransformSet;
	/**
	* The stored left view transform set via SetTransform().
	**/
	D3DXMATRIX m_leftView;
	/**
	* The stored right view transform set via SetTransform().
	**/
	D3DXMATRIX m_rightView;
	/**
	* The stored left projection transform set via SetTransform(). 
	**/
	D3DXMATRIX m_leftProjection;
	/**
	* The stored right projection transform set via SetTransform(). 
	**/
	D3DXMATRIX m_rightProjection;
	/**
	* Either the left or right view, depending on active render side.
	**/
	D3DXMATRIX* m_pCurrentView;
	/**
	* Either the left or right projection, depending on active render side.	
	**/
	D3DXMATRIX* m_pCurrentProjection;
	/**
	* Backup of the current game profile.
	***/
	ProxyHelper::ProxyConfig m_configBackup;
	/**
	* Current HUD 3D Depth mode.
	***/
	HUD_3D_Depth_Modes hud3DDepthMode;
	/**
	* Current GUI scale mode.
	***/
	GUI_3D_Depth_Modes gui3DDepthMode;
	/**
	* Old HUD 3D Depth mode, used to switch between 2 modes using hotkeys.
	***/
	HUD_3D_Depth_Modes oldHudMode;
	/**
	* Old GUI 3D Depth mode, used to switch between 2 modes using hotkeys.
	***/
	GUI_3D_Depth_Modes oldGuiMode;
	/**
	* Current HUD 3D Depth presets.
	***/
	float hud3DDepthPresets[4];
	/**
	* Current HUD distance presets.
	***/
	float hudDistancePresets[4];
	/**
	* Current GUI 3D Depth presets.
	***/
	float gui3DDepthPresets[4];
	/**
	* Current GUI squish presets.
	***/
	float guiSquishPresets[4];
	/**
	* Hotkey indices for the GUI.
	***/
	byte guiHotkeys[5];
	/**
	* Hotkey indices for the HUD.
	***/
	byte hudHotkeys[5];
	/**
	* Hotkey for VRBoost toggle.
	***/
	byte toggleVRBoostHotkey;
	/**
	* True if BRASSA is waiting to catch a hotkey.
	***/
	bool hotkeyCatch;
	/**
	* True if screenshot is taken next frame.
	***/
	int screenshot;
};

#endif