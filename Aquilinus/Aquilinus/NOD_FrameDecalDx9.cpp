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
#include"NOD_FrameDecalDx9.h"
#include"ITA_D3D9Interfaces.h"
#include"VMT_IDirect3DDevice9.h"
#include"VMT_IDirect3DSwapchain9.h"

/**
* Constructor. Create texture output commander.
***/
NOD_FrameDecalDx9::NOD_FrameDecalDx9(LONG nX, LONG nY) : NOD_Basic(nX, nY, 150, 80),
	m_pcFrameSurface9(nullptr),
	m_pcFrameTexture9(nullptr)
{
	m_eNodeProvokingType = AQU_NodeProvokingType::Both;
	m_bReturn = false;
	m_paDecommanders.clear();
	m_paCommanders.clear();
	m_szTitle = L"Frame Decal Dx9";

	NOD_Commander* pC = new NOD_Commander();
	pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_IDIRECT3DTEXTURE9;
	pC->m_szTitle = L"FrameTexture";
	pC->m_pOutput = (void*)&m_pcFrameTexture9;
	pC->m_paDecommanders.clear();
	m_paCommanders.push_back(pC);

	ZeroMemory(&m_sDesc, sizeof(D3DSURFACE_DESC));
}

/**
* Destructor.
***/
NOD_FrameDecalDx9::~NOD_FrameDecalDx9()
{
	if (m_pcFrameSurface9) 
	{
		m_pcFrameSurface9->Release();
		m_pcFrameSurface9 = nullptr;
	}
	if (m_pcFrameTexture9) 
	{
		m_pcFrameTexture9->Release();
		m_pcFrameTexture9 = nullptr;
	}
}

/**
* This node supports IDirect3DDevice9->Present() and IDirect3DSwapChain9->Present().
***/
bool NOD_FrameDecalDx9::SupportsD3DMethod(int eD3D, int eD3DInterface, int eD3DMethod)
{
	if ((eD3D >= (int)AQU_Direct3DVersion::DirectX_9_0) &&
		(eD3D <= (int)AQU_Direct3DVersion::DirectX_9_29))
	{
		// return true if d3d, device or swapchain, and present()
		if (((eD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9) &&
			(eD3DMethod == (int)VMT_IDIRECT3DDEVICE9::Present)) || 
			((eD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSwapChain9) &&
			(eD3DMethod == (int)VMT_IDIRECT3DSWAPCHAIN9::Present)))
			return true;
	}
	return false;
}

/**
* Get the backbuffer and stretch the backbuffer surface to a texture surface.
***/
void* NOD_FrameDecalDx9::Provoke(void* pcThis, int eD3D, int eD3DInterface, int eD3DMethod, std::vector<NOD_Basic*>* ppaNodes)
{
	// cast device
	LPDIRECT3DDEVICE9 pcDevice = nullptr;
	bool bReleaseDevice = false;
	if (eD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9)
	{
		pcDevice = (LPDIRECT3DDEVICE9)pcThis;
	}
	else if (eD3DInterface == (int)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSwapChain9)
	{
		LPDIRECT3DSWAPCHAIN9 pcSwapChain = (LPDIRECT3DSWAPCHAIN9)pcThis;
		if (!pcSwapChain) 
		{
			OutputDebugString(L"FrameDecalDx9 Node : No swapchain !");
			return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
		}
		pcSwapChain->GetDevice(&pcDevice);
		bReleaseDevice = true;
	}
	if (!pcDevice)
	{
		OutputDebugString(L"FrameDecalDx9 Node : No device !");
		return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
	}

	// create frame texture and surface (if not created)
	if ((!m_pcFrameTexture9) && (m_sDesc.Width > 0))
	{
		pcDevice->CreateTexture(m_sDesc.Width, m_sDesc.Height, 0, D3DUSAGE_RENDERTARGET, m_sDesc.Format, D3DPOOL_DEFAULT, &m_pcFrameTexture9, NULL);
		m_pcFrameTexture9->GetSurfaceLevel(0, &m_pcFrameSurface9);
	}

	try 
	{
		// get back buffer
		IDirect3DSurface9* pcBackBuffer = nullptr;
		pcDevice->GetBackBuffer(0, 0, D3DBACKBUFFER_TYPE_MONO, &pcBackBuffer);

		if (pcBackBuffer)
		{
			// create frame texture and surface (if not created)
			if (!m_pcFrameTexture9)
			{
				pcBackBuffer->GetDesc(&m_sDesc);
			}
			else
			{
				// stretch back buffer to texture
				pcDevice->StretchRect(pcBackBuffer, NULL, m_pcFrameSurface9, NULL, D3DTEXF_NONE);

				// release back buffer
				pcBackBuffer->Release();
			}
		}
	}
	catch (std::out_of_range) 
	{
		OutputDebugString(L"NOD_FrameDecalDx9 : Failed to get back buffer !");
	}

	// release device if provided by swapchain
	if (bReleaseDevice) pcDevice->Release();

	return NOD_Basic::Provoke(pcThis, eD3D, eD3DInterface, eD3DMethod, ppaNodes);
}