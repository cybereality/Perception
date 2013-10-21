/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyStateBlock.h> and
Class <D3D9ProxyStateBlock> :
Copyright (C) 2013 Chris Drain

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

#ifndef D3D9PROXYSTATEBLOCK_H_INCLUDED
#define D3D9PROXYSTATEBLOCK_H_INCLUDED

#include <d3d9.h>
#include <d3dx9.h>
#include "D3DProxyDevice.h"
#include "Direct3DStateBlock9.h"
#include <unordered_set>
#include <unordered_map>
#include <map>
#include "Direct3DVertexBuffer9.h"
#include "Direct3DIndexBuffer9.h"
#include "D3D9ProxyPixelShader.h"
#include "D3D9ProxyVertexShader.h"
#include "Direct3DPixelShader9.h"
#include "Direct3DVertexDeclaration9.h"
#include "StereoShaderConstant.h"

class BaseDirect3DStateBlock9;
class D3DProxyDevice;
class D3D9ProxyPixelShader;
class D3D9ProxyVertexShader;

/**
* D3D proxy state block class - broker class for any device states.
*
* Responsible for applying state changes to the proxy device so that the proxy devices state remains
* consistent with the actual devices state when an actual state block is applied.
*
* Also responsible for ensuring that the correct side is applied for stereo components. (The side at 
* the time of capture may not match the side when the state block is applied.)
* 
*
* All simple states can be tracked by the device. This is basically all states that are basic value 
* types rather than interfaces (For example, everything set by SetSamplerState).
*
*
* Any states that are wrapped need to have the wrapped state stored. (Textures, Vertex/Pixel Shaders,
* etc.)
* 
* For stereo components like Textures all parameters needed to manually re-apply them need to be 
* saved (textures only need the texture and the number of the sampler the texture is in). This is 
* because we can't guarantee that all states will be saved while the same side is active. If all 
* states were saved on the same side then the stereo components won't need to be reapplied and the 
* procedure will be the same as for non-stereo components stated below.
*
* Non-stereo componenets like Vertex Buffers and Shaders just need the wrapper reference stored in 
* order to be able to restore the wrapper in the device proxy. The actual state will be restored on 
* the device by the actual StateBlock.
*
*
* View, projection and world matrix because we keep a left and right version. (TODO World matrix 
* might be world-view in which case there will be left and right versions. Don't have a good way to 
* handle this yet (or a test case), leaving world matrix to actual device for now)
*
* Ignore Texture transforms - assumed to always be mono at the moment.
*
* Don't save stereo versions of constants. Will be recalculated as needed from dirty registers when 
* proxy registers have been restored.
*
* Pixel shader constants TODO - if needed (if stereo'ified for some reason), otherwise leave these to 
* the actual device.
*/
class D3D9ProxyStateBlock : public BaseDirect3DStateBlock9
{
public:
	/**
	* Capture type according to state block type.
	***/
	enum CaptureType
	{
		Cap_Type_Full = 1,                        /**< D3DSTATEBLOCKTYPE::D3DSBT_ALL **/
		Cap_Type_Pixel = 2,                       /**< D3DSTATEBLOCKTYPE::D3DSBT_PIXELSTATE **/
		Cap_Type_Vertex = 3,                      /**< D3DSTATEBLOCKTYPE::D3DSBT_VERTEXSTATE **/
		Cap_Type_Selected = 4,                    /**< Proxy type : Selected - used for proxy device class intern state block. @see m_pCapturingStateTo @see D3D9ProxyDevice::BeginStateBlock() **/
		Cap_Type_FORCE_DWORD   = 0x7fffffff       /**< D3DSTATEBLOCKTYPE::D3DSBT_FORCE_DWORD **/
	};

	/**
	* In this context states to capture are any state that a proxy needs to track extra information for
	* beyond the underlying StateBlocks normal capture. 
	***/
	enum CaptureableState
	{
		IndexBuffer = 0,            /**< Index buffer will be captured. **/       
		Viewport = 1,               /**< Viewport will be captured. **/
		ViewMatrices = 2,           /**< View matrices will be captured. (transform) **/
		ProjectionMatrices = 3,     /**< Projection matrices will be captured. (transform) **/
		PixelShader = 4,            /**< Pixel shader will be captured. **/
		VertexShader = 5,           /**< Vertex shader will be captured. **/
		VertexDeclaration = 6       /**< VertexDeclaration will be captured. **/
	};

	/**
	*
	***/
	enum IndexedCaptureableState
	{
		Texture = 1,					/**< needs to track which sampler **/
		VertexBuffer = 2,				/**< needs to track which stream **/
		VertexShaderConstantsF = 3,		/**< StereoShaderConstants only, leave the rest to the actual state block **/
	};

	D3D9ProxyStateBlock(IDirect3DStateBlock9* pActualStateBlock, D3DProxyDevice* pOwningDevice, CaptureType type, bool isSideLeft);
	virtual ~D3D9ProxyStateBlock();

	/*** IDirect3DStateBlock9 methods ***/
	virtual HRESULT WINAPI Capture();
	virtual HRESULT WINAPI Apply();

	/*** D3D9ProxyStateBlock public methods ***/ 
	void           SelectAndCaptureState(BaseDirect3DIndexBuffer9* pWrappedIndexBuffer);
	void           SelectAndCaptureState(D3DVIEWPORT9 viewport);
	void           SelectAndCaptureViewTransform(D3DXMATRIX left, D3DXMATRIX right);
	void           SelectAndCaptureProjectionTransform(D3DXMATRIX left, D3DXMATRIX right);
	void           SelectAndCaptureState(D3D9ProxyPixelShader* pWrappedPixelShader);
	void           SelectAndCaptureState(D3D9ProxyVertexShader* pWrappedVertexShader);
	void           SelectAndCaptureState(BaseDirect3DVertexDeclaration9* pWrappedVertexDeclaration);
	void           SelectAndCaptureState(DWORD Stage, IDirect3DBaseTexture9* pWrappedTexture);
	void           SelectAndCaptureState(UINT StreamNumber, BaseDirect3DVertexBuffer9* pWrappedStreamData);
	HRESULT WINAPI SelectAndCaptureStateVSConst(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	HRESULT WINAPI SelectAndCaptureStatePSConst(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	void           EndStateBlock(IDirect3DStateBlock9* pActualStateBlock);
	//void         ClearSelected(UINT StartRegister);

private:
	/*** D3D9ProxyStateBlock private methods ***/ 
	void CaptureSelectedFromProxyDevice();
	void Capture(CaptureableState toCap);
	void ClearCapturedData();
	void Apply(CaptureableState toApply, bool reApplyStereo);
	void updateCaptureSideTracking();

	/**
	* If all captures of stereo states are done on the same side we can just switch to
	* that side and apply when needed. If stereo states are captured on different sides
	* then they need to be reapplied using the current side after the actual stateblock 
	* has been applied.
	* @see updateCaptureSideTracking()
	***/
	enum CaptureSides
	{
		SidesAllLeft = 1,     /**< consistent on left side **/
		SidesAllRight = 2,    /**< consistent on right side **/
		SidesMixed = 3        /**< sides became inconsistent **/
	};

	/**	
	* Were all states in this block captured while the device side was set to the same side? 
	***/
	CaptureSides m_eSidesAre;
	/**
	* The constant capture mode set in constructor.
	***/
	const CaptureType m_eCaptureMode;
	/**
	* Selected States to capture are only relevant if CaptureType is Cap_Type_Selected.
	***/
	std::unordered_set<CaptureableState> m_selectedStates;
	/**
	* Selected States to capture are only relevant if CaptureType is Cap_Type_Selected.
	***/
	std::unordered_set<DWORD> m_selectedTextureSamplers; 
	/**
	* Selected States to capture are only relevant if CaptureType is Cap_Type_Selected.
	***/
	std::unordered_set<UINT> m_selectedVertexStreams;
	/**
	* Selected States to capture are only relevant if CaptureType is Cap_Type_Selected.
	***/
	std::unordered_set<UINT> m_selectedVertexConstantRegistersF;
	/**
	* Selected States to capture are only relevant if CaptureType is Cap_Type_Selected.
	***/
	std::unordered_set<UINT> m_selectedPixelConstantRegistersF;
	/**
	* General States - Textures in samplers (standard, vertex and displacement).
	***/
	std::unordered_map<DWORD, IDirect3DBaseTexture9*> m_storedTextureStages;
	/**
	* General States - Vertex Buffers.
	***/
	std::unordered_map<UINT, BaseDirect3DVertexBuffer9*> m_storedVertexBuffers;
	/**
	* General States - Index Buffer.
	***/
	BaseDirect3DIndexBuffer9* m_pStoredIndicies;	
	/**
	* General States - Viewport.
	* Viewport handled by actual device but need to update proxy device viewport state (update 
	* m_bActiveViewportIsDefault and m_LastViewportSet).
	***/
	D3DVIEWPORT9 m_storedViewport;
	/**
	* General States - Left view matrix (transform).
	***/
	D3DXMATRIX m_storedLeftView;
	/**
	* General States - Right view matrix (transform).
	***/
	D3DXMATRIX m_storedRightView;
	/**
	* General States - Left projection matrix (transform).
	***/
	D3DXMATRIX m_storedLeftProjection;
	/**
	* General States - Right projection matrix (transform).
	***/
	D3DXMATRIX m_storedRightProjection;
	/**
	* Vertex Shader States - Stored vertex shader.
	***/
	D3D9ProxyVertexShader* m_pStoredVertexShader;
	/**
	* Vertex Shader States - vertex declaration
	***/
	BaseDirect3DVertexDeclaration9* m_pStoredVertexDeclaration;
	/**
	* Vertex Shader States -  Shader registers 
	* Use this when using Cap_Type_Selected.
	**/
	std::map<UINT, D3DXVECTOR4> m_storedSelectedVSRegistersF;
	/**
	* Vertex Shader States -  Shader registers 
	* Use this to copy everything when needed with other modes.
	**/
	std::vector<float> m_storedAllVSRegistersF;
	/**
	* Pixel Shader State - Stored pixel shader.
	***/
	D3D9ProxyPixelShader* m_pStoredPixelShader;
	/**
	* Pixel Shader States -  Shader registers 
	* Use this when using Cap_Type_Selected.
	**/
	std::map<UINT, D3DXVECTOR4> m_storedSelectedPSRegistersF;
	/**
	* Pixel Shader States -  Shader registers 
	* Use this to copy everything when needed with other modes.
	**/
	std::vector<float> m_storedAllPSRegistersF;
	/**
	* Pointer to wrapped device - quick solution (TODO) 
	* Had issues when the type of the device in the base class was the wrapped type rather than the 
	* Interface type.
	* Duplicated the device here under the wrapped pointer. Almost certain to at least confuse someone 
	* later.
	* TODO fix. This was the quick and dirty 2am style solution. This shouldn't be here and the base 
	* classs device should just be of the derived type.
	***/
	D3DProxyDevice* const m_pWrappedDevice;
};
#endif

