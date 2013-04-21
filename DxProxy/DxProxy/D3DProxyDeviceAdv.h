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

#ifndef D3DPROXYDEVICEADV_H_INCLUDED
#define D3DPROXYDEVICEADV_H_INCLUDED

#include "Direct3DDevice9.h"
#include "D3DProxyDevice.h"
#include "ProxyHelper.h"
#include <windows.h>
#include <list>

class D3DProxyDeviceAdv : public D3DProxyDevice
{
public:
	D3DProxyDeviceAdv(IDirect3DDevice9* pDevice);
	virtual ~D3DProxyDeviceAdv();
	virtual HRESULT WINAPI BeginScene();
	virtual HRESULT WINAPI EndScene();
	virtual HRESULT WINAPI Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
	virtual HRESULT WINAPI SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	virtual HRESULT WINAPI SetVertexShader(IDirect3DVertexShader9* pShader);

	virtual HRESULT WINAPI DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
	virtual HRESULT WINAPI DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	virtual HRESULT WINAPI DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
	virtual HRESULT WINAPI DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	virtual HRESULT WINAPI DrawRectPatch(UINT Handle,CONST float* pNumSegs,CONST D3DRECTPATCH_INFO* pRectPatchInfo);
	virtual HRESULT WINAPI DrawTriPatch(UINT Handle,CONST float* pNumSegs,CONST D3DTRIPATCH_INFO* pTriPatchInfo);

	virtual void Init(ProxyHelper::ProxyConfig& cfg);

	void adjustEyeOffsetAndViewFrustum(D3DXMATRIX &outMatrix, D3DXMATRIX &inMatrix);
	//void transformIndividualFloat(IDirect3DDevice9 *pD3Ddev, int startRegister);
	//void transformRowMajor4by4(IDirect3DDevice9 *pD3Ddev, int startRegister);
	//void transform4by4(IDirect3DDevice9 *pD3Ddev, int startRegister);
	void transformDirtyShaderParams(IDirect3DDevice9 *pD3Ddev);
	void removeExistingMatrices(D3DXCONSTANT_DESC &desc);
	void parse4by4Matrices(D3DXCONSTANT_DESC &desc);
	void parseIndividualFloats(D3DXCONSTANT_DESC &desc);
	void findWeirdMirrorsEdgeShader(UINT pSizeOfData);
	bool hasSeenShader(UINT size);

	struct TargetMatrix {
		UINT startRegister;
		void (*transformationFunc)(IDirect3DDevice9*, int, D3DProxyDeviceAdv*);
		TargetMatrix():startRegister(0),transformationFunc(NULL) {}
	};
	std::list<TargetMatrix> targetMatrices;
	std::list<UINT> seenShaders;
};

#endif