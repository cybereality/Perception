/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez
Modifications Copyright (C) 2013 Chris Drain, Denis Reischl, Neil Schneider 
  and Joshua Brown 

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

#include "D3D9ProxyVertexShader.h"

/**
* Constructor.
* @param pModLoader Can be NULL (no modifications in this game profile).
***/
D3D9ProxyVertexShader::D3D9ProxyVertexShader(IDirect3DVertexShader9* pActualVertexShader, D3DProxyDevice *pOwningDevice, ShaderModificationRepository* pModLoader) :
	BaseDirect3DVertexShader9(pActualVertexShader, pOwningDevice),
	m_pActualDevice(pOwningDevice->getActual()),
	m_modifiedConstants()
{
	if (pModLoader)
		m_modifiedConstants = pModLoader->GetModifiedConstantsF(pActualVertexShader);
}

/**
* Empty destructor.
***/
D3D9ProxyVertexShader::~D3D9ProxyVertexShader()
{}

/**
* Returns modified constants pointer.
***/
std::map<UINT, StereoShaderConstant<float>>* D3D9ProxyVertexShader::ModifiedConstants()
{
	return &m_modifiedConstants;
}