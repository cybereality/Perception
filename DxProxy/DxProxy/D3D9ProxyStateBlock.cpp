/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyStateBlock.cpp> and
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

#include "D3D9ProxyStateBlock.h"
#include <assert.h>

/**
* Constructor.
* pActualStateBlock can be NULL, but should only be NULL when creating block from within BeginStateBlock.
* The actual state block should be passed by calling EndStateBlock with the actual stateblock in the
* EndStateBlock of the device.
*
* Creates states selection (m_selectedStates) according to capture type.
*
* Calls CaptureSelectedFromProxyDevice() only if type is not Cap_Type_Selected.
* @see CaptureSelectedFromProxyDevice()
***/
D3D9ProxyStateBlock::D3D9ProxyStateBlock(IDirect3DStateBlock9* pActualStateBlock, D3DProxyDevice *pOwningDevice, CaptureType type, bool isSideLeft) :
	BaseDirect3DStateBlock9(pActualStateBlock, pOwningDevice),
	m_pWrappedDevice(pOwningDevice),
	m_eCaptureMode(type),
	m_storedTextureStages(),
	m_storedVertexBuffers(),
	m_storedViewport(),
	m_eSidesAre(isSideLeft ? SidesAllLeft : SidesAllRight),
	m_selectedStates(),
	m_selectedTextureSamplers(),
	m_selectedVertexStreams(),
	m_storedSelectedPSRegistersF(),
	m_storedAllPSRegistersF(),
	m_selectedPixelConstantRegistersF(),
	m_storedSelectedVSRegistersF(),
	m_storedAllVSRegistersF(),
	m_selectedVertexConstantRegistersF()
{
	assert (pOwningDevice != NULL);

	if (!pActualStateBlock) {
		assert(type == Cap_Type_Selected);
	}

	m_pWrappedDevice->AddRef();

	m_pStoredIndicies = NULL;
	m_pStoredVertexShader = NULL;
	m_pStoredVertexDeclaration = NULL;
	m_pStoredPixelShader = NULL;

	D3DXMatrixIdentity(&m_storedLeftView);
	D3DXMatrixIdentity(&m_storedRightView);
	D3DXMatrixIdentity(&m_storedLeftProjection);
	D3DXMatrixIdentity(&m_storedRightProjection);

	switch (type) {
	case Cap_Type_Full: 
		{
			static const CaptureableState toCapture[] = { IndexBuffer, Viewport, ViewMatrices, ProjectionMatrices, PixelShader, VertexShader, VertexDeclaration };
			m_selectedStates.insert(toCapture, toCapture + 7);
			break;
		}

	case Cap_Type_Vertex:
		{
			static const CaptureableState toCapture[] = { VertexShader, VertexDeclaration };
			m_selectedStates.insert(toCapture, toCapture + 2);
			break;
		}

	case Cap_Type_Pixel:
		{
			m_selectedStates.insert(PixelShader);
			break;
		}

	case Cap_Type_Selected:
		{
			// Select no specific state, state to capture will be indicated by calls to the various SelectAndCaptureState methods.
			break;
		}

	default:
		{
			OutputDebugString("Unhandled stateblock capture mode\n");
			break;
		}
	}


	if (type != Cap_Type_Selected) {
		CaptureSelectedFromProxyDevice();
	}
}

/**
* Destructor, calls ClearCapturedData().
* @see ClearCapturedData().
***/
D3D9ProxyStateBlock::~D3D9ProxyStateBlock()
{
	ClearCapturedData();

	m_selectedStates.clear();
	m_selectedTextureSamplers.clear();
	m_selectedVertexStreams.clear();
	m_selectedPixelConstantRegistersF.clear();
	m_selectedVertexConstantRegistersF.clear();

	m_pWrappedDevice->Release();
}

/**
* Calls super method and then CaptureSelectedFromProxyDevice().
* @see CaptureSelectedFromProxyDevice()
***/
HRESULT WINAPI D3D9ProxyStateBlock::Capture()
{
	HRESULT result = BaseDirect3DStateBlock9::Capture();

	if (SUCCEEDED(result)) {
		CaptureSelectedFromProxyDevice();
	}

	return result;
}

/**
* Applies basic state block states and stored ones.
*
* Calls private Apply() to apply non-indexed states. Note that this private method needs to know
* wether the states were captured while the device side was set to the same side. (set in 
* updateCaptureSideTracking() )
*
* Removes co-existing vertex buffers in wrapped device and inserts stored ones.
*
* Then it calls the managed shader register class (ShaderRegisters) stored in the wrapped device 
* (m_spManagedShaderRegisters) to apply all vertex shader constants according to capture type.
*
* If states were not captured while the device side was set to the same side it applies the 
* stored texture stages to the actual device else it applies the stored texture stages to the 
* wrapped device without applying to the actual device.
* @see updateCaptureSideTracking()
* @see Apply(CaptureableState toApply, bool reApplyStereo)
* @see ShaderRegisters
* @see m_spManagedShaderRegisters
***/
HRESULT WINAPI D3D9ProxyStateBlock::Apply()
{
	// assert that device isn't in the middle of a begin/end stateblock capture cycle because said situation is not accounted for 
	// (probably an error in D3D but haven't tested to check)
	assert (!m_pWrappedDevice->m_bInBeginEndStateBlock);


	// If all stereo states recorded on the same side then switch the proxy device to that side
	if (m_eSidesAre == SidesAllLeft) {
		m_pWrappedDevice->setDrawingSide(vireio::Left);
	}
	else if (m_eSidesAre == SidesAllRight) {
		m_pWrappedDevice->setDrawingSide(vireio::Right);
	}


	HRESULT result = BaseDirect3DStateBlock9::Apply();

	if (SUCCEEDED(result)) {

		// If mixed sides then manually apply all state from stereo components based on the current proxy device side to the actual device
		bool reApplyStereo = (m_eSidesAre == SidesMixed);

		// Note: We're only updating the internal state of the proxy device to reflect the state that has already been applied by the 
		// actual stateblock to the actual device for non-stereo states. For stereo states we do need to reapply the states on the device 
		// when the sides are mixed, so we use the normal Set methods in that case.)

		// Apply non-indexed states
		auto itSelected = m_selectedStates.begin();
		while (itSelected != m_selectedStates.end()) {
			Apply(*itSelected, reApplyStereo);
			++itSelected;
		}



		// Apply non-stereo indexed states
		auto itVertexBuffer = m_storedVertexBuffers.begin();
		while (itVertexBuffer != m_storedVertexBuffers.end()) {

			// Remove and Release existing buffer in proxy device
			if (m_pWrappedDevice->m_activeVertexBuffers.count(itVertexBuffer->first) == 1) {

				auto removing = m_pWrappedDevice->m_activeVertexBuffers[itVertexBuffer->first];
				if (removing != NULL) {
					removing->Release();
				}
				m_pWrappedDevice->m_activeVertexBuffers.erase(itVertexBuffer->first);
			}

			auto inserted = m_pWrappedDevice->m_activeVertexBuffers.insert(std::pair<UINT, BaseDirect3DVertexBuffer9*>(itVertexBuffer->first, itVertexBuffer->second));
			// insert success and buffer not NULL
			if (inserted.second && inserted.first->second) {
				inserted.first->second->AddRef();
			}

			++itVertexBuffer;
		}



		// Shader constants
		switch (m_eCaptureMode) 
		{			
		case Cap_Type_Selected:

			if (m_selectedStates.count(VertexShader) == 1) 
				m_pWrappedDevice->m_spManagedShaderRegisters->SetFromStateBlockVertexShader(m_pStoredVertexShader);
			if (m_selectedStates.count(PixelShader) == 1)
				m_pWrappedDevice->m_spManagedShaderRegisters->SetFromStateBlockPixelShader(m_pStoredPixelShader);

			m_pWrappedDevice->m_spManagedShaderRegisters->SetFromStateBlockData(&m_storedSelectedVSRegistersF, &m_storedSelectedPSRegistersF);
			break;

		case Cap_Type_Full:
			m_pWrappedDevice->m_spManagedShaderRegisters->SetFromStateBlockVertexShader(m_pStoredVertexShader);
			m_pWrappedDevice->m_spManagedShaderRegisters->SetFromStateBlockPixelShader(m_pStoredPixelShader);
			m_pWrappedDevice->m_spManagedShaderRegisters->SetFromStateBlockData(&m_storedAllVSRegistersF, &m_storedAllPSRegistersF);
			break;

		case Cap_Type_Vertex:
			m_pWrappedDevice->m_spManagedShaderRegisters->SetFromStateBlockVertexShader(m_pStoredVertexShader);
			m_pWrappedDevice->m_spManagedShaderRegisters->SetFromStateBlockData(&m_storedAllVSRegistersF, NULL);
			break;

		case Cap_Type_Pixel:
		default:
			m_pWrappedDevice->m_spManagedShaderRegisters->SetFromStateBlockPixelShader(m_pStoredPixelShader);
			m_pWrappedDevice->m_spManagedShaderRegisters->SetFromStateBlockData(NULL, &m_storedAllPSRegistersF);
			break;
		}



		// Apply stereo indexed states
		if (reApplyStereo) {

			// Textures
			auto itTextures = m_storedTextureStages.begin();
			while (itTextures != m_storedTextureStages.end()) {

				m_pWrappedDevice->SetTexture(itTextures->first, itTextures->second);
				++itTextures;
			}
		}
		// Update the internal state of the proxy device for the above indexed stereo states without applying to the actual device (actual stateblock will
		// have applied the correct state already)
		else {

			// Textures 
			auto itTextures = m_storedTextureStages.begin();
			while (itTextures != m_storedTextureStages.end()) {

				// Remove and Release existing active texture in proxy device
				if (m_pWrappedDevice->m_activeTextureStages.count(itTextures->first) == 1) {

					auto removing = m_pWrappedDevice->m_activeTextureStages[itTextures->first];
					if (removing != NULL) {
						removing->Release();
					}
					m_pWrappedDevice->m_activeTextureStages.erase(itTextures->first);
				}

				auto inserted = m_pWrappedDevice->m_activeTextureStages.insert(std::pair<DWORD, IDirect3DBaseTexture9*>(itTextures->first, itTextures->second));
				// insert success and texture not NULL
				if (inserted.second && inserted.first->second) {
					inserted.first->second->AddRef();
				}

				++itTextures;
			}
		}
	}


	return result;
}

/**
* Adds CaptureableState::IndexBuffer to selected states and captures the buffer.
* Use these methods when the respective methods on the device are called between Start/End StateBlock.
* @param pWrappedIndexBuffer The wrapped index buffer to be captured.
* @see D3D9ProxyStateBlock::CaptureableState
***/
void D3D9ProxyStateBlock::SelectAndCaptureState(BaseDirect3DIndexBuffer9* pWrappedIndexBuffer)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);
	assert (!m_pActualStateBlock);

	if (!m_selectedStates.insert(IndexBuffer).second) {
		// Release existing
		if (m_pStoredIndicies) {
			m_pStoredIndicies->Release();
		}
	} 

	m_pStoredIndicies = pWrappedIndexBuffer;
	if (m_pStoredIndicies) {
		m_pStoredIndicies->AddRef();
	}
}

/**
* Adds CaptureableState::Viewport to selected states and captures the viewport.
* Use these methods when the respective methods on the device are called between Start/End StateBlock.
* @param viewport The viewport to be captured.
* @see D3D9ProxyStateBlock::CaptureableState
***/
void D3D9ProxyStateBlock::SelectAndCaptureState(D3DVIEWPORT9 viewport)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (!m_pActualStateBlock);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);

	m_selectedStates.insert(Viewport);

	m_storedViewport = viewport;
}

/**
* Adds CaptureableState::ViewMatrices to selected states and captures the left and right view matrix.
* Use these methods when the respective methods on the device are called between Start/End StateBlock.
* Calls updateCaptureSideTracking().
* @param left The left view matrix to be captured.
* @param right The right view matrix to be captured.
* @see D3D9ProxyStateBlock::CaptureableState
* @see updateCaptureSideTracking()
***/
void D3D9ProxyStateBlock::SelectAndCaptureViewTransform(D3DXMATRIX left, D3DXMATRIX right)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);
	assert (!m_pActualStateBlock);

	m_selectedStates.insert(ViewMatrices);

	m_storedLeftView = left;
	m_storedRightView = right;

	updateCaptureSideTracking();
}

/**
* Adds CaptureableState::ProjectionMatrices to selected states and captures the projection matrices.
* Use these methods when the respective methods on the device are called between Start/End StateBlock.
* Calls updateCaptureSideTracking().
* @param left The left projection matrix to be captured.
* @param right The right projection matrix to be captured.
* @see D3D9ProxyStateBlock::CaptureableState
* @see updateCaptureSideTracking()
***/
void D3D9ProxyStateBlock::SelectAndCaptureProjectionTransform(D3DXMATRIX left, D3DXMATRIX right)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);
	assert (!m_pActualStateBlock);

	m_selectedStates.insert(ProjectionMatrices);

	m_storedLeftProjection = left;
	m_storedRightProjection = right;

	updateCaptureSideTracking();
}

/**
* Adds CaptureableState::PixelShader to selected states and captures the pixel shader.
* Use these methods when the respective methods on the device are called between Start/End StateBlock.
* @param pWrappedPixelShader The wrapped pixel shader to be captured.
* @see D3D9ProxyStateBlock::CaptureableState
***/
void D3D9ProxyStateBlock::SelectAndCaptureState(D3D9ProxyPixelShader* pWrappedPixelShader)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);
	assert (!m_pActualStateBlock);

	if (!m_selectedStates.insert(PixelShader).second) {
		// Release existing
		if (m_pStoredPixelShader) {
			m_pStoredPixelShader->Release();
		}
	} 

	m_pStoredPixelShader = pWrappedPixelShader;
	if (m_pStoredPixelShader) {
		m_pStoredPixelShader->AddRef();
	}
}

/**
* Adds CaptureableState::VertexShader to selected states and captures the vertex shader.
* Use these methods when the respective methods on the device are called between Start/End StateBlock.
* @param pWrappedVertexShader The wrapped vertex shader to be captured.
* @see D3D9ProxyStateBlock::CaptureableState
***/
void D3D9ProxyStateBlock::SelectAndCaptureState(D3D9ProxyVertexShader* pWrappedVertexShader)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);
	assert (!m_pActualStateBlock);

	if (!m_selectedStates.insert(VertexShader).second) {
		// Release existing
		if (m_pStoredVertexShader) {
			m_pStoredVertexShader->Release();
		}
	} 

	m_pStoredVertexShader = pWrappedVertexShader;
	if (m_pStoredVertexShader) {
		m_pStoredVertexShader->AddRef();
	}
}

/**
* Adds CaptureableState::VertexDeclaration to selected states and captures the vertex declaration.
* Use these methods when the respective methods on the device are called between Start/End StateBlock.
* @param pWrappedVertexDeclaration The vertex declaration to be captured.
* @see D3D9ProxyStateBlock::CaptureableState
***/
void D3D9ProxyStateBlock::SelectAndCaptureState(BaseDirect3DVertexDeclaration9* pWrappedVertexDeclaration)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);
	assert (!m_pActualStateBlock);

	if (!m_selectedStates.insert(VertexDeclaration).second) {
		// Release existing
		if (m_pStoredVertexDeclaration) {
			m_pStoredVertexDeclaration->Release();
		}
	} 

	m_pStoredVertexDeclaration = pWrappedVertexDeclaration;
	if (m_pStoredVertexDeclaration) {
		m_pStoredVertexDeclaration->AddRef();
	}
}

/**
* Adds stage to selected texture samplers and captures the texture.
* Use these methods when the respective methods on the device are called between Start/End StateBlock.
* Calls updateCaptureSideTracking().
* @param pWrappedTexture The texture to be captured.
* @see updateCaptureSideTracking()
***/
void D3D9ProxyStateBlock::SelectAndCaptureState(DWORD Stage, IDirect3DBaseTexture9* pWrappedTexture)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);
	assert (!m_pActualStateBlock);

	m_selectedTextureSamplers.insert(Stage);

	if (m_storedTextureStages.count(Stage) == 1) {
		IDirect3DBaseTexture9* pOldTexture = m_storedTextureStages.at(Stage);

		if (pOldTexture)
			pOldTexture->Release();

		m_storedTextureStages.erase(Stage);
	}

	if(m_storedTextureStages.insert(std::pair<DWORD, IDirect3DBaseTexture9*>(Stage, pWrappedTexture)).second) {
		if (pWrappedTexture)
			pWrappedTexture->AddRef();
	}

	updateCaptureSideTracking();
}

/**
* Adds stream number to selected vertex streams and captures the vertex buffer.
* Use these methods when the respective methods on the device are called between Start/End StateBlock.
* @param pWrappedStreamData The vertex buffer to be captured.
***/
void D3D9ProxyStateBlock::SelectAndCaptureState(UINT StreamNumber, BaseDirect3DVertexBuffer9* pWrappedStreamData)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);
	assert (!m_pActualStateBlock);

	m_selectedVertexStreams.insert(StreamNumber);

	if (m_storedVertexBuffers.count(StreamNumber) == 1) {
		BaseDirect3DVertexBuffer9* pOldBuffer = m_storedVertexBuffers.at(StreamNumber);

		if (pOldBuffer)
			pOldBuffer->Release();

		m_storedVertexBuffers.erase(StreamNumber);
	}

	if(m_storedVertexBuffers.insert(std::pair<DWORD, BaseDirect3DVertexBuffer9*>(StreamNumber, pWrappedStreamData)).second) {
		if (pWrappedStreamData)
			pWrappedStreamData->AddRef();
	}
}

/** 
* Adds registers to selected vertex constant registers and captures the constant data.
* Use these methods when the respective methods on the device are called between Start/End StateBlock.
* Assumption: Only float registers will need to be stereo. 
* If this proves to be untrue then int and bool containers will need adding throughout this class.
* @see D3DProxyDevice::SetVertexShaderConstantF()
***/
HRESULT WINAPI D3D9ProxyStateBlock::SelectAndCaptureStateVSConst(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);
	assert (!m_pActualStateBlock);

	// The assumption here is that most registers won't need to be stereo so we set them all on the actual device imiediately and overwrite the stereo ones 
	// when we Apply. (if there are any). This Simplifies "Apply" when this block is applied later.
	HRESULT result = m_pOwningDevice->SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);

	if (SUCCEEDED(result)) {
		// Mark these registers as being tracked and save them
		for (UINT i = StartRegister; i < (StartRegister + Vector4fCount); i++) {
			m_selectedVertexConstantRegistersF.insert(i);
			m_storedSelectedVSRegistersF.insert(std::pair<UINT, D3DXVECTOR4>(i, pConstantData));
		}
	}

	return result;
}

/** 
* Adds registers to selected pixel constant registers and captures the constant data.
* Use these methods when the respective methods on the device are called between Start/End StateBlock.
* Assumption: Only float registers will need to be stereo. 
* If this proves to be untrue then int and bool containers will need adding throughout this class.
* @see D3DProxyDevice::SetPixelShaderConstantF()
***/
HRESULT WINAPI D3D9ProxyStateBlock::SelectAndCaptureStatePSConst(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);
	assert (!m_pActualStateBlock);

	// The assumption here is that most registers won't need to be stereo so we set them all on the actual device imiediately and overwrite the stereo ones 
	// when we Apply. (if there are any). This Simplifies "Apply" when this block is applied later.
	HRESULT result = m_pOwningDevice->SetPixelShaderConstantF(StartRegister, pConstantData, Vector4fCount);

	if (SUCCEEDED(result)) {
		// Mark these registers as being tracked and save them
		for (UINT i = StartRegister; i < (StartRegister + Vector4fCount); i++) {
			m_selectedPixelConstantRegistersF.insert(i);
			m_storedSelectedPSRegistersF.insert(std::pair<UINT, D3DXVECTOR4>(i, pConstantData));
		}
	}

	return result;
}

/**
* If this ProxyStateBlock was created in a BeginStateBlock call then call this method in the 
* EndStateBlock with the actual stateblock returned from EndStateBlock of the actual device. 
* It is an error to call this method with NULL, to call it on a ProxyStateBlock created with an 
* actual state block or more than once. 
***/
void D3D9ProxyStateBlock::EndStateBlock(IDirect3DStateBlock9* pActualStateBlock)
{
	assert (pActualStateBlock != NULL);

	m_pActualStateBlock = pActualStateBlock;
}

/*void D3D9ProxyStateBlock::ClearSelected(UINT StartRegister) 
{
m_StoredStereoShaderConstsF.erase(StartRegister);
}*/

/**
* Captures all selected data from D3D proxy device.
*
* Calls Capture() to 'copy' all selected states to proxy state block from proxy device.
*
* Copies shader constant data from managed shader class (ShaderRegisters) stored in the wrapped
* device (m_spManagedShaderRegisters) according to capture type.
*
* If capture type is 'selected', it copies additional data according to the selections.
*
* @see CaptureType
* @see Capture()
* @see ShaderRegisters
* @see m_spManagedShaderRegisters
***/
void D3D9ProxyStateBlock::CaptureSelectedFromProxyDevice()
{
	// Clear out any existing captured data before we begin.
	ClearCapturedData();


	// 'Copy' (actually just keeping a reference) all selected (non-indexed) states to ProxyStateBlock from ProxyDevice
	auto itSelected = m_selectedStates.begin();
	while (itSelected != m_selectedStates.end()) {
		Capture(*itSelected);
		++itSelected;
	}

	// Copy indexed states
	switch (m_eCaptureMode) {
	case Cap_Type_Full: 
		{
			// if full - copy all textures, vertex buffers and shader constants.

			// Textures
			m_storedTextureStages = m_pWrappedDevice->m_activeTextureStages;

			// TODO Do we need to copy Textures rather than just keeping reference. Textures could be changed (have new data stretched/copied into them) 
			// TODO Check actual behaviour of state block. Is it saving a reference to the texture or a copy of the texture??
			// Need to increase ref count on all copied textures
			auto itTextures = m_storedTextureStages.begin();
			while (itTextures != m_storedTextureStages.end()) {
				if (itTextures->second != NULL) {
					itTextures->second->AddRef();
				}
				++itTextures;
			}


			// Vertex buffers
			m_storedVertexBuffers = m_pWrappedDevice->m_activeVertexBuffers;

			// TODO same question as for Textures above
			// Need to increase ref count on all copied vbs
			auto itVB = m_storedVertexBuffers.begin();
			while (itVB != m_storedVertexBuffers.end()) {
				if (itVB->second != NULL) {
					itVB->second->AddRef();
				}
				++itVB;
			}


			// Vertex Shader constants
			m_storedAllVSRegistersF = m_pWrappedDevice->m_spManagedShaderRegisters->GetAllVSConstantRegistersF();
			// Pixel Shader constants
			m_storedAllPSRegistersF = m_pWrappedDevice->m_spManagedShaderRegisters->GetAllPSConstantRegistersF();
			break;
		}

	case Cap_Type_Vertex:
		{
			// Vertex Shader constants
			m_storedAllVSRegistersF = m_pWrappedDevice->m_spManagedShaderRegisters->GetAllVSConstantRegistersF();

			break;
		}

	case Cap_Type_Pixel:
		{
			// Pixel Shader constants
			m_storedAllPSRegistersF = m_pWrappedDevice->m_spManagedShaderRegisters->GetAllPSConstantRegistersF();
			break;
		}

	case Cap_Type_Selected:
		{
			// if selected - iterate m_selectedTextureSamplers, m_selectedVertexStreams and m_selectedVertexConstantRegistersF and copy as selected.

			auto itSelectedTextures = m_selectedTextureSamplers.begin();
			while (itSelectedTextures != m_selectedTextureSamplers.end()) {

				if (m_pWrappedDevice->m_activeTextureStages.count(*itSelectedTextures) == 1) {

					auto inserted = m_storedTextureStages.insert(std::pair<DWORD, IDirect3DBaseTexture9*>(*itSelectedTextures, m_pWrappedDevice->m_activeTextureStages[*itSelectedTextures]));
					// insert success and texture is not NULL
					if (inserted.second && inserted.first->second) {
						inserted.first->second->AddRef();
					}
					else { // insertfailed
						//OutputDebugString("Texture sampler capture to StateBlock failed");
						//TODO aaaaa why does this get spammed
					}
				}

				++itSelectedTextures;
			}


			auto itSelectedVertexStreams = m_selectedVertexStreams.begin();
			while (itSelectedVertexStreams != m_selectedVertexStreams.end()) {

				if (m_pWrappedDevice->m_activeVertexBuffers.count(*itSelectedVertexStreams) == 1) {

					auto inserted = m_storedVertexBuffers.insert(std::pair<UINT, BaseDirect3DVertexBuffer9*>(*itSelectedVertexStreams, m_pWrappedDevice->m_activeVertexBuffers[*itSelectedVertexStreams]));
					// insert success and buffer is not NULL
					if (inserted.second && inserted.first->second) {
						inserted.first->second->AddRef();
					}
					else { // insertfailed
						OutputDebugString("Vertex buffer capture to StateBlock failed");
					}
				}

				++itSelectedVertexStreams;
			}



			// Vertex Shader constants
			float currentRegister [4];
			auto itSelectedVertexConstants = m_selectedVertexConstantRegistersF.begin();
			while (itSelectedVertexConstants != m_selectedVertexConstantRegistersF.end()) {

				m_pWrappedDevice->m_spManagedShaderRegisters->GetVertexShaderConstantF(*itSelectedVertexConstants, currentRegister, 1);

				m_storedSelectedVSRegistersF.insert(std::pair<UINT, D3DXVECTOR4>(*itSelectedVertexConstants, currentRegister));

				++itSelectedVertexConstants;
			}

			// Pixel Shader constants
			auto itSelectedPixelConstants = m_selectedPixelConstantRegistersF.begin();
			while (itSelectedPixelConstants != m_selectedPixelConstantRegistersF.end()) {

				m_pWrappedDevice->m_spManagedShaderRegisters->GetPixelShaderConstantF(*itSelectedPixelConstants, currentRegister, 1);

				m_storedSelectedPSRegistersF.insert(std::pair<UINT, D3DXVECTOR4>(*itSelectedPixelConstants, currentRegister));

				++itSelectedPixelConstants;
			}

			break;
		}

	default:
		{
			OutputDebugString("Unhandled stateblock capture mode\n");
			break;
		}
	}


	if (m_pWrappedDevice->m_currentRenderingSide == vireio::Left) {
		m_eSidesAre = SidesAllLeft;
	}
	else if (m_pWrappedDevice->m_currentRenderingSide == vireio::Right) {
		m_eSidesAre = SidesAllRight;
	}
	else {
		OutputDebugString("CaptureSelectedFromProxyDevice: This shouldn't be possible.\n");
	}
}

/**
* Stores data from wrapped device according to selection.
* @param toCap The selection to be copied.
* @see CaptureableState
***/
void D3D9ProxyStateBlock::Capture(CaptureableState toCap)
{
	switch (toCap) 
	{
	case IndexBuffer: 
		{
			m_pStoredIndicies = m_pWrappedDevice->m_pActiveIndicies;
			if (m_pStoredIndicies)
				m_pStoredIndicies->AddRef();

			break;
		}

	case Viewport: 
		{
			m_storedViewport = m_pWrappedDevice->m_LastViewportSet;
			break;
		}

	case ViewMatrices: 
		{
			m_storedLeftView = m_pWrappedDevice->m_leftView;
			m_storedRightView = m_pWrappedDevice->m_rightView;
			break;
		}

	case ProjectionMatrices: 
		{
			m_storedLeftProjection = m_pWrappedDevice->m_leftProjection;
			m_storedRightProjection = m_pWrappedDevice->m_rightProjection;
			break;
		}

	case PixelShader: 
		{
			m_pStoredPixelShader = m_pWrappedDevice->m_pActivePixelShader;
			if (m_pStoredPixelShader)
				m_pStoredPixelShader->AddRef();

			break;
		}

	case VertexShader: 
		{
			m_pStoredVertexShader = m_pWrappedDevice->m_pActiveVertexShader;
			if (m_pStoredVertexShader)
				m_pStoredVertexShader->AddRef();

			break;
		}

	case VertexDeclaration: 
		{
			m_pStoredVertexDeclaration = m_pWrappedDevice->m_pActiveVertexDeclaration;
			if (m_pStoredVertexDeclaration)
				m_pStoredVertexDeclaration->AddRef();

			break;
		}

	default:
		{
			OutputDebugString("SelectAndCaptureState: Unknown CaptureableState in ProxyStateBlock\n");
			break;
		}
	}
}

/**
* Clears all captured data.
***/
void D3D9ProxyStateBlock::ClearCapturedData()
{
	auto it = m_storedTextureStages.begin();
	while (it != m_storedTextureStages.end()) {
		if (it->second)
			it->second->Release();
		++it;
	}
	m_storedTextureStages.clear();

	auto it2 = m_storedVertexBuffers.begin();
	while (it2 != m_storedVertexBuffers.end()) {
		if (it2->second)
			it2->second->Release();
		++it2;
	}
	m_storedVertexBuffers.clear();

	m_storedSelectedVSRegistersF.clear();
	m_storedAllVSRegistersF.clear();

	m_storedSelectedPSRegistersF.clear();
	m_storedAllPSRegistersF.clear();

	if (m_pStoredIndicies) {
		m_pStoredIndicies->Release();
		m_pStoredIndicies = NULL;
	}

	if (m_pStoredVertexShader) {
		m_pStoredVertexShader->Release();
		m_pStoredVertexShader = NULL;
	}

	if (m_pStoredVertexDeclaration) {
		m_pStoredVertexDeclaration->Release();
		m_pStoredVertexDeclaration = NULL;
	}

	if (m_pStoredPixelShader) {
		m_pStoredPixelShader->Release();
		m_pStoredPixelShader = NULL;
	}

	D3DXMatrixIdentity(&m_storedLeftView);
	D3DXMatrixIdentity(&m_storedRightView);
	D3DXMatrixIdentity(&m_storedLeftProjection);
	D3DXMatrixIdentity(&m_storedRightProjection);
}

/**
* Applies stored data to wrapped device according to selection.
* @param toApply The selection to be applied.
* @param reApplyStereo True if the states were captured while the device side was set to the same side.  
* @see updateCaptureSideTracking() )
***/
void D3D9ProxyStateBlock::Apply(CaptureableState toApply, bool reApplyStereo)
{
	switch (toApply) 
	{
	case IndexBuffer: 
		{
			if (m_pWrappedDevice->m_pActiveIndicies)
				m_pWrappedDevice->m_pActiveIndicies->Release();

			m_pWrappedDevice->m_pActiveIndicies = m_pStoredIndicies;
			if (m_pWrappedDevice->m_pActiveIndicies)
				m_pStoredIndicies->AddRef();

			break;
		}

	case Viewport: 
		{
			m_pWrappedDevice->m_LastViewportSet = m_storedViewport;
			m_pWrappedDevice->m_bActiveViewportIsDefault = m_pWrappedDevice->isViewportDefaultForMainRT(&m_storedViewport);

			break;
		}

	case ViewMatrices: 
		{
			m_pWrappedDevice->SetStereoViewTransform(m_storedLeftView, m_storedRightView, reApplyStereo);

			break;
		}

	case ProjectionMatrices: 
		{
			m_pWrappedDevice->SetStereoProjectionTransform(m_storedLeftProjection, m_storedRightProjection, reApplyStereo);

			break;
		}

	case PixelShader: 
		{
			if (m_pWrappedDevice->m_pActivePixelShader)
				m_pWrappedDevice->m_pActivePixelShader->Release();

			m_pWrappedDevice->m_pActivePixelShader = m_pStoredPixelShader;
			if (m_pWrappedDevice->m_pActivePixelShader)
				m_pWrappedDevice->m_pActivePixelShader->AddRef();

			break;
		}

	case VertexShader: 
		{
			if (m_pWrappedDevice->m_pActiveVertexShader)
				m_pWrappedDevice->m_pActiveVertexShader->Release();

			m_pWrappedDevice->m_pActiveVertexShader = m_pStoredVertexShader;
			if (m_pWrappedDevice->m_pActiveVertexShader)
				m_pWrappedDevice->m_pActiveVertexShader->AddRef();

			break;
		}

	case VertexDeclaration: 
		{
			if (m_pWrappedDevice->m_pActiveVertexDeclaration)
				m_pWrappedDevice->m_pActiveVertexDeclaration->Release();

			m_pWrappedDevice->m_pActiveVertexDeclaration = m_pStoredVertexDeclaration;
			if (m_pWrappedDevice->m_pActiveVertexDeclaration)
				m_pWrappedDevice->m_pActiveVertexDeclaration->AddRef();

			break;
		}

	default:
		{
			OutputDebugString("SelectAndCaptureState: Unknown CaptureableState in ProxyStateBlock\n");
			break;
		}
	}
}

/**
* Needs to be called whenever a stereo state is recorded to see if side remains consistent.
* If sides ever become inconsistent amongst tracked stereo states then all stereo states need
* to be manually reapplied on apply. If they remain consistent then device side should be switched 
* to that side before applying (actual device will then apply the correct states for the side)
***/
inline void D3D9ProxyStateBlock::updateCaptureSideTracking()
{
	if (m_eSidesAre == SidesMixed)
		return;

	if (((m_pWrappedDevice->m_currentRenderingSide == vireio::Left) && (m_eSidesAre != SidesAllLeft)) ||
		((m_pWrappedDevice->m_currentRenderingSide == vireio::Right) && (m_eSidesAre != SidesAllRight))) {

			m_eSidesAre = SidesMixed;
	}
}