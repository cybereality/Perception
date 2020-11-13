/***************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio
Copyright © 2014 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider

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
****************************************************************/

#include "PCL_ID3D10Device.h"

/**
* Constructor.
***/
PCL_ID3D10Device::PCL_ID3D10Device(ID3D10Device* pID3D10Device)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("PCL_ID3D10Device");
#endif
		
	m_pID3D10Device = pID3D10Device;
}

/**
* Empty Deconstructor.
***/
PCL_ID3D10Device::~PCL_ID3D10Device()
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("~PCL_ID3D10Device");
#endif


}

#define IF_GUID(riid,a,b,c,d,e,f,g,h,i,j,k) if ((riid.Data1==a)&&(riid.Data2==b)&&(riid.Data3==c)&&(riid.Data4[0]==d)&&(riid.Data4[1]==e)&&(riid.Data4[2]==f)&&(riid.Data4[3]==g)&&(riid.Data4[4]==h)&&(riid.Data4[5]==i)&&(riid.Data4[6]==j)&&(riid.Data4[7]==k))
/**
* Wrapped QueryInterface functionality.
***/
HRESULT PCL_ID3D10Device::QueryInterface(REFIID riid, void** ppvObject)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("QueryInterface");
#endif

	/* IID IID_ID3D10Device */
	/* 9B7E4C0F-342C-4106-A19F-4F2704F689F0 */
	IF_GUID(riid,0x9B7E4C0F,0x342C,0x4106,0xA1,0x9F,0x4F,0x27,0x94,0xF6,0x89,0xF0)
	{	
		OutputDebugString("IID_ID3D10Device");
		*ppvObject=(LPVOID)this;
		this->AddRef();
		return S_OK;
	}

	/* IID_IDXGIDevice */
	/* 54ec77fa-1377-44e6-8c32-88fd5f44c84c */
	IF_GUID(riid,0x54ec77fa,0x1377,0x44e6,0x8c,0x32,0x88,0xfd,0x5f,0x44,0xc8,0x4c)
	{
		OutputDebugString("IID_IDXGIDevice");

	}

	HRESULT hRes = m_pID3D10Device->QueryInterface(riid, ppvObject);

	return hRes;
}

/**
* Base functionality.
***/
ULONG PCL_ID3D10Device::AddRef()
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("AddRef");
#endif

	return m_pID3D10Device->AddRef();
}

/**
* Base functionality.
***/
ULONG PCL_ID3D10Device::Release()
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("Release");
#endif

	extern PCL_ID3D10Device* g_pPCL_ID3D10Device;

	ULONG count = m_pID3D10Device->Release();

	if (count == 0)
	{
		g_pPCL_ID3D10Device = NULL;
		delete this;
	}

	/*char buf[64];
	sprintf_s(buf, "%u", count);
	OutputDebugString(buf);*/

	return count;
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::VSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppConstantBuffers)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("VSSetConstantBuffers");
#endif

	m_pID3D10Device->VSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::PSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView *const *ppShaderResourceViews)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("PSSetShaderResources");
#endif


	m_pID3D10Device->PSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::PSSetShader(ID3D10PixelShader *pPixelShader)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("PSSetShader");
#endif

	m_pID3D10Device->PSSetShader(pPixelShader);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::PSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D10SamplerState *const *ppSamplers)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("PSSetSamplers");
#endif
	/*char buf[64];
	sprintf_s(buf, "%u", StartSlot);
	OutputDebugString(buf);
	sprintf_s(buf, "%u", NumSamplers);
	OutputDebugString(buf);
	sprintf_s(buf, "%u", ppSamplers);
	OutputDebugString(buf);
	sprintf_s(buf, "%u", *ppSamplers);
	OutputDebugString(buf);
	sprintf_s(buf, "%u", **ppSamplers);
	OutputDebugString(buf);*/

	m_pID3D10Device->PSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::VSSetShader(ID3D10VertexShader *pVertexShader)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("VSSetShader");
#endif

	m_pID3D10Device->VSSetShader(pVertexShader);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::DrawIndexed(UINT IndexCount, UINT StartIndexLocation, INT BaseVertexLocation)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("DrawIndexed");
#endif

	m_pID3D10Device->DrawIndexed(IndexCount, StartIndexLocation, BaseVertexLocation);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::Draw(UINT VertexCount, UINT StartVertexLocation)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("Draw");
#endif

	m_pID3D10Device->Draw(VertexCount, StartVertexLocation);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::PSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppConstantBuffers)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("PSSetConstantBuffers");
#endif

	m_pID3D10Device->PSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::IASetInputLayout(ID3D10InputLayout *pInputLayout)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("IASetInputLayout");
#endif

	m_pID3D10Device->IASetInputLayout(pInputLayout);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::IASetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppVertexBuffers, const UINT *pStrides, const UINT *pOffsets)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("IASetVertexBuffers");
#endif

	m_pID3D10Device->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::IASetIndexBuffer(ID3D10Buffer *pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("IASetIndexBuffer");
#endif

	m_pID3D10Device->IASetIndexBuffer(pIndexBuffer, Format, Offset);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::DrawIndexedInstanced(UINT IndexCountPerInstance, UINT InstanceCount, UINT StartIndexLocation, INT BaseVertexLocation, UINT StartInstanceLocation)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("DrawIndexedInstanced");
#endif

	m_pID3D10Device->DrawIndexedInstanced(IndexCountPerInstance, InstanceCount, StartIndexLocation, BaseVertexLocation, StartInstanceLocation);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::DrawInstanced(UINT VertexCountPerInstance, UINT InstanceCount, UINT StartVertexLocation, UINT StartInstanceLocation)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("DrawInstanced");
#endif

	m_pID3D10Device->DrawInstanced(VertexCountPerInstance, InstanceCount, StartVertexLocation, StartInstanceLocation);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::GSSetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer *const *ppConstantBuffers)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GSSetConstantBuffers");
#endif

	m_pID3D10Device->GSSetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::GSSetShader(ID3D10GeometryShader *pShader)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GSSetShader");
#endif

	m_pID3D10Device->GSSetShader(pShader);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY Topology)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("IASetPrimitiveTopology");
#endif

	m_pID3D10Device->IASetPrimitiveTopology(Topology);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::VSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView *const *ppShaderResourceViews)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("VSSetShaderResources");
#endif

	m_pID3D10Device->VSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::VSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D10SamplerState *const *ppSamplers)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("VSSetSamplers");
#endif

	m_pID3D10Device->VSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::SetPredication(ID3D10Predicate *pPredicate, BOOL PredicateValue)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("SetPredication");
#endif

	m_pID3D10Device->SetPredication(pPredicate, PredicateValue);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::GSSetShaderResources(UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView *const *ppShaderResourceViews)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GSSetShaderResources");
#endif

	m_pID3D10Device->GSSetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::GSSetSamplers(UINT StartSlot, UINT NumSamplers, ID3D10SamplerState *const *ppSamplers)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GSSetSamplers");
#endif

	m_pID3D10Device->GSSetSamplers(StartSlot, NumSamplers, ppSamplers);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::OMSetRenderTargets(UINT NumViews, ID3D10RenderTargetView *const *ppRenderTargetViews, ID3D10DepthStencilView *pDepthStencilView)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("OMSetRenderTargets");
#endif

	m_pID3D10Device->OMSetRenderTargets(NumViews, ppRenderTargetViews, pDepthStencilView);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::OMSetBlendState(ID3D10BlendState *pBlendState, const FLOAT BlendFactor[4], UINT SampleMask)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("OMSetBlendState");
#endif

	m_pID3D10Device->OMSetBlendState(pBlendState, BlendFactor, SampleMask);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::OMSetDepthStencilState(ID3D10DepthStencilState *pDepthStencilState, UINT StencilRef)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("OMSetDepthStencilState");
#endif

	m_pID3D10Device->OMSetDepthStencilState(pDepthStencilState, StencilRef);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::SOSetTargets(UINT NumBuffers, ID3D10Buffer *const *ppSOTargets, const UINT *pOffsets)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("SOSetTargets");
#endif

	m_pID3D10Device->SOSetTargets(NumBuffers, ppSOTargets, pOffsets);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::DrawAuto()
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("DrawAuto");
#endif

	m_pID3D10Device->DrawAuto();
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::RSSetState(ID3D10RasterizerState *pRasterizerState)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("RSSetState");
#endif

	m_pID3D10Device->RSSetState(pRasterizerState);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::RSSetViewports(UINT NumViewports, const D3D10_VIEWPORT *pViewports)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("RSSetViewports");
#endif

	m_pID3D10Device->RSSetViewports(NumViewports, pViewports);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::RSSetScissorRects(UINT NumRects, const D3D10_RECT *pRects)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("RSSetScissorRects");
#endif

	m_pID3D10Device->RSSetScissorRects(NumRects, pRects);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::CopySubresourceRegion(ID3D10Resource *pDstResource, UINT DstSubresource, UINT DstX, UINT DstY, UINT DstZ, ID3D10Resource *pSrcResource, UINT SrcSubresource, const D3D10_BOX *pSrcBox)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CopySubresourceRegion");
#endif

	m_pID3D10Device->CopySubresourceRegion(pDstResource, DstSubresource, DstX, DstY, DstZ, pSrcResource, SrcSubresource, pSrcBox);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::CopyResource(ID3D10Resource *pDstResource, ID3D10Resource *pSrcResource)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CopyResource");
#endif

	m_pID3D10Device->CopyResource(pDstResource, pSrcResource);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::UpdateSubresource(ID3D10Resource *pDstResource, UINT DstSubresource, const D3D10_BOX *pDstBox, const void *pSrcData, UINT SrcRowPitch, UINT SrcDepthPitch)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("UpdateSubresource");
#endif

	m_pID3D10Device->UpdateSubresource(pDstResource, DstSubresource, pDstBox, pSrcData, SrcRowPitch, SrcDepthPitch);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::ClearRenderTargetView(ID3D10RenderTargetView *pRenderTargetView, const FLOAT ColorRGBA[4])
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("ClearRenderTargetView");
#endif

	m_pID3D10Device->ClearRenderTargetView(pRenderTargetView, ColorRGBA);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::ClearDepthStencilView(ID3D10DepthStencilView *pDepthStencilView, UINT ClearFlags, FLOAT Depth, UINT8 Stencil)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("ClearDepthStencilView");
#endif

	m_pID3D10Device->ClearDepthStencilView(pDepthStencilView, ClearFlags, Depth, Stencil);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::GenerateMips(ID3D10ShaderResourceView *pShaderResourceView)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GenerateMips");
#endif

	m_pID3D10Device->GenerateMips(pShaderResourceView);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::ResolveSubresource(ID3D10Resource *pDstResource, UINT DstSubresource, ID3D10Resource *pSrcResource, UINT SrcSubresource, DXGI_FORMAT Format)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("ResolveSubresource");
#endif

	m_pID3D10Device->ResolveSubresource(pDstResource, DstSubresource, pSrcResource, SrcSubresource, Format);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::VSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("VSGetConstantBuffers");
#endif

	m_pID3D10Device->VSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::PSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("PSGetShaderResources");
#endif

	m_pID3D10Device->PSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::PSGetShader(ID3D10PixelShader **ppPixelShader)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("PSGetShader");
#endif

	m_pID3D10Device->PSGetShader(ppPixelShader);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::PSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("PSGetSamplers");
#endif

	m_pID3D10Device->PSGetSamplers(StartSlot, NumSamplers, ppSamplers);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::VSGetShader(ID3D10VertexShader **ppVertexShader)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("VSGetShader");
#endif

	m_pID3D10Device->VSGetShader(ppVertexShader);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::PSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("PSGetConstantBuffers");
#endif

	m_pID3D10Device->PSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::IAGetInputLayout(ID3D10InputLayout **ppInputLayout)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("IAGetInputLayout");
#endif

	m_pID3D10Device->IAGetInputLayout(ppInputLayout);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::IAGetVertexBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppVertexBuffers, UINT *pStrides, UINT *pOffsets)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("IAGetVertexBuffers");
#endif

	m_pID3D10Device->IAGetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::IAGetIndexBuffer(ID3D10Buffer **pIndexBuffer, DXGI_FORMAT *Format, UINT *Offset)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("IAGetIndexBuffer");
#endif

	m_pID3D10Device->IAGetIndexBuffer(pIndexBuffer, Format, Offset);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::GSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GSGetConstantBuffers");
#endif

	m_pID3D10Device->GSGetConstantBuffers(StartSlot, NumBuffers, ppConstantBuffers);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::GSGetShader(ID3D10GeometryShader **ppGeometryShader)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GSGetShader");
#endif

	m_pID3D10Device->GSGetShader(ppGeometryShader);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::IAGetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY *pTopology)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("IAGetPrimitiveTopology");
#endif

	m_pID3D10Device->IAGetPrimitiveTopology(pTopology);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::VSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("VSGetShaderResources");
#endif

	m_pID3D10Device->VSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::VSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("VSGetSamplers");
#endif

	m_pID3D10Device->VSGetSamplers(StartSlot, NumSamplers, ppSamplers);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::GetPredication(ID3D10Predicate **ppPredicate, BOOL *pPredicateValue)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetPredication");
#endif

	m_pID3D10Device->GetPredication(ppPredicate, pPredicateValue);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::GSGetShaderResources(UINT StartSlot, UINT NumViews, ID3D10ShaderResourceView **ppShaderResourceViews)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GSGetShaderResources");
#endif

	m_pID3D10Device->GSGetShaderResources(StartSlot, NumViews, ppShaderResourceViews);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::GSGetSamplers(UINT StartSlot, UINT NumSamplers, ID3D10SamplerState **ppSamplers)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GSGetSamplers");
#endif

	m_pID3D10Device->GSGetSamplers(StartSlot, NumSamplers, ppSamplers);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::OMGetRenderTargets(UINT NumViews, ID3D10RenderTargetView **ppRenderTargetViews, ID3D10DepthStencilView **ppDepthStencilView)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("OMGetRenderTargets");
#endif

	m_pID3D10Device->OMGetRenderTargets(NumViews, ppRenderTargetViews, ppDepthStencilView);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::OMGetBlendState(ID3D10BlendState **ppBlendState, FLOAT BlendFactor[4], UINT *pSampleMask)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("OMGetBlendState");
#endif

	m_pID3D10Device->OMGetBlendState(ppBlendState, BlendFactor, pSampleMask);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::OMGetDepthStencilState(ID3D10DepthStencilState **ppDepthStencilState, UINT *pStencilRef)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("OMGetDepthStencilState");
#endif

	m_pID3D10Device->OMGetDepthStencilState(ppDepthStencilState, pStencilRef);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::SOGetTargets(UINT NumBuffers, ID3D10Buffer **ppSOTargets, UINT *pOffsets)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("SOGetTargets");
#endif

	m_pID3D10Device->SOGetTargets(NumBuffers, ppSOTargets, pOffsets);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::RSGetState(ID3D10RasterizerState **ppRasterizerState)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("RSGetState");
#endif

	m_pID3D10Device->RSGetState(ppRasterizerState);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::RSGetViewports(UINT *NumViewports, D3D10_VIEWPORT *pViewports)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("RSGetViewports");
#endif

	m_pID3D10Device->RSGetViewports(NumViewports, pViewports);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::RSGetScissorRects(UINT *NumRects, D3D10_RECT *pRects)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("RSGetScissorRects");
#endif

	m_pID3D10Device->RSGetScissorRects(NumRects, pRects);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::GetDeviceRemovedReason()
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetDeviceRemovedReason");
#endif

	return m_pID3D10Device->GetDeviceRemovedReason();
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::SetExceptionMode(UINT RaiseFlags)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("SetExceptionMode");
#endif

	return m_pID3D10Device->SetExceptionMode(RaiseFlags);
}

/**
* Base functionality.
***/
UINT PCL_ID3D10Device::GetExceptionMode()
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetExceptionMode");
#endif

	return m_pID3D10Device->GetExceptionMode();
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::GetPrivateData(REFGUID guid,__inout UINT *pDataSize, void *pData)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetPrivateData");
#endif

	return m_pID3D10Device->GetPrivateData(guid, pDataSize, pData);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::SetPrivateData(REFGUID guid, UINT DataSize, const void *pData)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("SetPrivateData");
#endif

	return m_pID3D10Device->SetPrivateData(guid, DataSize, pData);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::SetPrivateDataInterface(REFGUID guid, const IUnknown *pData)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("SetPrivateDataInterface");
#endif

	return m_pID3D10Device->SetPrivateDataInterface(guid, pData);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::ClearState()
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("ClearState");
#endif

	m_pID3D10Device->ClearState();
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::Flush()
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("Flush");
#endif

	m_pID3D10Device->Flush();
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateBuffer(const D3D10_BUFFER_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Buffer **ppBuffer)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateBuffer");
#endif

	return m_pID3D10Device->CreateBuffer(pDesc, pInitialData, ppBuffer);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateTexture1D(const D3D10_TEXTURE1D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture1D **ppTexture1D)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateTexture1D");
#endif

	return m_pID3D10Device->CreateTexture1D(pDesc, pInitialData, ppTexture1D);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateTexture2D(const D3D10_TEXTURE2D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture2D **ppTexture2D)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateTexture2D");
#endif

	return m_pID3D10Device->CreateTexture2D(pDesc, pInitialData, ppTexture2D);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateTexture3D(const D3D10_TEXTURE3D_DESC *pDesc, const D3D10_SUBRESOURCE_DATA *pInitialData, ID3D10Texture3D **ppTexture3D)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateTexture3D");
#endif

	return m_pID3D10Device->CreateTexture3D(pDesc, pInitialData, ppTexture3D);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateShaderResourceView(ID3D10Resource *pResource, const D3D10_SHADER_RESOURCE_VIEW_DESC *pDesc, ID3D10ShaderResourceView **ppSRView)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateShaderResourceView");
#endif

	return m_pID3D10Device->CreateShaderResourceView(pResource, pDesc, ppSRView);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateRenderTargetView(ID3D10Resource *pResource, const D3D10_RENDER_TARGET_VIEW_DESC *pDesc, ID3D10RenderTargetView **ppRTView)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateRenderTargetView");
#endif

	return m_pID3D10Device->CreateRenderTargetView(pResource, pDesc, ppRTView);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateDepthStencilView(ID3D10Resource *pResource, const D3D10_DEPTH_STENCIL_VIEW_DESC *pDesc, ID3D10DepthStencilView **ppDepthStencilView)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateDepthStencilView");
#endif

	return m_pID3D10Device->CreateDepthStencilView(pResource, pDesc, ppDepthStencilView);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateInputLayout(const D3D10_INPUT_ELEMENT_DESC *pInputElementDescs, UINT NumElements, const void *pShaderBytecodeWithInputSignature, SIZE_T BytecodeLength, ID3D10InputLayout **ppInputLayout)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateInputLayout");
#endif

	return m_pID3D10Device->CreateInputLayout(pInputElementDescs, NumElements, pShaderBytecodeWithInputSignature, BytecodeLength, ppInputLayout);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateVertexShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10VertexShader **ppVertexShader)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateVertexShader");
#endif

	return m_pID3D10Device->CreateVertexShader(pShaderBytecode, BytecodeLength, ppVertexShader);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateGeometryShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10GeometryShader **ppGeometryShader)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateGeometryShader");
#endif

	return m_pID3D10Device->CreateGeometryShader(pShaderBytecode, BytecodeLength, ppGeometryShader);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateGeometryShaderWithStreamOutput(const void *pShaderBytecode, SIZE_T BytecodeLength, const D3D10_SO_DECLARATION_ENTRY *pSODeclaration, UINT NumEntries, UINT OutputStreamStride, ID3D10GeometryShader **ppGeometryShader)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateGeometryShaderWithStreamOutput");
#endif

	return m_pID3D10Device->CreateGeometryShaderWithStreamOutput(pShaderBytecode, BytecodeLength, pSODeclaration, NumEntries, OutputStreamStride, ppGeometryShader);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreatePixelShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10PixelShader **ppPixelShader)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreatePixelShader");
#endif

	return m_pID3D10Device->CreatePixelShader(pShaderBytecode, BytecodeLength, ppPixelShader);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateBlendState(const D3D10_BLEND_DESC *pBlendStateDesc, ID3D10BlendState **ppBlendState)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateBlendState");
#endif

	return m_pID3D10Device->CreateBlendState(pBlendStateDesc, ppBlendState);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateDepthStencilState(const D3D10_DEPTH_STENCIL_DESC *pDepthStencilDesc, ID3D10DepthStencilState **ppDepthStencilState)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateDepthStencilState");
#endif

	return m_pID3D10Device->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateRasterizerState(const D3D10_RASTERIZER_DESC *pRasterizerDesc, ID3D10RasterizerState **ppRasterizerState)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateRasterizerState");
#endif

	return m_pID3D10Device->CreateRasterizerState(pRasterizerDesc, ppRasterizerState);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateSamplerState(const D3D10_SAMPLER_DESC *pSamplerDesc, ID3D10SamplerState **ppSamplerState)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateSamplerState");
#endif

	return m_pID3D10Device->CreateSamplerState(pSamplerDesc, ppSamplerState);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateQuery(const D3D10_QUERY_DESC *pQueryDesc, ID3D10Query **ppQuery)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateQuery");
#endif

	return m_pID3D10Device->CreateQuery(pQueryDesc, ppQuery);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreatePredicate(const D3D10_QUERY_DESC *pPredicateDesc, ID3D10Predicate **ppPredicate)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreatePredicate");
#endif

	return m_pID3D10Device->CreatePredicate(pPredicateDesc, ppPredicate);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CreateCounter(const D3D10_COUNTER_DESC *pCounterDesc, ID3D10Counter **ppCounter)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CreateCounter");
#endif

	return m_pID3D10Device->CreateCounter(pCounterDesc, ppCounter);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CheckFormatSupport(DXGI_FORMAT Format, UINT *pFormatSupport)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CheckFormatSupport");
#endif

	return m_pID3D10Device->CheckFormatSupport(Format, pFormatSupport);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CheckMultisampleQualityLevels(DXGI_FORMAT Format, UINT SampleCount, UINT *pNumQualityLevels)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CheckMultisampleQualityLevels");
#endif

	return m_pID3D10Device->CheckMultisampleQualityLevels(Format, SampleCount, pNumQualityLevels);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::CheckCounterInfo(D3D10_COUNTER_INFO *pCounterInfo)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CheckCounterInfo");
#endif

	m_pID3D10Device->CheckCounterInfo(pCounterInfo);
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::CheckCounter(const D3D10_COUNTER_DESC *pDesc, D3D10_COUNTER_TYPE *pType, UINT *pActiveCounters, LPSTR szName, UINT *pNameLength, LPSTR szUnits, UINT *pUnitsLength, LPSTR szDescription, UINT *pDescriptionLength)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("CheckCounter");
#endif

	return m_pID3D10Device->CheckCounter(pDesc, pType, pActiveCounters, szName, pNameLength, szUnits, pUnitsLength, szDescription, pDescriptionLength);
}

/**
* Base functionality.
***/
UINT PCL_ID3D10Device::GetCreationFlags()
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetCreationFlags");
#endif

	return m_pID3D10Device->GetCreationFlags();
}

/**
* Base functionality.
***/
HRESULT PCL_ID3D10Device::OpenSharedResource(HANDLE hResource, REFIID ReturnedInterface, void **ppResource)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("OpenSharedResource");
#endif

	return m_pID3D10Device->OpenSharedResource(hResource, ReturnedInterface, ppResource);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::SetTextFilterSize(UINT Width, UINT Height)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("SetTextFilterSize");
#endif

	m_pID3D10Device->SetTextFilterSize(Width, Height);
}

/**
* Base functionality.
***/
void PCL_ID3D10Device::GetTextFilterSize(UINT *pWidth, UINT *pHeight)
{
#ifdef AQU_DEBUG_OUTPUT
	OutputDebugString("GetTextFilterSize");
#endif

	m_pID3D10Device->GetTextFilterSize(pWidth, pHeight);
}