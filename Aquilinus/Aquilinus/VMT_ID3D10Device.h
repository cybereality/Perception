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
#ifndef VMT_ID3D10DEVICE_ENUM
#define VMT_ID3D10DEVICE_ENUM

#include "VMT_IUnknown.h"

namespace VMT_ID3D10DEVICE
{

	/**
	* Virtual Methods Table for ID3D10Device.
	***/
	enum VMT_ID3D10Device
	{
		/*** ID3D10Device methods ***/
		VSSetConstantBuffers                  = 3,
		PSSetShaderResources                  = 4,
		PSSetShader                           = 5,
		PSSetSamplers                         = 6,
		VSSetShader                           = 7,
		DrawIndexed                           = 8,
		Draw                                  = 9,
		PSSetConstantBuffers                  = 10,
		IASetInputLayout                      = 11,
		IASetVertexBuffers                    = 12,
		IASetIndexBuffer                      = 13,
		DrawIndexedInstanced                  = 14,
		DrawInstanced                         = 15,
		GSSetConstantBuffers                  = 16,
		GSSetShader                           = 17,
		IASetPrimitiveTopology                = 18,
		VSSetShaderResources                  = 19,
		VSSetSamplers                         = 20,
		SetPredication                        = 21,
		GSSetShaderResources                  = 22,
		GSSetSamplers                         = 23,
		OMSetRenderTargets                    = 24,
		OMSetBlendState                       = 25,
		OMSetDepthStencilState                = 26,
		SOSetTargets                          = 27,
		DrawAuto                              = 28,
		RSSetState                            = 29,
		RSSetViewports                        = 30,
		RSSetScissorRects                     = 31,
		CopySubresourceRegion                 = 32,
		CopyResource                          = 33,
		UpdateSubresource                     = 34,
		ClearRenderTargetView                 = 35,
		ClearDepthStencilView                 = 36,
		GenerateMips                          = 37,
		ResolveSubresource                    = 38,
		VSGetConstantBuffers                  = 39,
		PSGetShaderResources                  = 40,
		PSGetShader                           = 41,
		PSGetSamplers                         = 42,
		VSGetShader                           = 43,
		PSGetConstantBuffers                  = 44,
		IAGetInputLayout                      = 45,
		IAGetVertexBuffers                    = 46,
		IAGetIndexBuffer                      = 47,
		GSGetConstantBuffers                  = 48,
		GSGetShader                           = 49,
		IAGetPrimitiveTopology                = 50,
		VSGetShaderResources                  = 51,
		VSGetSamplers                         = 52,
		GetPredication                        = 53,
		GSGetShaderResources                  = 54,
		GSGetSamplers                         = 55,
		OMGetRenderTargets                    = 56,
		OMGetBlendState                       = 57,
		OMGetDepthStencilState                = 58,
		SOGetTargets                          = 59,
		RSGetState                            = 60,
		RSGetViewports                        = 61,
		RSGetScissorRects                     = 62,
		GetDeviceRemovedReason                = 63,
		SetExceptionMode                      = 64,
		GetExceptionMode                      = 65,
		GetPrivateData                        = 66,
		SetPrivateData                        = 67,
		SetPrivateDataInterface               = 68,
		ClearState                            = 69,
		Flush                                 = 70,
		CreateBuffer                          = 71,
		CreateTexture1D                       = 72,
		CreateTexture2D                       = 73,
		CreateTexture3D                       = 74,
		CreateShaderResourceView              = 75,
		CreateRenderTargetView                = 76,
		CreateDepthStencilView                = 77,
		CreateInputLayout                     = 78,
		CreateVertexShader                    = 79,
		CreateGeometryShader                  = 80,
		CreateGeometryShaderWithStreamOutput  = 81,
		CreatePixelShader                     = 82,
		CreateBlendState                      = 83,
		CreateDepthStencilState               = 84,
		CreateRasterizerState                 = 85,
		CreateSamplerState                    = 86,
		CreateQuery                           = 87,
		CreatePredicate                       = 88,
		CreateCounter                         = 89,
		CheckFormatSupport                    = 90,
		CheckMultisampleQualityLevels         = 91,
		CheckCounterInfo                      = 92,
		CheckCounter                          = 93,
		GetCreationFlags                      = 94,
		OpenSharedResource                    = 95,
		SetTextFilterSize                     = 96,
		GetTextFilterSize                     = 97,
	};

}
#endif