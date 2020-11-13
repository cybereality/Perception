/********************************************************************
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
********************************************************************/
#ifndef VMT_ID3D11DEVICECONTEXT_ENUM
#define VMT_ID3D11DEVICECONTEXT_ENUM

#include"VMT_IUnknown.h"
#include"VMT_ID3D11DeviceChild.h"

namespace VMT_ID3D11DEVICECONTEXT
{

	/**
	* Virtual Methods Table for ID3D11DeviceContext.
	***/
	enum VMT_ID3D11DeviceContext
	{
		/*** ID3D11DeviceContext methods ***/
		VSSetConstantBuffers = 7,
		PSSetShaderResources = 8,
		PSSetShader = 9,
		PSSetSamplers = 10,
		VSSetShader = 11,
		DrawIndexed = 12,
		Draw = 13,
		Map = 14,
		Unmap = 15,
		PSSetConstantBuffers = 16,
		IASetInputLayout = 17,
		IASetVertexBuffers = 18,
		IASetIndexBuffer = 19,
		DrawIndexedInstanced = 20,
		DrawInstanced = 21,
		GSSetConstantBuffers = 22,
		GSSetShader = 23,
		IASetPrimitiveTopology = 24,
		VSSetShaderResources = 25,
		VSSetSamplers = 26,
		Begin = 27,
		End = 28,
		GetData = 29,
		SetPredication = 30,
		GSSetShaderResources = 31,
		GSSetSamplers = 32,
		OMSetRenderTargets = 33,
		OMSetRenderTargetsAndUnorderedAccessViews = 34,
		OMSetBlendState = 35,
		OMSetDepthStencilState = 36,
		SOSetTargets = 37,
		DrawAuto = 38,
		DrawIndexedInstancedIndirect = 39,
		DrawInstancedIndirect = 40,
		Dispatch = 41,
		DispatchIndirect = 42,
		RSSetState = 43,
		RSSetViewports = 44,
		RSSetScissorRects = 45,
		CopySubresourceRegion = 46,
		CopyResource = 47,
		UpdateSubresource = 48,
		CopyStructureCount = 49,
		ClearRenderTargetView = 50,
		ClearUnorderedAccessViewUint = 51,
		ClearUnorderedAccessViewFloat = 52,
		ClearDepthStencilView = 53,
		GenerateMips = 54,
		SetResourceMinLOD = 55,
		GetResourceMinLOD = 56,
		ResolveSubresource = 57,
		ExecuteCommandList = 58,
		HSSetShaderResources = 59,
		HSSetShader = 60,
		HSSetSamplers = 61,
		HSSetConstantBuffers = 62,
		DSSetShaderResources = 63,
		DSSetShader = 64,
		DSSetSamplers = 65,
		DSSetConstantBuffers = 66,
		CSSetShaderResources = 67,
		CSSetUnorderedAccessViews = 68,
		CSSetShader = 69,
		CSSetSamplers = 70,
		CSSetConstantBuffers = 71,
		VSGetConstantBuffers = 72,
		PSGetShaderResources = 73,
		PSGetShader = 74,
		PSGetSamplers = 75,
		VSGetShader = 76,
		PSGetConstantBuffers = 77,
		IAGetInputLayout = 78,
		IAGetVertexBuffers = 79,
		IAGetIndexBuffer = 80,
		GSGetConstantBuffers = 81,
		GSGetShader = 82,
		IAGetPrimitiveTopology = 83,
		VSGetShaderResources = 84,
		VSGetSamplers = 85,
		GetPredication = 86,
		GSGetShaderResources = 87,
		GSGetSamplers = 88,
		OMGetRenderTargets = 89,
		OMGetRenderTargetsAndUnorderedAccessViews = 90,
		OMGetBlendState = 91,
		OMGetDepthStencilState = 92,
		SOGetTargets = 93,
		RSGetState = 94,
		RSGetViewports = 95,
		RSGetScissorRects = 96,
		HSGetShaderResources = 97,
		HSGetShader = 98,
		HSGetSamplers = 99,
		HSGetConstantBuffers = 100,
		DSGetShaderResources = 101,
		DSGetShader = 102,
		DSGetSamplers = 103,
		DSGetConstantBuffers = 104,
		CSGetShaderResources = 105,
		CSGetUnorderedAccessViews = 106,
		CSGetShader = 107,
		CSGetSamplers = 108,
		CSGetConstantBuffers = 109,
		ClearState = 110,
		Flush = 111,
		GetType = 112,
		GetContextFlags = 113,
		FinishCommandList = 114,
	};
};
#endif