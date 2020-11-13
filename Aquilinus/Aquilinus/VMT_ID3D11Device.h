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
#ifndef VMT_ID3D11DEVICE_ENUM
#define VMT_ID3D11DEVICE_ENUM

#include "VMT_IUnknown.h"

namespace VMT_ID3D11DEVICE
{

	/**
	* Virtual Methods Table for ID3D11Device.
	***/
	enum VMT_ID3D11Device
	{
		/*** ID3D11Device methods ***/     	
		CreateBuffer                         = 3,
		CreateTexture1D                      = 4,
		CreateTexture2D                      = 5,
		CreateTexture3D                      = 6,
		CreateShaderResourceView             = 7,
		CreateUnorderedAccessView            = 8,
		CreateRenderTargetView               = 9,
		CreateDepthStencilView               = 10,
		CreateInputLayout                    = 11,
		CreateVertexShader                   = 12,
		CreateGeometryShader                 = 13,
		CreateGeometryShaderWithStreamOutput = 14,
		CreatePixelShader                    = 15,
		CreateHullShader                     = 16,
		CreateDomainShader                   = 17,
		CreateComputeShader                  = 18,
		CreateClassLinkage                   = 19,
		CreateBlendState                     = 20,
		CreateDepthStencilState              = 21,
		CreateRasterizerState                = 22,
		CreateSamplerState                   = 23,
		CreateQuery                          = 24,
		CreatePredicate                      = 25,
		CreateCounter                        = 26,
		CreateDeferredContext                = 27,
		OpenSharedResource                   = 28,
		CheckFormatSupport                   = 29,
		CheckMultisampleQualityLevels        = 30,
		CheckCounterInfo                     = 31,
		CheckCounter                         = 32,
		CheckFeatureSupport                  = 33,
		GetPrivateData                       = 34,
		SetPrivateData                       = 35,
		SetPrivateDataInterface              = 36,
		GetFeatureLevel                      = 37,
		GetCreationFlags                     = 38,
		GetDeviceRemovedReason               = 39,
		GetImmediateContext                  = 40,
		SetExceptionMode                     = 41,
		GetExceptionMode                     = 42,
	};

}
#endif