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

#ifndef D3D9PROXYVERTEXSHADER_H_INCLUDED
#define D3D9PROXYVERTEXSHADER_H_INCLUDED

#include <d3d9.h>
#include <memory>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include "Direct3DVertexShader9.h"
#include "Direct3DDevice9.h"
#include "ShaderRegisters.h"
#include "D3DProxyDevice.h"
#include "ShaderModificationRepository.h"


class D3DProxyDevice;
class ShaderModificationRepository;

class D3D9ProxyVertexShader : public BaseDirect3DVertexShader9
{
public:
	D3D9ProxyVertexShader(IDirect3DVertexShader9* pActualVertexShader, D3DProxyDevice* pOwningDevice, ShaderModificationRepository* pModLoader);
	virtual ~D3D9ProxyVertexShader();

	std::map<UINT, StereoShaderConstant<>>* ModifiedConstants();


protected:

	IDirect3DDevice9* m_pActualDevice;

	std::map<UINT, StereoShaderConstant<>> m_modifiedConstants;
};

#endif
