/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
********************************************************************/

#include "D3DProxyDeviceAdv.h"

#define _SAFE_RELEASE(x) if(x) { x->Release(); x = NULL; } 

bool dirtyRegisters[300];

const float n = 0.1f;
const float f = 10.0f;
const float l = -0.5f;
const float r = 0.5f;
const float t = 0.5f;
const float b = -0.5f;

const D3DXMATRIX invertProjection(
	1.0f/(2.0f*n),	0.0f,			0.0f,	0.0f,
	0.0f,			1.0f/(2.0f*n),	0.0f,	0.0f,
	0.0f,			0.0f,			0.0f,	(n-f)/(f*n),
	0.0f,			0.0f,			1.0f,	1.0f/n
);

D3DProxyDeviceAdv::D3DProxyDeviceAdv(IDirect3DDevice9* pDevice):D3DProxyDevice(pDevice)
{
}

D3DProxyDeviceAdv::~D3DProxyDeviceAdv()
{
}

void D3DProxyDeviceAdv::Init(ProxyHelper::ProxyConfig& cfg)
{
	OutputDebugString("D3D ProxyDev Test Init\n");
	D3DProxyDevice::Init(cfg);
	//matrixIndex = getMatrixIndex();
}


HRESULT WINAPI D3DProxyDeviceAdv::BeginScene()
{
	HandleControls();
	HandleTracking();

	if(saveDebugFile)
	{
		debugFile.open("d3d9_debug.txt", std::ios::out);
	}

	return D3DProxyDevice::BeginScene();
}



HRESULT WINAPI D3DProxyDeviceAdv::EndScene()
{
	// delay to avoid crashing on start
	static int initDelay = 120;
	initDelay--;

	if(!stereoView->initialized && initDelay < 0)
	{
		stereoView->Init(m_pDevice);
	}

	return D3DProxyDevice::EndScene();
}

HRESULT WINAPI D3DProxyDeviceAdv::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	if(stereoView->initialized)
	{
		if(eyeShutter > 0)
		{
			stereoView->UpdateEye(StereoView::LEFT_EYE);
		}
		else 
		{
			stereoView->UpdateEye(StereoView::RIGHT_EYE);
		}

		stereoView->Draw();

		eyeShutter *= -1;
	}

	if(eyeShutter > 0) return D3D_OK;
	HRESULT hr = D3DProxyDevice::Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);

	return hr;
}

HRESULT WINAPI D3DProxyDeviceAdv::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	
	if(stereoView->initialized)
	{
		for(UINT i = StartRegister; i < StartRegister + Vector4fCount; i++)
		{
			dirtyRegisters[i] = true;
		}
	}

	return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

void D3DProxyDeviceAdv::adjustEyeOffsetAndViewFrustum(D3DXMATRIX &outMatrix, D3DXMATRIX &inMatrix)
{
	D3DXMATRIX transform;
	D3DXMatrixTranslation(&transform, separation*eyeShutter*10.0f+offset*10.0f, 0, 0);

	float adjustedFrustumOffset = convergence*eyeShutter*0.1f;		
	D3DXMATRIX reProject;
	D3DXMatrixPerspectiveOffCenterLH(&reProject, l+adjustedFrustumOffset, r+adjustedFrustumOffset, b, t, n, f);

	if(trackerInitialized && tracker->isAvailable())
	{
		D3DXMATRIX rollMatrix;
		D3DXMatrixRotationZ(&rollMatrix, tracker->currentRoll);
		D3DXMatrixMultiply(&transform, &transform, &rollMatrix);
	}

	outMatrix = inMatrix * invertProjection * transform * reProject;
}

void transformIndividualFloat(IDirect3DDevice9 *pD3Ddev, int startRegister, D3DProxyDeviceAdv* proxyDev) 
{
	float pConstantData[4];
	pD3Ddev->GetVertexShaderConstantF(startRegister, pConstantData, 1);
	pConstantData[0] += proxyDev->separation*proxyDev->eyeShutter*100.0f+proxyDev->offset;
	pD3Ddev->SetVertexShaderConstantF(startRegister, pConstantData, 1);
	memset(&dirtyRegisters[startRegister], 0, sizeof(boolean));
}

void transformRowMajor4by4(IDirect3DDevice9 *pD3Ddev, int startRegister, D3DProxyDeviceAdv* proxyDev) 
{
	float pConstantData[16];
	pD3Ddev->GetVertexShaderConstantF(startRegister, pConstantData, 4);
	D3DXMATRIX original = D3DXMATRIX(pConstantData);
	D3DXMATRIX transposed;
	D3DXMatrixTranspose(&transposed, &original);
	proxyDev->adjustEyeOffsetAndViewFrustum(original, transposed);
	D3DXMatrixTranspose(&transposed, &original);
	pD3Ddev->SetVertexShaderConstantF(startRegister, transposed,4);
	memset(&dirtyRegisters[startRegister], 0, sizeof(boolean)*4);
}

void transform4by4(IDirect3DDevice9 *pD3Ddev, int startRegister, D3DProxyDeviceAdv* proxyDev) 
{
	float pConstantData[16];
	pD3Ddev->GetVertexShaderConstantF(startRegister, pConstantData, 4);
	D3DXMATRIX inMatrix = D3DXMATRIX(pConstantData);
	if(inMatrix._41 == 0.0f) return; //Mirror's Edge compat. hack (otherwise you get weird squares)
	D3DXMATRIX outMatrix;
	proxyDev->adjustEyeOffsetAndViewFrustum(outMatrix, inMatrix);
	pD3Ddev->SetVertexShaderConstantF(startRegister, outMatrix,4);
	memset(&dirtyRegisters[startRegister], 0, sizeof(boolean)*4);
}

void D3DProxyDeviceAdv::transformDirtyShaderParams(IDirect3DDevice9 *pD3Ddev)
{
	for(std::list<TargetMatrix>::iterator i = targetMatrices.begin(); i != targetMatrices.end(); i++)
	{
		if(dirtyRegisters[i->startRegister])
		{
			i->transformationFunc(pD3Ddev, i->startRegister, this);
		}
	}
}

void D3DProxyDeviceAdv::removeExistingMatrices(D3DXCONSTANT_DESC &desc)
{	
	std::list<TargetMatrix>::iterator i = targetMatrices.begin();
	while(i != targetMatrices.end())
	{
		if(desc.RegisterIndex <= (*i).startRegister && desc.RegisterIndex+desc.RegisterCount > (*i).startRegister)
		{
			targetMatrices.erase(i++);
		}
		else
		{
			++i;
		}
	}
}

void D3DProxyDeviceAdv::parse4by4Matrices(D3DXCONSTANT_DESC &desc)
{
	if(desc.Name == NULL) return;
	if(!strstr(desc.Name, "proj") && !strstr(desc.Name, "Proj"))return;
	if(desc.RegisterCount != 4) return;			
	TargetMatrix tm;
	tm.startRegister = desc.RegisterIndex;
	tm.transformationFunc = (desc.Class == 2) ? &transformRowMajor4by4 : &transform4by4;
	targetMatrices.push_back(tm);
}

void D3DProxyDeviceAdv::parseIndividualFloats(D3DXCONSTANT_DESC &desc)
{
	if(desc.Name == NULL) return;
	if(!strstr(desc.Name, "EyePos")) return;
	if(desc.RegisterCount != 1) return;
	TargetMatrix tm;
	tm.startRegister = desc.RegisterIndex;
	tm.transformationFunc = &transformIndividualFloat;
	targetMatrices.push_back(tm);
}

void D3DProxyDeviceAdv::findWeirdMirrorsEdgeShader(UINT pSizeOfData)
{	
	if(pSizeOfData != 172) return;
	TargetMatrix tm;
	tm.startRegister = 0;

	tm.transformationFunc = &transform4by4;
	targetMatrices.push_back(tm);
}

bool D3DProxyDeviceAdv::hasSeenShader(UINT size)
{
	for(std::list<UINT>::iterator i = seenShaders.begin(); i != seenShaders.end(); i++)
	{
		if(*i == size) return true;
	}
	return false;
}

HRESULT APIENTRY D3DProxyDeviceAdv::SetVertexShader(IDirect3DVertexShader9* pvShader) 
{	
	IDirect3DVertexShader9* pShader = NULL;
	LPD3DXCONSTANTTABLE pConstantTable = NULL;
	BYTE *pData = NULL;

	HRESULT hr = m_pDevice->SetVertexShader(pvShader);
	m_pDevice->GetVertexShader(&pShader);
	UINT pSizeOfData;
	if(NULL == pShader) goto grexit;
	pShader->GetFunction(NULL,&pSizeOfData);
	findWeirdMirrorsEdgeShader(pSizeOfData);
	pData = new BYTE[pSizeOfData];
	pShader->GetFunction(pData,&pSizeOfData);

	bool shaderSeen = hasSeenShader(pSizeOfData);

    D3DXCONSTANT_DESC pConstantDesc[32];
    UINT pConstantNum = 32;

    D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(pData),&pConstantTable);
	if(pConstantTable == NULL) goto grexit;
    D3DXCONSTANTTABLE_DESC pDesc;
    pConstantTable->GetDesc(&pDesc);
	for(UINT i = 0; i < pDesc.Constants; i++)
	{
		D3DXHANDLE Handle = pConstantTable->GetConstant(NULL,i);
		if(Handle == NULL) continue;
		pConstantTable->GetConstantDesc(Handle,pConstantDesc,&pConstantNum);
		for(UINT j = 0; j < pConstantNum; j++)
		{
			removeExistingMatrices(pConstantDesc[j]);
			parse4by4Matrices(pConstantDesc[j]);
			parseIndividualFloats(pConstantDesc[j]);			
		}
	}
	
grexit:
	_SAFE_RELEASE(pConstantTable);
	_SAFE_RELEASE(pShader);
	if(pData) delete[] pData;
	return hr;
}

HRESULT APIENTRY D3DProxyDeviceAdv::DrawIndexedPrimitive(D3DPRIMITIVETYPE Type,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	transformDirtyShaderParams(m_pDevice);
	return D3DProxyDevice::DrawIndexedPrimitive(Type,BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

HRESULT APIENTRY D3DProxyDeviceAdv::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT MinIndex, UINT NumVertices, UINT PrimitiveCount, CONST void *pIndexData, D3DFORMAT IndexDataFormat, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) 
{	
	transformDirtyShaderParams(m_pDevice);
	return D3DProxyDevice::DrawIndexedPrimitiveUP(PrimitiveType, MinIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY D3DProxyDeviceAdv::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType, UINT StartVertex, UINT PrimitiveCount) 
{
	transformDirtyShaderParams(m_pDevice);
	return D3DProxyDevice::DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

HRESULT APIENTRY D3DProxyDeviceAdv::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType, UINT PrimitiveCount, CONST void *pVertexStreamZeroData, UINT VertexStreamZeroStride) 
{
	transformDirtyShaderParams(m_pDevice);
	return D3DProxyDevice::DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

HRESULT APIENTRY D3DProxyDeviceAdv::DrawRectPatch(UINT Handle, CONST float *pNumSegs, CONST D3DRECTPATCH_INFO *pRectPatchInfo) 
{
	transformDirtyShaderParams(m_pDevice);
	return D3DProxyDevice::DrawRectPatch(Handle, pNumSegs, pRectPatchInfo);
}

HRESULT APIENTRY D3DProxyDeviceAdv::DrawTriPatch(UINT Handle, CONST float *pNumSegs, CONST D3DTRIPATCH_INFO *pTriPatchInfo)
{	
	transformDirtyShaderParams(m_pDevice);
	return D3DProxyDevice::DrawTriPatch(Handle, pNumSegs, pTriPatchInfo);
}