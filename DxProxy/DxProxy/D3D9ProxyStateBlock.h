/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2013 Chris Drain

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
#include "Direct3DVertexBuffer9.h"
#include "Direct3DIndexBuffer9.h"
#include "Direct3DVertexShader9.h"
#include "Direct3DPixelShader9.h"
#include "Direct3DVertexDeclaration9.h"
#include "StereoShaderConstant.h"

class BaseDirect3DStateBlock9;
class D3DProxyDevice;

/*
	Responsible for applying state changes to the proxy device so that the proxy devices state remains
	consistent with the actual devices state when an actual state block is applied.

	Also responsible for ensuring the the correct side is applied for stereo components. (The side at 
	the time of capture may not match the side when the state block is applied.)

 */
class D3D9ProxyStateBlock : public BaseDirect3DStateBlock9
{
public:

	enum CaptureType
	{
		Cap_Type_Full = 1, 
		Cap_Type_Pixel = 2,
		Cap_Type_Vertex = 3,
		Cap_Type_Selected = 4,
		Cap_Type_FORCE_DWORD   = 0x7fffffff
	};

	/* In this context states to capture are any state that a proxy needs to track extra information for
		beyond the underlying StateBlocks normal capture. */
	enum CaptureableState
	{
		IndexBuffer = 0,
		Viewport = 1,
		ViewMatricies = 2,			//(transform)
		ProjectionMatricies = 3,	//(transform)
		PixelShader = 4,
		VertexShader = 5,
		VertexDeclaration = 6
	};

	enum IndexedCaptureableState
	{
		Texture = 1,					// needs to track which sampler
		VertexBuffer = 2,				// needs to track which stream
		VertexShaderConstantsF = 3,		// StereoShaderConstants only, leave the rest to the actual state block
	};

	/*
		pActualStateBlock can be NULL, but should only be NULL when creating block from within BeginStateBlock.
		The actual state block should be passed by calling EndStateBlock with the actual stateblock in the
		EndStateBlock of the device.
	 */
	D3D9ProxyStateBlock(IDirect3DStateBlock9* pActualStateBlock, D3DProxyDevice* pOwningDevice, CaptureType type, bool isSideLeft);
	virtual ~D3D9ProxyStateBlock();

	// ID3D9ProxyStateBlock
	virtual HRESULT WINAPI Capture();
	virtual HRESULT WINAPI Apply();

	/*** Use these methods when the respective methods on the device are called between Start/End StateBlock ***/
	void SelectAndCaptureState(BaseDirect3DIndexBuffer9* pWrappedIndexBuffer);
	void SelectAndCaptureState(D3DVIEWPORT9 viewport);
	void SelectAndCaptureViewTransform(D3DXMATRIX left, D3DXMATRIX right);
	void SelectAndCaptureProjectionTransform(D3DXMATRIX left, D3DXMATRIX right);
	void SelectAndCaptureState(BaseDirect3DPixelShader9* pWrappedPixelShader);
	void SelectAndCaptureState(BaseDirect3DVertexShader9* pWrappedVertexShader);
	void SelectAndCaptureState(BaseDirect3DVertexDeclaration9* pWrappedVertexDeclaration);

	/* 
		Assumption: If multiple SetConstant calls are made during a Begin/End StateBlock they will not partially
		overwrite each other. There is nothing to stop a program doing this but it would be more difficult to handle
		and I can't think of a reason for this to be intentionally done so I'm ignoring the possibility for now. 

		Assumption 2: Only float registers will need to be stereo. If this proves to be untrue then int and bool containers
		will need adding throughout this class.
	 */
	void SelectAndCaptureState(StereoShaderConstant<float> stereoFloatConstant);

	void SelectAndCaptureState(DWORD Stage, IDirect3DBaseTexture9* pWrappedTexture);
	void SelectAndCaptureState(UINT StreamNumber, BaseDirect3DVertexBuffer9* pWrappedStreamData);
	/***********************************************************************************************************/

	/* If this ProxyStateBlock was created in a BeginStateBlock call then
		call this method in the EndStateBlock with the actual stateblock returned from
		EndStateBlock of the actual device. 
		It is an error to call this method with NULL, to call it on a ProxyStateBlock
		created with an actual state block or more than once. */
	void EndStateBlock(IDirect3DStateBlock9* pActualStateBlock);

private:
	void CaptureSelectedFromProxyDevice();
	void Capture(CaptureableState toCap);

	void ClearCapturedData();

	void Apply(CaptureableState toApply, bool reApplyStereo);

	// Needs to be called whenever a stereo state is recorded to see if side remains consistent.
	// If sides ever become inconsistent amongst tracked stereo states then all stereo states need
	// to be manually reapplied on apply. If they remain consisten then device side should be switched 
	// to that side before applying (actual device will then apply the correct states for the side)
	void updateCaptureSideTracking();

	/*
		If all captures of stereo states are done on the same side we can just switch to
		that side and apply when needed. If stereo states are captured on different sides
		then they need to be reapplied using the current side after the actual stateblock 
		has been applied.
	 */
	enum CaptureSides
	{
		SidesAllLeft = 1,
		SidesAllRight = 2,
		SidesMixed = 3
	};



	/*	Were all states in this block captured while the device side was set to the same side. */
	CaptureSides m_eSidesAre;

	const CaptureType m_eCaptureMode;

	// Selected States to capture are only relevant if CaptureType is Cap_Type_Selected
	std::unordered_set<CaptureableState> m_selectedStates;
	std::unordered_set<DWORD> m_selectedTextureSamplers; 
	std::unordered_set<UINT> m_selectedVertexStreams;
	std::unordered_set<UINT> m_selectedVertexConstantRegistersF;




	/*
		All simple states can be tracked by the device. This is basically all states that are basic value types rather 
		than interfaces (For example, everything set by SetSamplerState).


		Any states that are wrapped need to have the wrapped state stored. (Textures, Vertex/Pixel Shaders, etc.)

		For stereo components like Textures all parameters needed to manually re-apply them need to be saved (textures only 
		need the texture and the number of the sampler the texture is in). This is because we can't guarantee that all states
		will be saved while the same side is active. If all states were saved on the same side then the stereo components won't 
		need to be reapplied and the procedure will be the same as for non-stereo components stated below.

		Non-stereo componenets like Vertex Buffers and Shaders just need the wrapper reference stored in order to be able
		to restore the wrapper in the device proxy. The actual state will be restored on the device by the actual StateBlock.


		View, projection and world matrix because we keep a left and right version. (TODO World matrix might be world-view 
		in which case there will be left and right versions. Don't have a good way to handle this yet (or a test case), 
		leaving world matrix to actual device for now)
	 */
		
	////////////////////// General State //////////////////////

	// Textures in samplers (standard, vertex and displacement)
	std::unordered_map<DWORD, IDirect3DBaseTexture9*> m_storedTextureStages;

	// Vertex Buffers
	std::unordered_map<UINT, BaseDirect3DVertexBuffer9*> m_storedVertexBuffers;

	// Index Buffer
	BaseDirect3DIndexBuffer9* m_pStoredIndicies;

	// Viewport - Viewport handled by actual device but need to update proxy device viewport state (update m_bActiveViewportIsDefault and m_LastViewportSet) 
	D3DVIEWPORT9 m_storedViewport;
	
	// View and Projection matricies (transform)
	D3DXMATRIX m_storedLeftView;
	D3DXMATRIX m_storedRightView;
	D3DXMATRIX m_storedLeftProjection;
	D3DXMATRIX m_storedRightProjection;

	// Texture transforms (Ignore - assumed to always be mono at the moment)




	////////////////////// Vertex Shader State //////////////////////

	// vertex shader
	BaseDirect3DVertexShader9* m_pStoredVertexShader;

	// vertex declaration
	BaseDirect3DVertexDeclaration9* m_pStoredVertexDeclaration;

	// stereo vertex shader constants - (normal constants are handled by actual StateBlock)
	std::unordered_map<UINT, StereoShaderConstant<float>> m_StoredStereoShaderConstsF;
	



	////////////////////// Pixel Shader State //////////////////////

	// pixel shader 
	BaseDirect3DPixelShader9* m_pStoredPixelShader;

	// Pixel shader constants TODO - if needed (if stereo'ified for some reason), otherwise leave these to the actual device.


	// Had issues when the type of the device in the base class was the wrapped type rather than the Interface type.
	// Duplicated the device here under the wrapped pointer. Almost certain to at least confuse someone later.
	// TODO fix. This was the quick and dirty 2am style solution. This shouldn't be here and the base classs device should
	// just be of the derived type.
	D3DProxyDevice* const m_pWrappedDevice;
};

#endif

