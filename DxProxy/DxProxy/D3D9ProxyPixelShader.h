/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyPixelShader.h> and
Class <D3D9ProxyPixelShader> :
Copyright (C) 2013 Denis Reischl

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

#ifndef D3D9PROXYPIXELSHADER_H_INCLUDED
#define D3D9PROXYPIXELSHADER_H_INCLUDED

#include <d3d9.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "Direct3DPixelShader9.h"
#include "Direct3DDevice9.h"
#include "ShaderRegisters.h"
#include "D3DProxyDevice.h"
#include "ShaderModificationRepository.h"


class D3DProxyDevice;
class ShaderModificationRepository;

/**
*  Direct 3D proxy pixel shader class.
*  Overwrites BaseDirect3DPixelShader9 and handles modified constants.
*/
class D3D9ProxyPixelShader : public BaseDirect3DPixelShader9
{
public:	
	D3D9ProxyPixelShader(IDirect3DPixelShader9* pActualPixelShader, D3DProxyDevice* pOwningDevice, ShaderModificationRepository* pModLoader);
	virtual ~D3D9ProxyPixelShader();

	/*** D3D9ProxyPixelShader public methods ***/
	std::map<UINT, StereoShaderConstant<>>* ModifiedConstants();
protected:
	/**
	* Currently not used actual owning device.
	***/
	IDirect3DDevice9* m_pActualDevice;
	/**
	* Modified shader constants.
	* <StartRegister, StereoShaderConstant<>>
	* @see StereoShaderConstant
	***/
	std::map<UINT, StereoShaderConstant<>> m_modifiedConstants;
};
#endif