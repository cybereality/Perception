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
	m_pActualVertexShader(pActualVertexShader),
	m_pOwningDevice(pOwningDevice),
	m_nRefCount(1),
	m_pActualDevice(pOwningDevice->getActual()),
	m_modifiedConstants()
{
	assert (pActualVertexShader != NULL);
	assert (pOwningDevice != NULL);
	
	pOwningDevice->AddRef();

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
{
	if(m_pActualVertexShader) 
		m_pActualVertexShader->Release();

	if (m_pOwningDevice)
		m_pOwningDevice->Release();
}





/**
* Base QueryInterface functionality. 
***/
HRESULT WINAPI D3D9ProxyVertexShader::QueryInterface(REFIID riid, LPVOID* ppv)
{
	return m_pActualVertexShader->QueryInterface(riid, ppv);
}

/**
* Base AddRef functionality.
***/
ULONG WINAPI D3D9ProxyVertexShader::AddRef()
{
	return ++m_nRefCount;
}

/**
* Base Release functionality.
***/
ULONG WINAPI D3D9ProxyVertexShader::Release()
{
	if(--m_nRefCount == 0)
	{
		delete this;
		return 0;
	}

	return m_nRefCount;
}

/**
* Base GetDevice functionality.
* TODO D3D behaviour. Docs don't have the notice that is usually there about a refcount increase
***/
HRESULT WINAPI D3D9ProxyVertexShader::GetDevice(IDirect3DDevice9** ppDevice)
{
	if (!m_pOwningDevice)
		return D3DERR_INVALIDCALL;
	else {
		*ppDevice = m_pOwningDevice;
		//m_pOwningDevice->AddRef(); //TODO Test this. Docs don't have the notice that is usually there about a refcount increase
		return D3D_OK;
	}
}

/**
* Base GetFunction functionality.
***/
HRESULT WINAPI D3D9ProxyVertexShader::GetFunction(void *pDate, UINT *pSizeOfData)
{
	return m_pActualVertexShader->GetFunction(pDate, pSizeOfData);
}

/**
* Returns the actual embedded shader pointer.
***/
IDirect3DVertexShader9* D3D9ProxyVertexShader::getActual()
{
	return m_pActualVertexShader;
}




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