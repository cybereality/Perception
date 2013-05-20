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
	m_eCaptureMode(type),
	m_storedTextureStages(),
	m_storedVertexBuffers(),
	m_storedViewport(),
	m_eSidesAre(isSideLeft ? SidesAllLeft : SidesAllRight),
	m_selectedStates(),
	m_selectedTextureSamplers(),
	m_selectedVertexStreams()
{
	if (!pActualStateBlock) {
		assert(type == Cap_Type_Selected);
	}

	m_pStoredIndicies = NULL;
	m_pStoredVertexShader = NULL;
	m_pStoredVertexDeclaration = NULL;
	//m_pVertexShaderConstants = NULL;
	m_pStoredPixelShader = NULL;
	
	D3DXMatrixIdentity(&m_storedLeftView);
	D3DXMatrixIdentity(&m_storedRightView);
	D3DXMatrixIdentity(&m_storedLeftProjection);
	D3DXMatrixIdentity(&m_storedRightProjection);
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

	m_selectedStates.clear();
	m_selectedTextureSamplers.clear();
	m_selectedVertexStreams.clear();

	if (m_pStoredIndicies)
		m_pStoredIndicies->Release();

	if (m_pStoredVertexShader)
		m_pStoredVertexShader->Release();

	if (m_pStoredVertexDeclaration)
		m_pStoredVertexDeclaration->Release();

	if (m_pStoredPixelShader)
		m_pStoredPixelShader->Release();
		
}




HRESULT WINAPI D3D9ProxyStateBlock::Capture()
{
	// TODO
	// Capture on actual device
	// For all actively tracked special states capture from proxy device 
	//  this could be none (tracked states are all simple and handled by device), it could be a collection of individual states, 
	//  all states, vertex states or pixel states)


	return m_pActualStateBlock->Capture();
}

HRESULT WINAPI D3D9ProxyStateBlock::Apply()
{
	// assert that device isn't in the middle of a begin/end stateblock capture cycle becauase said situation is not accounted for 
	// (don't know if it's possible in directx. Won't test unless this assert starts getting triggerd)

	// TODO
	// Apply on actual device.
	// If mixed sides then manually apply all state from stereo components based on the current device side

	// Update the proxy device 


	return m_pActualStateBlock->Apply();
}


void D3D9ProxyStateBlock::SelectAndCaptureState(BaseDirect3DIndexBuffer9* pWrappedIndexBuffer)
{
	assert(m_eCaptureMode == Cap_Type_Selected);

	m_selectedStates.insert(IndexBuffer);

	m_pStoredIndicies = pWrappedIndexBuffer;
	if (!m_pStoredIndicies) {
		m_pStoredIndicies->AddRef();
	}
}

void D3D9ProxyStateBlock::SelectAndCaptureState(D3DVIEWPORT9 viewport)
{
	assert(m_eCaptureMode == Cap_Type_Selected);

	m_selectedStates.insert(Viewport);

	m_storedViewport = viewport;
}

void D3D9ProxyStateBlock::SelectAndCaptureViewTransform(D3DXMATRIX left, D3DXMATRIX right)
{
	assert(m_eCaptureMode == Cap_Type_Selected);

	m_selectedStates.insert(ViewMatricies);

	m_storedLeftView = left;
	m_storedRightView = right;
}

void D3D9ProxyStateBlock::SelectAndCaptureProjectionTransform(D3DXMATRIX left, D3DXMATRIX right)
{
	assert(m_eCaptureMode == Cap_Type_Selected);

	m_selectedStates.insert(ProjectionMatricies);

	m_storedLeftProjection = left;
	m_storedRightProjection = right;
}

void D3D9ProxyStateBlock::SelectAndCaptureState(BaseDirect3DPixelShader9* pWrappedPixelShader)
{
	assert(m_eCaptureMode == Cap_Type_Selected);

	m_selectedStates.insert(PixelShader);

	m_pStoredPixelShader = pWrappedPixelShader;
	if (!m_pStoredPixelShader) {
		m_pStoredPixelShader->AddRef();
	}
}

void D3D9ProxyStateBlock::SelectAndCaptureState(BaseDirect3DVertexShader9* pWrappedVertexShader)
{
	assert(m_eCaptureMode == Cap_Type_Selected);

	m_selectedStates.insert(VertexShader);

	m_pStoredVertexShader = pWrappedVertexShader;
	if (!m_pStoredVertexShader) {
		m_pStoredVertexShader->AddRef();
	}
}

void D3D9ProxyStateBlock::SelectAndCaptureState(BaseDirect3DVertexDeclaration9* pWrappedVertexDeclaration)
{
	assert(m_eCaptureMode == Cap_Type_Selected);

	m_selectedStates.insert(VertexDeclaration);

	m_pStoredVertexDeclaration = pWrappedVertexDeclaration;
	if (!m_pStoredVertexDeclaration) {
		m_pStoredVertexDeclaration->AddRef();
	}
}

void D3D9ProxyStateBlock::SelectAndCaptureState(ConstantRecord<float> stereoFloatConstant)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
}

/*void D3D9ProxyStateBlock::SelectAndCaptureState(ConstantRecord<int> stereoIntConstant)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
}

void D3D9ProxyStateBlock::SelectAndCaptureState(ConstantRecord<bool> stereoBoolConstant)
{
	assert(m_eCaptureMode == Cap_Type_Selected);
}*/

void D3D9ProxyStateBlock::SelectAndCaptureState(DWORD Stage, IDirect3DBaseTexture9* pWrappedTexture)
{
	assert(m_eCaptureMode == Cap_Type_Selected);

	m_selectedTextureSamplers.insert(Stage);

	if (m_storedTextureStages.count(Stage) == 1) {
		IDirect3DBaseTexture9* pOldTexture = m_storedTextureStages.at(Stage);
		pOldTexture->Release();
		m_storedTextureStages.erase(Stage);
	}

	if(m_storedTextureStages.insert(std::pair<DWORD, IDirect3DBaseTexture9*>(Stage, pWrappedTexture)).second) {
		pWrappedTexture->AddRef();
	}
}

void D3D9ProxyStateBlock::SelectAndCaptureState(UINT StreamNumber, BaseDirect3DVertexBuffer9* pWrappedStreamData)
{
	assert(m_eCaptureMode == Cap_Type_Selected);

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