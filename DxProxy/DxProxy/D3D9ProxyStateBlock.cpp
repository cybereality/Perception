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
	p_WrappedDevice(pOwningDevice),
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

	p_WrappedDevice->AddRef();

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
			static const StateToCapture toCapture[] = { IndexBuffer, Viewport, ViewMatricies, ProjectionMatricies, PixelShader, VertexShader, VertexDeclaration };
			m_selectedStates.insert(toCapture, toCapture + 7);
		}

		case Cap_Type_Vertex:
		{
			static const StateToCapture toCapture[] = { VertexShader, VertexDeclaration };
			m_selectedStates.insert(toCapture, toCapture + 2);
		}
		
		case Cap_Type_Pixel:
		{
			m_selectedStates.insert(PixelShader);
		}

		case Cap_Type_Selected:
		{
			// Select no specific state, state to capture will be indicated by calls to the various SelectAndCaptureState methods.
		}

		default:
		{
			OutputDebugString("Unhandled stateblock capture mode\n");
		}
	}


	if (type != Cap_Type_Selected) {
		CaptureSelectedFromProxyDevice();
	}
}

D3D9ProxyStateBlock::~D3D9ProxyStateBlock()
{
	auto it = m_storedTextureStages.begin();
	while (it != m_storedTextureStages.end()) {
		it->second->Release();
		it = m_storedTextureStages.erase(it);
	}

	auto it2 = m_storedVertexBuffers.begin();
	while (it2 != m_storedVertexBuffers.end()) {
		it2->second->Release();
		it2 = m_storedVertexBuffers.erase(it2);
	}

	m_StoredStereoShaderConstsF.clear();


	m_selectedStates.clear();
	m_selectedTextureSamplers.clear();
	m_selectedVertexStreams.clear();
	m_selectedVertexConstantRegistersF.clear();


	if (m_pStoredIndicies)
		m_pStoredIndicies->Release();

	if (m_pStoredVertexShader)
		m_pStoredVertexShader->Release();

	if (m_pStoredVertexDeclaration)
		m_pStoredVertexDeclaration->Release();

	if (m_pStoredPixelShader)
		m_pStoredPixelShader->Release();
		
	p_WrappedDevice->Release();
}


inline void D3D9ProxyStateBlock::updateCaptureSideTracking()
{
	if (m_eSidesAre == SidesMixed)
		return;
	
	if (((p_WrappedDevice->m_currentRenderingSide == D3DProxyDevice::Left) && (m_eSidesAre != SidesAllLeft)) ||
		((p_WrappedDevice->m_currentRenderingSide == D3DProxyDevice::Right) && (m_eSidesAre != SidesAllRight))) {

		m_eSidesAre = SidesMixed;
	}
}


void D3D9ProxyStateBlock::CaptureSelectedFromProxyDevice()
{

	// TODO copy all selected state to ProxyStateBlock from ProxyDevice
	

	if (p_WrappedDevice->m_currentRenderingSide == D3DProxyDevice::Left) {
		m_eSidesAre = SidesAllLeft;
	}
	else if (p_WrappedDevice->m_currentRenderingSide == D3DProxyDevice::Right) {
		m_eSidesAre = SidesAllRight;
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

HRESULT WINAPI D3D9ProxyStateBlock::Apply()
{
	// assert that device isn't in the middle of a begin/end stateblock capture cycle because said situation is not accounted for 
	// (probably an error in D3D but haven't tested to check)
	assert (!p_WrappedDevice->m_bInBeginEndStateBlock);


	// If all stereo states recorded on the same side then switch to that side
	if (m_eSidesAre == SidesAllLeft) {
		p_WrappedDevice->setDrawingSide(D3DProxyDevice::Left);
	}
	else if (m_eSidesAre == SidesAllRight) {
		p_WrappedDevice->setDrawingSide(D3DProxyDevice::Right);
	}

	HRESULT result = BaseDirect3DStateBlock9::Apply();

	if (SUCCEEDED(result)) {
		// If mixed sides then manually apply all state from stereo components based on the current proxy device side to the actual device
		if (m_eSidesAre == SidesMixed) {
			// TODO View matrix
			// Projection matrix
			// Textures
			// VertexShader constants
		}

		// TODO update all proxy device data
	}


	return m_pActualStateBlock->Apply();
}


void D3D9ProxyStateBlock::EndStateBlock(IDirect3DStateBlock9* pActualStateBlock)
{
	assert (!pActualStateBlock);

	m_pActualStateBlock = pActualStateBlock;
}


void D3D9ProxyStateBlock::SelectAndCaptureState(BaseDirect3DIndexBuffer9* pWrappedIndexBuffer)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (p_WrappedDevice->m_bInBeginEndStateBlock);

	m_selectedStates.insert(IndexBuffer);

	m_pStoredIndicies = pWrappedIndexBuffer;
	if (!m_pStoredIndicies) {
		m_pStoredIndicies->AddRef();
	}
}

void D3D9ProxyStateBlock::SelectAndCaptureState(D3DVIEWPORT9 viewport)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (p_WrappedDevice->m_bInBeginEndStateBlock);

	m_selectedStates.insert(Viewport);

	m_storedViewport = viewport;
}

void D3D9ProxyStateBlock::SelectAndCaptureViewTransform(D3DXMATRIX left, D3DXMATRIX right)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (p_WrappedDevice->m_bInBeginEndStateBlock);

	m_selectedStates.insert(ViewMatricies);

	m_storedLeftView = left;
	m_storedRightView = right;

	updateCaptureSideTracking();
}

void D3D9ProxyStateBlock::SelectAndCaptureProjectionTransform(D3DXMATRIX left, D3DXMATRIX right)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (p_WrappedDevice->m_bInBeginEndStateBlock);

	m_selectedStates.insert(ProjectionMatricies);

	m_storedLeftProjection = left;
	m_storedRightProjection = right;

	updateCaptureSideTracking();
}

void D3D9ProxyStateBlock::SelectAndCaptureState(BaseDirect3DPixelShader9* pWrappedPixelShader)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (p_WrappedDevice->m_bInBeginEndStateBlock);

	m_selectedStates.insert(PixelShader);

	m_pStoredPixelShader = pWrappedPixelShader;
	if (!m_pStoredPixelShader) {
		m_pStoredPixelShader->AddRef();
	}
}

void D3D9ProxyStateBlock::SelectAndCaptureState(BaseDirect3DVertexShader9* pWrappedVertexShader)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (p_WrappedDevice->m_bInBeginEndStateBlock);

	m_selectedStates.insert(VertexShader);

	m_pStoredVertexShader = pWrappedVertexShader;
	if (!m_pStoredVertexShader) {
		m_pStoredVertexShader->AddRef();
	}
}

void D3D9ProxyStateBlock::SelectAndCaptureState(BaseDirect3DVertexDeclaration9* pWrappedVertexDeclaration)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (p_WrappedDevice->m_bInBeginEndStateBlock);

	m_selectedStates.insert(VertexDeclaration);

	m_pStoredVertexDeclaration = pWrappedVertexDeclaration;
	if (!m_pStoredVertexDeclaration) {
		m_pStoredVertexDeclaration->AddRef();
	}
}

void D3D9ProxyStateBlock::SelectAndCaptureState(StereoShaderConstant<float> stereoFloatConstant)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (p_WrappedDevice->m_bInBeginEndStateBlock);

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
	assert (p_WrappedDevice->m_bInBeginEndStateBlock);

	m_selectedTextureSamplers.insert(Stage);

	if (m_storedTextureStages.count(Stage) == 1) {
		IDirect3DBaseTexture9* pOldTexture = m_storedTextureStages.at(Stage);
		pOldTexture->Release();
		m_storedTextureStages.erase(Stage);
	}

	if(m_storedTextureStages.insert(std::pair<DWORD, IDirect3DBaseTexture9*>(Stage, pWrappedTexture)).second) {
		pWrappedTexture->AddRef();
	}

	updateCaptureSideTracking();
}

void D3D9ProxyStateBlock::SelectAndCaptureState(UINT StreamNumber, BaseDirect3DVertexBuffer9* pWrappedStreamData)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
	assert (p_WrappedDevice->m_bInBeginEndStateBlock);

	m_selectedVertexStreams.insert(StreamNumber);

	if (m_storedVertexBuffers.count(StreamNumber) == 1) {
		BaseDirect3DVertexBuffer9* pOldBuffer = m_storedVertexBuffers.at(StreamNumber);
		pOldBuffer->Release();
		m_storedVertexBuffers.erase(StreamNumber);
	}

	if(m_storedVertexBuffers.insert(std::pair<DWORD, BaseDirect3DVertexBuffer9*>(StreamNumber, pWrappedStreamData)).second) {
		pWrappedStreamData->AddRef();
	}
}


//
//void D3D9ProxyStateBlock::SelectAndCaptureState(StateToCapture state)
//{
//	switch (state) 
//	{
//		case IndexBuffer: 
//		{
//
//			break;
//		}
//
//		case Viewport: 
//		{
//
//			break;
//		}
//
//		case ViewMatricies: 
//		{
//
//			break;
//		}
//
//		case ProjectionMatricies: 
//		{
//
//			break;
//		}
//
//		case PixelShader: 
//		{
//
//			break;
//		}
//
//		case VertexShader: 
//		{
//
//			break;
//		}
//
//		case VertexShaderConstants:
//		{
//
//			break;
//		}
//
//		case VertexDeclaration: 
//		{
//
//			break;
//		}
//
//		default:
//		{
//			OutputDebugString("SelectAndCaptureState: Unknown StateToCapture in ProxyStateBlock\n");
//			break;
//		}
//	}
//}
//
//void D3D9ProxyStateBlock::SelectAndCaptureState(IndexedStateToCapture state, UINT index)
//{
//	switch (state) 
//	{
//		case IndexBuffer: 
//		{
//
//			break;
//		}
//
//		case Viewport: 
//		{
//
//			break;
//		}
//
//		default:
//		{
//			OutputDebugString("SelectAndCaptureState: Unknown IndexedStateToCapture in ProxyStateBlock\n");
//			break;
//		}
//	}
//}