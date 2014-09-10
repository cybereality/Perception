/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyVertexShader.cpp> and
Class <D3D9ProxyVertexShader> :
Copyright (C) 2013 Chris Drain

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
	{
		m_modifiedConstants = pModLoader->GetModifiedConstantsF(pActualVertexShader);
		m_bSquishViewport = pModLoader->SquishViewportForShader(pActualVertexShader);
	}
	else m_bSquishViewport = false;
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

/**
* Returns true if viewport is to be squished for that shader.
***/
bool D3D9ProxyVertexShader::SquishViewport()
{
	return m_bSquishViewport;
}