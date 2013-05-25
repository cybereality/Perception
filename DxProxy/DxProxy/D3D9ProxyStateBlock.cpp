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

#include "D3D9ProxyStateBlock.h"
#include <assert.h>

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
	m_StoredStereoShaderConstsF(),
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
			static const CaptureableState toCapture[] = { IndexBuffer, Viewport, ViewMatricies, ProjectionMatricies, PixelShader, VertexShader, VertexDeclaration };
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

D3D9ProxyStateBlock::~D3D9ProxyStateBlock()
{
	ClearCapturedData();

	m_selectedStates.clear();
	m_selectedTextureSamplers.clear();
	m_selectedVertexStreams.clear();
	m_selectedVertexConstantRegistersF.clear();
		
	m_pWrappedDevice->Release();
}


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

	m_StoredStereoShaderConstsF.clear();


	


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


inline void D3D9ProxyStateBlock::updateCaptureSideTracking()
{
	if (m_eSidesAre == SidesMixed)
		return;
	
	if (((m_pWrappedDevice->m_currentRenderingSide == D3DProxyDevice::Left) && (m_eSidesAre != SidesAllLeft)) ||
		((m_pWrappedDevice->m_currentRenderingSide == D3DProxyDevice::Right) && (m_eSidesAre != SidesAllRight))) {

		m_eSidesAre = SidesMixed;
	}
}


void D3D9ProxyStateBlock::CaptureSelectedFromProxyDevice()
{
	// Clear out any existing captured data before we begin.
	ClearCapturedData();

	// Copy all selected (non-indexed) states to ProxyStateBlock from ProxyDevice
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

			// Need to increase ref count on all copied vbs
			auto itVB = m_storedVertexBuffers.begin();
			while (itVB != m_storedVertexBuffers.end()) {
				if (itVB->second != NULL) {
					itVB->second->AddRef();
				}
				++itVB;
			}


			// Vertex Shader constants
			m_StoredStereoShaderConstsF = m_pWrappedDevice->m_activeStereoVShaderConstF;
			
			break;
		}

		case Cap_Type_Vertex:
		{
			// Vertex Shader constants
			m_StoredStereoShaderConstsF = m_pWrappedDevice->m_activeStereoVShaderConstF;

			break;
		}
		
		case Cap_Type_Pixel:
		{
			// No indexed pixel shader states need to be handled by the proxy.
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
						OutputDebugString("Texture sampler capture to StateBlock failed");
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
			auto itSelectedVertexConstants = m_selectedVertexConstantRegistersF.begin();
			while (itSelectedVertexConstants != m_selectedVertexConstantRegistersF.end()) {
				
				if (m_pWrappedDevice->m_activeStereoVShaderConstF.count(*itSelectedVertexConstants) == 1) {

					auto toInsert = m_pWrappedDevice->m_activeStereoVShaderConstF.find(*itSelectedVertexConstants);

					if (!(m_StoredStereoShaderConstsF.insert(std::pair<UINT, StereoShaderConstant<float>>(toInsert->first, toInsert->second)).second)) {
						OutputDebugString("Vertex Shader constant capture to StateBlock failed");
					}
				}

				++itSelectedVertexConstants;
			}
			
			break;
		}

		default:
		{
			OutputDebugString("Unhandled stateblock capture mode\n");
			break;
		}
	}


	if (m_pWrappedDevice->m_currentRenderingSide == D3DProxyDevice::Left) {
		m_eSidesAre = SidesAllLeft;
	}
	else if (m_pWrappedDevice->m_currentRenderingSide == D3DProxyDevice::Right) {
		m_eSidesAre = SidesAllRight;
	}
	else {
		OutputDebugString("CaptureSelectedFromProxyDevice: This shouldn't be possible.\n");
	}
}


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

		case ViewMatricies: 
		{
			m_storedLeftView = m_pWrappedDevice->m_leftView;
			m_storedRightView = m_pWrappedDevice->m_rightView;
			break;
		}

		case ProjectionMatricies: 
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


HRESULT WINAPI D3D9ProxyStateBlock::Capture()
{
	HRESULT result = BaseDirect3DStateBlock9::Capture();

	if (SUCCEEDED(result)) {
		CaptureSelectedFromProxyDevice();
	}
	
	return result;
}



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

		case ViewMatricies: 
		{
			m_pWrappedDevice->SetStereoViewTransform(m_storedLeftView, m_storedRightView, reApplyStereo);

			break;
		}

		case ProjectionMatricies: 
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


HRESULT WINAPI D3D9ProxyStateBlock::Apply()
{
	// assert that device isn't in the middle of a begin/end stateblock capture cycle because said situation is not accounted for 
	// (probably an error in D3D but haven't tested to check)
	assert (!m_pWrappedDevice->m_bInBeginEndStateBlock);


	// If all stereo states recorded on the same side then switch the proxy device to that side
	if (m_eSidesAre == SidesAllLeft) {
		m_pWrappedDevice->setDrawingSide(D3DProxyDevice::Left);
	}
	else if (m_eSidesAre == SidesAllRight) {
		m_pWrappedDevice->setDrawingSide(D3DProxyDevice::Right);
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



		// Apply stereo indexed states
		if (reApplyStereo) {

			// Textures
			auto itTextures = m_storedTextureStages.begin();
			while (itTextures != m_storedTextureStages.end()) {

				m_pWrappedDevice->SetTexture(itTextures->first, itTextures->second);
				++itTextures;
			}
			
			// VertexShader constants
			auto itVSConstF = m_StoredStereoShaderConstsF.begin();
			while (itVSConstF != m_StoredStereoShaderConstsF.end()) {

				m_pWrappedDevice->SetVertexShaderConstantF(itVSConstF->second.StartRegister, (m_pWrappedDevice->m_currentRenderingSide == D3DProxyDevice::Left) ? itVSConstF->second.DataLeftPointer() : itVSConstF->second.DataRightPointer(), itVSConstF->second.Count);
				++itVSConstF;
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


			// VertexShader constants
			auto itVSConstF = m_StoredStereoShaderConstsF.begin();
			while (itVSConstF != m_StoredStereoShaderConstsF.end()) {

				if (m_pWrappedDevice->m_activeStereoVShaderConstF.count(itVSConstF->first) == 1) {
					m_pWrappedDevice->m_activeStereoVShaderConstF.erase(itVSConstF->first);
				}

				m_pWrappedDevice->m_activeStereoVShaderConstF.insert(std::pair<UINT, StereoShaderConstant<float>>(itVSConstF->first, itVSConstF->second));
				++itVSConstF;
			}

		}
	}


	return result;
}


void D3D9ProxyStateBlock::EndStateBlock(IDirect3DStateBlock9* pActualStateBlock)
{
	assert (pActualStateBlock != NULL);

	m_pActualStateBlock = pActualStateBlock;
}



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

void D3D9ProxyStateBlock::SelectAndCaptureState(D3DVIEWPORT9 viewport)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (!m_pActualStateBlock);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);

	m_selectedStates.insert(Viewport);

	m_storedViewport = viewport;
}

void D3D9ProxyStateBlock::SelectAndCaptureViewTransform(D3DXMATRIX left, D3DXMATRIX right)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);
	assert (!m_pActualStateBlock);

	m_selectedStates.insert(ViewMatricies);

	m_storedLeftView = left;
	m_storedRightView = right;

	updateCaptureSideTracking();
}

void D3D9ProxyStateBlock::SelectAndCaptureProjectionTransform(D3DXMATRIX left, D3DXMATRIX right)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);
	assert (!m_pActualStateBlock);

	m_selectedStates.insert(ProjectionMatricies);

	m_storedLeftProjection = left;
	m_storedRightProjection = right;

	updateCaptureSideTracking();
}

void D3D9ProxyStateBlock::SelectAndCaptureState(BaseDirect3DPixelShader9* pWrappedPixelShader)
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

void D3D9ProxyStateBlock::SelectAndCaptureState(BaseDirect3DVertexShader9* pWrappedVertexShader)
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

void D3D9ProxyStateBlock::SelectAndCaptureState(StereoShaderConstant<float> stereoFloatConstant)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (m_pWrappedDevice->m_bInBeginEndStateBlock);
	assert (!m_pActualStateBlock);

	// Mark this starting register as being tracked
	m_selectedVertexConstantRegistersF.insert(stereoFloatConstant.StartRegister);

	// remove existing (if there is one)
	m_StoredStereoShaderConstsF.erase(stereoFloatConstant.StartRegister);

	// Store stereoFloatConstant
	m_StoredStereoShaderConstsF.insert(std::pair<UINT, StereoShaderConstant<float>>(stereoFloatConstant.StartRegister, stereoFloatConstant));

	updateCaptureSideTracking();
}


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

