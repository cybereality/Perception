/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Cinema - Virtual Reality Cinema Node Plugin
Copyright (C) 2014 Denis Reischl

File <VireioCinema.cpp> and
Class <VireioCinema> :
Copyright (C) 2015 Denis Reischl

The stub class <AQU_Nodus> is the only public class from the Aquilinus
repository and permitted to be used for open source plugins of any kind.
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

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

#include"VireioCinema.h"
#include"Shader_D3D9.h"
#include"Resources.h"

#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }
#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"- %u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"- %x", a); OutputDebugString(buf); }

#define INTERFACE_IDIRECT3DDEVICE9 8
#define INTERFACE_IDIRECT3DSWAPCHAIN9 15
#define	METHOD_IDIRECT3DDEVICE9_PRESENT 17
#define	METHOD_IDIRECT3DDEVICE9_ENDSCENE 42
#define	METHOD_IDIRECT3DSWAPCHAIN9_PRESENT 3

/**
* Constructor.
***/
VireioCinema::VireioCinema() :AQU_Nodus(),
m_vsCinema(nullptr),
m_vsCinemaTheatre(nullptr),
m_psCinema(nullptr),
m_psCinemaTheatre(nullptr),
m_ctPCinema(nullptr),
m_ctPCinemaTheatre(nullptr),
m_ctVCinema(nullptr),
m_ctVCinemaTheatre(nullptr),
m_pVBCinemaScreen(nullptr),
m_pVBCinemaTheatre(nullptr),
m_pIBCinemaTheatre(nullptr),
m_pTextureCinema(nullptr),
m_pTextureCinemaTheatre(nullptr),
m_ppfYaw(nullptr),
m_ppfPitch(nullptr),
m_ppfRoll(nullptr),
m_fYaw(0.0f),
m_fPitch(0.0f),
m_fRoll(0.0f),
m_ppfPositionX(nullptr),
m_ppfPositionY(nullptr),
m_ppfPositionZ(nullptr),
m_ppcFrameTexture(nullptr),
m_pStereoOutputLeft(nullptr),
m_pStereoOutputRight(nullptr),
m_pStereoOutputSurfaceLeft(nullptr),
m_pStereoOutputSurfaceRight(nullptr),
m_pcDepthStencilLeft(nullptr),
m_pcDepthStencilRight(nullptr),
m_bOculusTinyRoomMesh(false)
{
	ZeroMemory(&m_sPositionVector, sizeof(D3DVECTOR));
	ZeroMemory(&m_sViewOffsetLeft, sizeof(D3DVECTOR));
	ZeroMemory(&m_sViewOffsetRight, sizeof(D3DVECTOR));

	// set default aspect ratio
	m_fAspectRatio = 1920.0f / 1080.0f;
}

/**
* Destructor.
***/
VireioCinema::~VireioCinema()
{
	if (m_pStereoOutputSurfaceLeft)
	{
		m_pStereoOutputSurfaceLeft->Release();
		m_pStereoOutputSurfaceLeft = nullptr;
	}
	if (m_pStereoOutputSurfaceRight)
	{
		m_pStereoOutputSurfaceRight->Release();
		m_pStereoOutputSurfaceRight = nullptr;
	}
	if (m_pStereoOutputLeft)
	{
		m_pStereoOutputLeft->Release();
		m_pStereoOutputLeft = nullptr;
	}
	if (m_pStereoOutputRight)
	{
		m_pStereoOutputRight->Release();
		m_pStereoOutputRight = nullptr;
	}
}

/**
* Return the name of the Vireio Cinema node.
***/
const char* VireioCinema::GetNodeType()
{
	return "Vireio Cinema";
}

/**
* Returns a global unique identifier for the Vireio Cinema node.
***/
UINT VireioCinema::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 128
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Vireio Cinema node.
***/
LPWSTR VireioCinema::GetCategory()
{
	return L"Virtual Reality";
}

/**
* Returns a logo to be used for the Vireio Cinema node.
***/
HBITMAP VireioCinema::GetLogo()
{
	HMODULE hModule = GetModuleHandle(L"VireioCinema.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR VireioCinema::GetCommanderName(DWORD dwCommanderIndex)
{
	switch ((VRC_Commanders)dwCommanderIndex)
	{
		case VRC_Commanders::StereoTextureLeft:
			return L"Stereo Output Texture Left";
		case VRC_Commanders::StereoTextureRight:
			return L"Stereo Output Texture Right";
	}

	return L"";
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR VireioCinema::GetDecommanderName(DWORD dwDecommanderIndex)
{
	switch ((VRC_Decommanders)dwDecommanderIndex)
	{
		case VRC_Decommanders::Yaw:
			return L"Yaw";
		case VRC_Decommanders::Pitch:
			return L"Pitch";
		case VRC_Decommanders::Roll:
			return L"Roll";
		case VRC_Decommanders::PositionX:
			return L"Position X";
		case VRC_Decommanders::PositionY:
			return L"Position Y";
		case VRC_Decommanders::PositionZ:
			return L"Position Z";
		case VRC_Decommanders::FrameTexture:
			return L"Frame Texture";
		case VRC_Decommanders::ViewOffsetLeft:
			return L"View Offset Left";
		case VRC_Decommanders::ViewOffsetRight:
			return L"View Offset Right";
		case VRC_Decommanders::ResolutionWidth:
			return L"Resolution Width";
		case VRC_Decommanders::ResolutionHeight:
			return L"Resolution Height";
		case VRC_Decommanders::ProjectionLeft:
			return L"Projection Left";
		case VRC_Decommanders::ProjectionRight:
			return L"Projection Right";
	}

	return L"";
}

/**
* Returns the plug type for the requested commander.
***/
DWORD VireioCinema::GetCommanderType(DWORD dwCommanderIndex)
{
	return PNT_IDIRECT3DTEXTURE9_PLUG_TYPE;
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD VireioCinema::GetDecommanderType(DWORD dwDecommanderIndex)
{
	if (dwDecommanderIndex < 6)
		return PNT_FLOAT_PLUG_TYPE;
	else if (dwDecommanderIndex == 6)
		return PNT_IDIRECT3DTEXTURE9_PLUG_TYPE;
	else if (dwDecommanderIndex < 9)
		return PNT_VECTOR3F_PLUG_TYPE;
	else if (dwDecommanderIndex < 11)
		return INT_PLUG_TYPE;
	else return PNT_D3DMATRIX_PLUG_TYPE;
}

/**
* Provides the output pointer for the requested commander.
***/
void* VireioCinema::GetOutputPointer(DWORD dwCommanderIndex)
{
	switch ((VRC_Commanders)dwCommanderIndex)
	{
		case VRC_Commanders::StereoTextureLeft:
			return (void*)&m_pStereoOutputLeft;
		case VRC_Commanders::StereoTextureRight:
			return (void*)&m_pStereoOutputRight;
	}

	return nullptr;
}

/**
* Sets the input pointer for the requested decommander.
***/
void VireioCinema::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
	switch ((VRC_Decommanders)dwDecommanderIndex)
	{
		case VRC_Decommanders::Yaw:
			m_ppfYaw = (float**)pData;
			break;
		case VRC_Decommanders::Pitch:
			m_ppfPitch = (float**)pData;
			break;
		case VRC_Decommanders::Roll:
			m_ppfRoll = (float**)pData;
			break;
		case VRC_Decommanders::PositionX:
			m_ppfPositionX = (float**)pData;
			break;
		case VRC_Decommanders::PositionY:
			m_ppfPositionY = (float**)pData;
			break;
		case VRC_Decommanders::PositionZ:
			m_ppfPositionZ = (float**)pData;
			break;
		case VRC_Decommanders::FrameTexture:
			m_ppcFrameTexture = (LPDIRECT3DTEXTURE9*)pData;
			break;
		case VRC_Decommanders::ViewOffsetLeft:
			m_ppsViewOffsetLeft = (D3DVECTOR**)pData;
			break;
		case VRC_Decommanders::ViewOffsetRight:
			m_ppsViewOffsetRight = (D3DVECTOR**)pData;
			break;
		case VRC_Decommanders::ResolutionWidth:
			m_pnTexResolutionWidth = (int*)pData;
			break;
		case VRC_Decommanders::ResolutionHeight:
			m_pnTexResolutionHeight = (int*)pData;
			break;
		case VRC_Decommanders::ProjectionLeft:
			m_ppsProjectionLeft = (D3DMATRIX**)pData;
			break;
		case VRC_Decommanders::ProjectionRight:
			m_ppsProjectionRight = (D3DMATRIX**)pData;
			break;
	}
}

/**
* Vireio Cinema supports D3D 9 Present() and EndScene() calls.
***/
bool VireioCinema::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
	if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_9_0) &&
		(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_9_29))
	{
		if (((nD3DInterface == INTERFACE_IDIRECT3DDEVICE9) &&
			(nD3DMethod == METHOD_IDIRECT3DDEVICE9_PRESENT)) ||
			((nD3DInterface == INTERFACE_IDIRECT3DDEVICE9) &&
			(nD3DMethod == METHOD_IDIRECT3DDEVICE9_ENDSCENE)) ||
			((nD3DInterface == INTERFACE_IDIRECT3DSWAPCHAIN9) &&
			(nD3DMethod == METHOD_IDIRECT3DSWAPCHAIN9_PRESENT)))
			return true;
	}
	return false;
}

/**
* Virtual Cinema entry point.
***/
void* VireioCinema::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
	// get device 
	LPDIRECT3DDEVICE9 pcDevice = nullptr;
	bool bReleaseDevice = false;
	if (eD3DInterface == INTERFACE_IDIRECT3DDEVICE9)
	{
		pcDevice = (LPDIRECT3DDEVICE9)pThis;
	}
	else if (eD3DInterface == INTERFACE_IDIRECT3DSWAPCHAIN9)
	{
		LPDIRECT3DSWAPCHAIN9 pSwapChain = (LPDIRECT3DSWAPCHAIN9)pThis;
		if (!pSwapChain)
		{
			OutputDebugString(L"VireioCinema : No swapchain !");
			return nullptr;
		}
		pSwapChain->GetDevice(&pcDevice);
		bReleaseDevice = true;
	}
	if (!pcDevice)
	{
		OutputDebugString(L"VireioCinema Node : No device !");
		return nullptr;
	}

	// get data
	if (m_ppfYaw)
	if (*m_ppfYaw) m_fYaw = **m_ppfYaw;
	if (m_ppfPitch)
	if (*m_ppfPitch)  m_fPitch = **m_ppfPitch;
	if (m_ppfRoll)
	if (*m_ppfRoll)  m_fRoll = **m_ppfRoll;
	if (m_ppfPositionX)
	if (*m_ppfPositionX)  m_sPositionVector.x = **m_ppfPositionX;
	if (m_ppfPositionY)
	if (*m_ppfPositionY)  m_sPositionVector.y = **m_ppfPositionY;
	if (m_ppfPositionZ)
	if (*m_ppfPositionZ)  m_sPositionVector.z = **m_ppfPositionZ;
	if (m_ppsViewOffsetLeft)
	{
		if (*m_ppsViewOffsetLeft)
		{
			m_sViewOffsetLeft.x = (*m_ppsViewOffsetLeft)->x;
			m_sViewOffsetLeft.y = (*m_ppsViewOffsetLeft)->y;
			m_sViewOffsetLeft.z = (*m_ppsViewOffsetLeft)->z;
		}
	}
	if (m_ppsViewOffsetRight)
	{
		if (*m_ppsViewOffsetRight)
		{
			m_sViewOffsetLeft.x = (*m_ppsViewOffsetLeft)->x;
			m_sViewOffsetLeft.y = (*m_ppsViewOffsetLeft)->y;
			m_sViewOffsetLeft.z = (*m_ppsViewOffsetLeft)->z;
		}
	}

	// init all stuff
	Init(pcDevice);

	// ..and render
	if ((m_pVBCinemaTheatre) && (m_pVBCinemaScreen))
		Render(pcDevice);

	// release device if provided by swapchain
	if (bReleaseDevice) pcDevice->Release();

	return nullptr;
}

/**
* Inits all stuff.
***/
void VireioCinema::Init(LPDIRECT3DDEVICE9 pcDevice)
{
	// output textures...left
	if ((!m_pStereoOutputLeft) && (m_pnTexResolutionWidth) && (m_pnTexResolutionHeight))
	{
		if ((*m_pnTexResolutionWidth) && (*m_pnTexResolutionHeight))
		{
			INT32 nWidth = *m_pnTexResolutionWidth;
			INT32 nHeight = *m_pnTexResolutionHeight;

			// set same aspect ratio for left and right
			m_fAspectRatio = float(nWidth) / float(nHeight);

			// create left render target
			if (FAILED(pcDevice->CreateTexture((UINT)nWidth, (UINT)nHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pStereoOutputLeft, NULL)))
			{
				OutputDebugString(L"VRCinema Node : Failed to create left stereo output texture.");
				m_pStereoOutputLeft = nullptr;
			}
			else
			{
				// get surface
				m_pStereoOutputLeft->GetSurfaceLevel(0, &m_pStereoOutputSurfaceLeft);

				// create depth stencil left
				if (m_pcDepthStencilLeft)
				{
					m_pcDepthStencilLeft->Release();
					m_pcDepthStencilLeft = nullptr;
				}
				if (FAILED(pcDevice->CreateDepthStencilSurface((UINT)nWidth, (UINT)nHeight, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, false, &m_pcDepthStencilLeft, NULL)))
				{
					OutputDebugString(L"VRCinema Node : Failed to create left depth stencil !");
				}
			}
		}
	}
	else
	{
		if (!(*m_pnTexResolutionWidth) || !(*m_pnTexResolutionHeight))
		{
			SAFE_RELEASE(m_pStereoOutputSurfaceLeft);
			SAFE_RELEASE(m_pStereoOutputLeft);
			SAFE_RELEASE(m_pcDepthStencilLeft);
		}
	}

	// output textures...right
	if ((!m_pStereoOutputRight) && (m_pnTexResolutionWidth) && (m_pnTexResolutionHeight))
	{
		if ((*m_pnTexResolutionWidth) && (*m_pnTexResolutionHeight))
		{
			INT32 nWidth = *m_pnTexResolutionWidth;
			INT32 nHeight = *m_pnTexResolutionHeight;

			// set same aspect ratio for left and right
			m_fAspectRatio = float(nWidth) / float(nHeight);

			// create left render target
			if (FAILED(pcDevice->CreateTexture((UINT)nWidth, (UINT)nHeight, 1, D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pStereoOutputRight, NULL)))
			{
				OutputDebugString(L"VRCinema Node : Failed to create right stereo output texture.");
				m_pStereoOutputRight = nullptr;
			}
			else
			{
				// get surface
				m_pStereoOutputRight->GetSurfaceLevel(0, &m_pStereoOutputSurfaceRight);

				// create depth stencil right
				if (m_pcDepthStencilRight)
				{
					m_pcDepthStencilRight->Release();
					m_pcDepthStencilRight = nullptr;
				}
				if (FAILED(pcDevice->CreateDepthStencilSurface((UINT)nWidth, (UINT)nHeight, D3DFMT_D24X8, D3DMULTISAMPLE_NONE, 0, false, &m_pcDepthStencilRight, NULL)))
				{
					OutputDebugString(L"VRCinema Node : Failed to create right depth stencil !");
				}
			}
		}
	}
	else
	{
		if (!(*m_pnTexResolutionWidth) || !(*m_pnTexResolutionHeight))
		{
			SAFE_RELEASE(m_pStereoOutputSurfaceRight);
			SAFE_RELEASE(m_pStereoOutputRight);
			SAFE_RELEASE(m_pcDepthStencilRight);
		}
	}

	// pixel shader created ?
	if (!m_psCinema)
	{
		LPD3DXBUFFER pShader;

		// compile and create shader
		if (SUCCEEDED(D3DXCompileShader(PSSimplest, strlen(PSSimplest), NULL, NULL, "OurFirstPixelShader", "ps_3_0", NULL, &pShader, NULL, &m_ctPCinema)))
		{
			OutputDebugString(L"Pixel shader >Textured< compiled!");
			pcDevice->CreatePixelShader((DWORD*)pShader->GetBufferPointer(), &m_psCinema);
		}
	}
	if (!m_psCinemaTheatre)
	{
		LPD3DXBUFFER pShader;

		// compile and create shader
		if (SUCCEEDED(D3DXCompileShader(PSTexColor, strlen(PSTexColor), NULL, NULL, "TexColorPixelShader", "ps_3_0", NULL, &pShader, NULL, &m_ctPCinemaTheatre)))
		{
			OutputDebugString(L"Pixel shader >Textured+Colored< compiled!");
			pcDevice->CreatePixelShader((DWORD*)pShader->GetBufferPointer(), &m_psCinemaTheatre);
		}
	}

	// vertex shader created ?
	if (!m_vsCinema)
	{
		LPD3DXBUFFER pShader;

		// compile and create shader
		if (SUCCEEDED(D3DXCompileShader(VSSimplest, strlen(VSSimplest), NULL, NULL, "SimplestVertexShader", "vs_1_1", NULL, &pShader, NULL, &m_ctVCinema)))
		{
			OutputDebugString(L"Vertex shader >Textured< compiled!");
			pcDevice->CreateVertexShader((DWORD*)pShader->GetBufferPointer(), &m_vsCinema);
		}
	}
	if (!m_vsCinemaTheatre)
	{
		LPD3DXBUFFER pShader;

		// compile and create shader
		if (SUCCEEDED(D3DXCompileShader(VSTexColor, strlen(VSTexColor), NULL, NULL, "TexColorVertexShader", "vs_1_1", NULL, &pShader, NULL, &m_ctVCinemaTheatre)))
		{
			OutputDebugString(L"Vertex shader >Textured+Colored< compiled!");
			pcDevice->CreateVertexShader((DWORD*)pShader->GetBufferPointer(), &m_vsCinemaTheatre);
		}
	}

	// oculus room created ?
	if (!m_bOculusTinyRoomMesh)
	{
		GenerateOculusRoomTinyTextures();
		GenerateOculusRoomTinyMesh();
		m_bOculusTinyRoomMesh = true;
	}

	// texture created ?
	if (!m_pTextureCinema)
	{
		HMODULE hModule = GetModuleHandle(L"VireioCinema.dll");
		if (SUCCEEDED(D3DXCreateTextureFromResource(pcDevice, hModule, MAKEINTRESOURCE(IMG_LOGO01), &m_pTextureCinema)))
			OutputDebugString(L"Texture created !");
	}
	if (!m_pTextureCinemaTheatre)
	{
		if (SUCCEEDED(D3DXCreateTexture(pcDevice, 256, 256, 1, D3DUSAGE_DYNAMIC, D3DFORMAT::D3DFMT_A8R8G8B8, D3DPOOL_DEFAULT, &m_pTextureCinemaTheatre)))
		{
			D3DLOCKED_RECT rect;
			m_pTextureCinemaTheatre->LockRect(0, &rect, NULL, D3DLOCK_DISCARD);
			CopyMemory(rect.pBits, tex_pixels[0], 256 * 256 * sizeof(D3DCOLOR));
			m_pTextureCinemaTheatre->UnlockRect(0);
		}
	}

	// vertex buffer created ?
	if (!m_pVBCinemaScreen)
	{
		TexturedVertex asVertices[] =
		{
			{ -1.92f, -1.08f, 0.0f, 1.0f, 1.0f },
			{ 1.92f, -1.08f, 0.0f, 0.0f, 1.0f },
			{ 1.92f, 1.08f, 0.0f, 0.0f, 0.0f },
			{ -1.92f, 1.08f, 0.0f, 1.0f, 0.0f }
		};

		float fScale = 0.8f;
		D3DXVECTOR3 sTranslate = D3DXVECTOR3(0.0f, 2.0f, 1.0f);
		for (int i = 0; i < 4; i++)
		{
			// scale and translate
			asVertices[i].x *= fScale;
			asVertices[i].y *= fScale;
			asVertices[i].z *= fScale;

			asVertices[i].x += sTranslate.x;
			asVertices[i].y += sTranslate.y;
			asVertices[i].z += sTranslate.z;
		}

		// create
		if (FAILED(pcDevice->CreateVertexBuffer(4 * sizeof(TexturedVertex), 0, D3DFVF_XYZ | D3DFVF_TEX1, D3DPOOL_DEFAULT, &m_pVBCinemaScreen, NULL)))
		{
			OutputDebugString(L"Error while creating VertexBuffer!");
		}

		// lock and copy vertices
		PVOID pVertices;
		if (FAILED(m_pVBCinemaScreen->Lock(0, 4 * sizeof(TexturedVertex), (void**)&pVertices, 0)))
		{
			OutputDebugString(L"Error trying to lock!");
		}
		else
		{
			memcpy(pVertices, asVertices, 4 * sizeof(TexturedVertex));
			m_pVBCinemaScreen->Unlock();
		}
	}

	if (!m_pVBCinemaTheatre)
	{
		// create vertex buffer
		if (FAILED(pcDevice->CreateVertexBuffer(numVertices*sizeof(TexColoredVertex), 0, D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1, D3DPOOL_DEFAULT, &m_pVBCinemaTheatre, NULL)))
		{
			OutputDebugString(L"Error while creating VertexBuffer!");
		}

		// lock and copy vertices
		PVOID pVertices;
		if (FAILED(m_pVBCinemaTheatre->Lock(0, numVertices*sizeof(TexColoredVertex), (void**)&pVertices, 0)))
		{
			OutputDebugString(L"Error trying to lock!");
		}
		else
		{
			memcpy(pVertices, Vertices, numVertices*sizeof(TexColoredVertex));
			m_pVBCinemaTheatre->Unlock();
		}

		// create index buffer
		if (FAILED(pcDevice->CreateIndexBuffer(numIndices*sizeof(UINT16), 0, D3DFORMAT::D3DFMT_INDEX16, D3DPOOL_DEFAULT, &m_pIBCinemaTheatre, NULL)))
		{
			OutputDebugString(L"Error while creating IndexBuffer!");
		}

		// lock and copy indices
		PVOID pIndices;
		if (FAILED(m_pIBCinemaTheatre->Lock(0, numIndices*sizeof(UINT16), (void**)&pIndices, 0)))
		{
			OutputDebugString(L"Error trying to lock!");
		}
		else
		{
			memcpy(pIndices, Indices, numIndices*sizeof(UINT16));
			m_pIBCinemaTheatre->Unlock();
		}
	}
}

/**
* Renders the virtual cinema.
***/
void VireioCinema::Render(LPDIRECT3DDEVICE9 pcDevice)
{
	// render... first create matrices
	D3DXMATRIX matProjection;
	//D3DXMatrixPerspectiveFovLH(&matProjection, (float)(70.0f * 3.14f / 180.0f), m_fAspectRatio, 0.01f, 50.0f);
	D3DXMatrixPerspectiveFovLH(&matProjection, (float)D3DXToRadian(116.0), m_fAspectRatio, 0.1f, 30.0f);
	D3DXMATRIX matView;
	D3DXMatrixLookAtRH(&matView, /*CameraPos*/&D3DXVECTOR3(0, 2.1f, -0.5f), &D3DXVECTOR3(0, 2.0, 0), &D3DXVECTOR3(0, 1, 0));

	// is that right to add the offset at this point ?
	D3DXMATRIX matViewLeft, matViewRight, matTransLeft, matTransRight;
	D3DXMatrixTranslation(&matTransLeft, m_sViewOffsetLeft.x, m_sViewOffsetLeft.y, m_sViewOffsetLeft.z);
	D3DXMatrixTranslation(&matTransRight, m_sViewOffsetRight.x, m_sViewOffsetRight.y, m_sViewOffsetRight.z);
	matViewLeft = matTransLeft * matView;
	matViewRight = matTransRight * matView;

	// yaw pitch roll
	D3DXMATRIX matYaw;
	D3DXMatrixIdentity(&matYaw);
	D3DXMatrixRotationY(&matYaw, -m_fYaw);
	D3DXMATRIX matPitch;
	D3DXMatrixIdentity(&matPitch);
	D3DXMatrixRotationX(&matPitch, -m_fPitch);
	D3DXMATRIX matRoll;
	D3DXMatrixIdentity(&matRoll);
	D3DXMatrixRotationZ(&matRoll, -m_fRoll);

	// first, use standard IPD (0.064 meters)
	D3DXMATRIX matSeparationLeft, matSeparationRight;
	D3DXMatrixTranslation(&matSeparationLeft, 0.032f, 0, 0);
	D3DXMatrixTranslation(&matSeparationRight, -0.032f, 0, 0);

	pcDevice->BeginScene();

	// save states
	IDirect3DStateBlock9* pStateBlock;
	pcDevice->CreateStateBlock(D3DSBT_ALL, &pStateBlock);

	// set ALL render states to default
	SetAllRenderStatesDefault(pcDevice);

	// set states
	pcDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_SELECTARG1);
	pcDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
	pcDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_SELECTARG1);
	pcDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_CONSTANT);
	pcDevice->SetTextureStageState(0, D3DTSS_CONSTANT, 0xffffffff);
	pcDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pcDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);// D3DZB_TRUE);
	pcDevice->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);// TRUE);
	pcDevice->SetSamplerState(0, D3DSAMP_SRGBTEXTURE, 0);
	pcDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	pcDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	pcDevice->SetSamplerState(0, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
	pcDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_ANISOTROPIC);
	pcDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_ANISOTROPIC);
	pcDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

	pcDevice->SetVertexShader(m_vsCinema);
	pcDevice->SetPixelShader(m_psCinema);
	pcDevice->SetVertexDeclaration(NULL);

	// set stream source, FVF, texture
	pcDevice->SetStreamSource(0, m_pVBCinemaScreen, 0, sizeof(TexturedVertex));
	pcDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	if (m_ppcFrameTexture)
	{
		if (*m_ppcFrameTexture)
			pcDevice->SetTexture(0, *m_ppcFrameTexture);
		else
			pcDevice->SetTexture(0, m_pTextureCinema);
	}
	else
		pcDevice->SetTexture(0, m_pTextureCinema);

	// save render targets.. TODO !! get max render targets
	int nTargets = 4;
	LPDIRECT3DSURFACE9 pSurface[4];
	for (int i = 0; i < nTargets; i++)
		pcDevice->GetRenderTarget(i, &pSurface[i]);

	D3DCOLOR clearColor = D3DCOLOR_RGBA(0, 255, 0, 0);

	// set matrix
	if (m_ppsProjectionLeft)
	{
		if (*m_ppsProjectionLeft)
			CopyMemory(&matProjection, (void*)(*m_ppsProjectionLeft), sizeof(D3DMATRIX));
	}
	m_ctVCinema->SetMatrix(pcDevice, "xViewProjection", &(matViewLeft * matYaw * matPitch * matRoll * matSeparationLeft * matProjection));
	// pcDevice->SetVertexShaderConstantF(0, &matProjection.m[0][0], 4);

	// set render target and clear
	pcDevice->SetRenderTarget(0, m_pStereoOutputSurfaceLeft);
	pcDevice->SetDepthStencilSurface(m_pcDepthStencilLeft);
	pcDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clearColor, 1.0f, 0);

	// draw left
	pcDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	// set render data and draw left theatre
	pcDevice->SetVertexShader(m_vsCinemaTheatre);
	pcDevice->SetPixelShader(m_psCinemaTheatre);
	pcDevice->SetStreamSource(0, m_pVBCinemaTheatre, 0, sizeof(TexColoredVertex));
	pcDevice->SetIndices(m_pIBCinemaTheatre);
	pcDevice->SetTexture(0, m_pTextureCinemaTheatre);
	m_ctVCinemaTheatre->SetMatrix(pcDevice, "xViewProjection", &(matViewLeft * matYaw * matPitch * matRoll * matSeparationLeft * matProjection));
	// pcDevice->SetVertexShaderConstantF(0, &matProjection.m[0][0], 4);
	pcDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	pcDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, 0, numIndices / 3);

	D3DRECT crec = { 200, 200, 400, 600 }; //position and dimensions of our rectangle
	pcDevice->Clear(1, &crec, D3DCLEAR_TARGET, D3DCOLOR_XRGB(255, 128, 64), 0, 0);
	
	// set matrix (right)
	if (m_ppsProjectionRight)
	{
		if (*m_ppsProjectionRight)
			CopyMemory(&matProjection, (void*)(*m_ppsProjectionRight), sizeof(D3DMATRIX));
	}
	m_ctVCinema->SetMatrix(pcDevice, "xViewProjection", &(matViewRight * matYaw * matPitch * matRoll * matSeparationRight * matProjection));

	// set all back and render right cinema screen
	pcDevice->SetVertexShader(m_vsCinema);
	pcDevice->SetPixelShader(m_psCinema);
	pcDevice->SetStreamSource(0, m_pVBCinemaScreen, 0, sizeof(TexturedVertex));
	pcDevice->SetFVF(D3DFVF_XYZ | D3DFVF_TEX1);
	if (m_ppcFrameTexture)
	{
		if (*m_ppcFrameTexture)
			pcDevice->SetTexture(0, *m_ppcFrameTexture);
		else
			pcDevice->SetTexture(0, m_pTextureCinema);
	}
	else
		pcDevice->SetTexture(0, m_pTextureCinema);
	pcDevice->SetRenderTarget(0, m_pStereoOutputSurfaceRight);
	pcDevice->SetDepthStencilSurface(m_pcDepthStencilRight);
	pcDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, clearColor, 1.0f, 0);

	// draw right
	pcDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);

	// set render data and draw
	pcDevice->SetVertexShader(m_vsCinemaTheatre);
	pcDevice->SetPixelShader(m_psCinemaTheatre);
	pcDevice->SetStreamSource(0, m_pVBCinemaTheatre, 0, sizeof(TexColoredVertex));
	pcDevice->SetIndices(m_pIBCinemaTheatre);
	pcDevice->SetTexture(0, m_pTextureCinemaTheatre);
	m_ctVCinemaTheatre->SetMatrix(pcDevice, "xViewProjection", &(matViewRight * matYaw * matPitch * matRoll * matSeparationRight * matProjection));
	pcDevice->SetFVF(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1);
	pcDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, numVertices, 0, numIndices / 3);

	// set back render target
	for (int i = 0; i < nTargets; i++)
		pcDevice->SetRenderTarget(i, pSurface[i]);

	pStateBlock->Apply();
	pStateBlock->Release();

	pcDevice->EndScene();
}

/**
* Sets all Direct3D 9 render states to their default values.
* Use this function only if a game does not want to render.
***/
void VireioCinema::SetAllRenderStatesDefault(LPDIRECT3DDEVICE9 pcDevice)
{
	// set all Direct3D 9 RenderStates to default values
	float fData = 0.0f;
	double dData = 0.0f;

	pcDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_TRUE);
	pcDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
	pcDevice->SetRenderState(D3DRS_SHADEMODE, D3DSHADE_GOURAUD);
	pcDevice->SetRenderState(D3DRS_ZWRITEENABLE, TRUE);
	pcDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_LASTPIXEL, TRUE);
	pcDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_ONE);
	pcDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ZERO);
	pcDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	pcDevice->SetRenderState(D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	pcDevice->SetRenderState(D3DRS_ALPHAREF, 0);
	pcDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pcDevice->SetRenderState(D3DRS_DITHERENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_SPECULARENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_FOGCOLOR, 0);
	pcDevice->SetRenderState(D3DRS_FOGTABLEMODE, D3DFOG_NONE);
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_FOGSTART, *((DWORD*)&fData));
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_FOGEND, *((DWORD*)&fData));
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_FOGDENSITY, *((DWORD*)&fData));
	pcDevice->SetRenderState(D3DRS_RANGEFOGENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_STENCILFAIL, D3DSTENCILOP_KEEP);
	pcDevice->SetRenderState(D3DRS_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pcDevice->SetRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
	pcDevice->SetRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
	pcDevice->SetRenderState(D3DRS_STENCILREF, 0);
	pcDevice->SetRenderState(D3DRS_STENCILMASK, 0xFFFFFFFF);
	pcDevice->SetRenderState(D3DRS_STENCILWRITEMASK, 0xFFFFFFFF);
	pcDevice->SetRenderState(D3DRS_TEXTUREFACTOR, 0xFFFFFFFF);
	pcDevice->SetRenderState(D3DRS_WRAP0, 0);
	pcDevice->SetRenderState(D3DRS_WRAP1, 0);
	pcDevice->SetRenderState(D3DRS_WRAP2, 0);
	pcDevice->SetRenderState(D3DRS_WRAP3, 0);
	pcDevice->SetRenderState(D3DRS_WRAP4, 0);
	pcDevice->SetRenderState(D3DRS_WRAP5, 0);
	pcDevice->SetRenderState(D3DRS_WRAP6, 0);
	pcDevice->SetRenderState(D3DRS_WRAP7, 0);
	pcDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
	pcDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	pcDevice->SetRenderState(D3DRS_AMBIENT, 0);
	pcDevice->SetRenderState(D3DRS_FOGVERTEXMODE, D3DFOG_NONE);
	pcDevice->SetRenderState(D3DRS_COLORVERTEX, TRUE);
	pcDevice->SetRenderState(D3DRS_LOCALVIEWER, TRUE);
	pcDevice->SetRenderState(D3DRS_NORMALIZENORMALS, FALSE);
	pcDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_COLOR1);
	pcDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_COLOR2);
	pcDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	pcDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	pcDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
	pcDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, 0);
	pcDevice->SetRenderState(D3DRS_POINTSIZE, 64);
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_POINTSIZE_MIN, *((DWORD*)&fData));
	pcDevice->SetRenderState(D3DRS_POINTSPRITEENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_POINTSCALEENABLE, FALSE);
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_POINTSCALE_A, *((DWORD*)&fData));
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_POINTSCALE_B, *((DWORD*)&fData));
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_POINTSCALE_C, *((DWORD*)&fData));
	pcDevice->SetRenderState(D3DRS_MULTISAMPLEANTIALIAS, TRUE);
	pcDevice->SetRenderState(D3DRS_MULTISAMPLEMASK, 0xFFFFFFFF);
	pcDevice->SetRenderState(D3DRS_PATCHEDGESTYLE, D3DPATCHEDGE_DISCRETE);
	pcDevice->SetRenderState(D3DRS_DEBUGMONITORTOKEN, D3DDMT_ENABLE);
	dData = 64.0;
	pcDevice->SetRenderState(D3DRS_POINTSIZE_MAX, *((DWORD*)&dData));
	pcDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_COLORWRITEENABLE, 0x0000000F);
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_TWEENFACTOR, *((DWORD*)&fData));
	pcDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pcDevice->SetRenderState(D3DRS_POSITIONDEGREE, D3DDEGREE_CUBIC);
	pcDevice->SetRenderState(D3DRS_NORMALDEGREE, D3DDEGREE_LINEAR);
	pcDevice->SetRenderState(D3DRS_SCISSORTESTENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_SLOPESCALEDEPTHBIAS, 0);
	pcDevice->SetRenderState(D3DRS_ANTIALIASEDLINEENABLE, FALSE);
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_MINTESSELLATIONLEVEL, *((DWORD*)&fData));
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_MAXTESSELLATIONLEVEL, *((DWORD*)&fData));
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_ADAPTIVETESS_X, *((DWORD*)&fData));
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_ADAPTIVETESS_Y, *((DWORD*)&fData));
	fData = 1.0f;
	pcDevice->SetRenderState(D3DRS_ADAPTIVETESS_Z, *((DWORD*)&fData));
	fData = 0.0f;
	pcDevice->SetRenderState(D3DRS_ADAPTIVETESS_W, *((DWORD*)&fData));
	pcDevice->SetRenderState(D3DRS_ENABLEADAPTIVETESSELLATION, FALSE);
	pcDevice->SetRenderState(D3DRS_TWOSIDEDSTENCILMODE, FALSE);
	pcDevice->SetRenderState(D3DRS_CCW_STENCILFAIL, D3DSTENCILOP_KEEP);
	pcDevice->SetRenderState(D3DRS_CCW_STENCILZFAIL, D3DSTENCILOP_KEEP);
	pcDevice->SetRenderState(D3DRS_CCW_STENCILPASS, D3DSTENCILOP_KEEP);
	pcDevice->SetRenderState(D3DRS_CCW_STENCILFUNC, D3DCMP_ALWAYS);
	pcDevice->SetRenderState(D3DRS_COLORWRITEENABLE1, 0x0000000f);
	pcDevice->SetRenderState(D3DRS_COLORWRITEENABLE2, 0x0000000f);
	pcDevice->SetRenderState(D3DRS_COLORWRITEENABLE3, 0x0000000f);
	pcDevice->SetRenderState(D3DRS_BLENDFACTOR, 0xffffffff);
	pcDevice->SetRenderState(D3DRS_SRGBWRITEENABLE, 0);
	pcDevice->SetRenderState(D3DRS_DEPTHBIAS, 0);
	pcDevice->SetRenderState(D3DRS_WRAP8, 0);
	pcDevice->SetRenderState(D3DRS_WRAP9, 0);
	pcDevice->SetRenderState(D3DRS_WRAP10, 0);
	pcDevice->SetRenderState(D3DRS_WRAP11, 0);
	pcDevice->SetRenderState(D3DRS_WRAP12, 0);
	pcDevice->SetRenderState(D3DRS_WRAP13, 0);
	pcDevice->SetRenderState(D3DRS_WRAP14, 0);
	pcDevice->SetRenderState(D3DRS_WRAP15, 0);
	pcDevice->SetRenderState(D3DRS_SEPARATEALPHABLENDENABLE, FALSE);
	pcDevice->SetRenderState(D3DRS_SRCBLENDALPHA, D3DBLEND_ONE);
	pcDevice->SetRenderState(D3DRS_DESTBLENDALPHA, D3DBLEND_ZERO);
	pcDevice->SetRenderState(D3DRS_BLENDOPALPHA, D3DBLENDOP_ADD);
}
