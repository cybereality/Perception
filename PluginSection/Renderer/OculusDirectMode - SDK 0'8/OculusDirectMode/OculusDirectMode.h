/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Oculus Direct Mode - Oculus Rift Direct Mode Node Plugin
Copyright (C) 2015 Denis Reischl

File <OculusDirectMode.h> and
Class <OculusDirectMode> :
Copyright (C) 2015 Denis Reischl

Parts of this class directly derive from the Oculus Rift sample
>OculusRoomTiny< (Author Tom Heath - Copyright :
Copyright 2015 Oculus, Inc. All Rights reserved.
http://www.apache.org/licenses/LICENSE-2.0)
and from the Vireio source code originally authored by Simon Brown.
(class OculusDirectToRiftView v3.0.0 2015).

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

#include"AQU_Nodus.h"
#include"Resources.h"
#include<sstream>

#include <DXGI.h>
#pragma comment(lib, "DXGI.lib")

#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d10_1.h>
#pragma comment(lib, "d3d10_1.lib")

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

#include <d3dx10.h>
#pragma comment(lib, "d3dx10.lib")

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include "Extras/OVR_Math.h"
#define OVR_D3D_VERSION 11
#include"OVR_CAPI_D3D.h"
#include"..\..\..\Include\Vireio_DX11Basics.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"

#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_INT_PLUG_TYPE                            107 
#define PNT_UINT_PLUG_TYPE                           112
#define PNT_D3DMATRIX_PLUG_TYPE                     2017
#define PNT_VECTOR3F_PLUG_TYPE                      2061
#define PNT_VECTOR4F_PLUG_TYPE                      2063
#define PPNT_IDIRECT3DINDEXBUFFER9_PLUG_TYPE        3041
#define PPNT_IDIRECT3DVERTEXBUFFER9_PLUG_TYPE       3049
#define PPNT_IDIRECT3DVERTEXDECLARATION9_PLUG_TYPE  3050

#define NUMBER_OF_DECOMMANDERS                         8

/**
* Node Commander Enumeration.
***/
enum ODM_Decommanders
{
	LeftTexture11,
	RightTexture11,
	LeftTexture10,
	RightTexture10,
	LeftTexture9,
	RightTexture9,
	HMD_Handle,
	ZoomOut
};

/**
* ovrSwapTextureSet wrapper class that also maintains the render target views
* needed for D3D11 rendering.
* Taken from the OculusRoomTiny demo for simplicity.
***/
struct OculusTexture
{
	ovrHmd                   hmd;
	ovrSwapTextureSet      * TextureSet;
	static const int         TextureCount = 2;
	ID3D11RenderTargetView * TexRtv[TextureCount];

	OculusTexture() :
		hmd(nullptr),
		TextureSet(nullptr)
	{
		TexRtv[0] = TexRtv[1] = nullptr;
	}

	bool Init(ID3D11Device* pcDevice, ovrHmd* _hmd, int sizeW, int sizeH)
	{
		//hmd = _hmd;

		D3D11_TEXTURE2D_DESC dsDesc;
		dsDesc.Width = sizeW;
		dsDesc.Height = sizeH;
		dsDesc.MipLevels = 1;
		dsDesc.ArraySize = 1;
		dsDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		dsDesc.SampleDesc.Count = 1;   // No multi-sampling allowed
		dsDesc.SampleDesc.Quality = 0;
		dsDesc.Usage = D3D11_USAGE_DEFAULT;
		dsDesc.CPUAccessFlags = 0;
		dsDesc.MiscFlags = 0;
		dsDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_RENDER_TARGET;

		ovrResult result = ovr_CreateSwapTextureSetD3D11(*_hmd, pcDevice, &dsDesc, ovrSwapTextureSetD3D11_Typeless, &TextureSet);
		if (!OVR_SUCCESS(result))
		{
			ovrErrorInfo sErrorInfo;
			ovr_GetLastErrorInfo(&sErrorInfo);

			OutputDebugString(L"Error:");
			OutputDebugStringA(sErrorInfo.ErrorString);
			return false;
		}

		if (TextureSet->TextureCount != TextureCount)
			OutputDebugString(L"TextureCount mismatch.");

		for (int i = 0; i < TextureCount; ++i)
		{
			ovrD3D11Texture* tex = (ovrD3D11Texture*)&TextureSet->Textures[i];
			D3D11_RENDER_TARGET_VIEW_DESC rtvd = {};
			rtvd.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			rtvd.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
			pcDevice->CreateRenderTargetView(tex->D3D11.pTexture, &rtvd, &TexRtv[i]);
		}

		return true;
	}

	~OculusTexture()
	{
		for (int i = 0; i < TextureCount; ++i)
		{
			if (TexRtv[i]) TexRtv[i]->Release();
		}
		if (TextureSet)
		{
			ovr_DestroySwapTextureSet(hmd, TextureSet);
		}
	}

	void AdvanceToNextTexture()
	{
		TextureSet->CurrentIndex = (TextureSet->CurrentIndex + 1) % TextureSet->TextureCount;
	}
};

/**
* Oculus Direct Mode Node Plugin.
***/
class OculusDirectMode : public AQU_Nodus
{
public:
	OculusDirectMode();
	virtual ~OculusDirectMode();

	/*** OculusDirectMode public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4 + 256 + 4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual DWORD           GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex);
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);

private:
	/**
	* Submit left and right shared texture constantly.
	***/
	static DWORD WINAPI SubmitFramesConstantly(void* Param)
	{		
		while (true)
		{
			if (m_phHMD)
			{
				if (*m_phHMD)
				{
					if (m_bInit)
					{
						// Initialize our single full screen Fov layer.
						ovrLayerEyeFov ld = {};
						ld.Header.Type = ovrLayerType_EyeFov;
						ld.Header.Flags = 0;

						for (int eye = 0; eye < 2; ++eye)
						{
							ld.ColorTexture[eye] = m_psEyeRenderTexture[eye]->TextureSet;
							ld.Viewport[eye] = m_psEyeRenderViewport[eye];
							ld.Fov[eye] = m_sHMDDesc.DefaultEyeFov[eye];
							ld.RenderPose[eye] = asEyeRenderPose[eye];
							ld.SensorSampleTime = sensorSampleTime;
						}

						ovrLayerHeader* psLayers = &ld.Header;
						ovrResult result = ovr_SubmitFrame(*m_phHMD, 0, nullptr, &psLayers, 1);

						if (true) // TODO !! MAKE THIS OPTIONALLY !!
						{
							Sleep(10);
						}
						else
						{
							Sleep(20);
						}
					}
				}
			}
		}
	}

	/**
	* True if OVR is initialized.
	***/
	static bool m_bInit;
	/**
	* The handle of the headset.
	***/
	ovrHmd m_hHMD;
	/**
	* The pointer to the HMD handle created either by this node or the oculus tracker node.
	***/
	static ovrHmd* m_phHMD;
	/**
	* The pointer to the handle created by the oculus tracker node.
	***/
	ovrHmd* m_phHMD_Tracker;
	/**
	* The HMD description.
	***/
	static ovrHmdDesc m_sHMDDesc;
	/**
	* Adapter Identifier.
	***/
	ovrGraphicsLuid m_sLuid;
	/**
	* The mirror texture (of the whole Oculus frame) to be shown on main window.
	***/
	ovrTexture* m_pcMirrorTexture;
	/**
	* The mirror texture (of the whole Oculus frame) to be shown on main window.
	* Dx version for the game device.
	***/
	ID3D11Texture2D* m_pcMirrorTextureD3D11;
	/**
	* The Oculus swapchain. (for both eyes)
	***/
	static OculusTexture* m_psEyeRenderTexture[2];
	/**
	* The Oculus render viewport. (for both eyes)
	***/
	static ovrRecti m_psEyeRenderViewport[2];
	/**
	* The Oculus render description. (for both eyes)
	***/
	ovrEyeRenderDesc m_psEyeRenderDesc[2];
	/**
	* Stereo Textures input. (DX11)
	***/
	ID3D11ShaderResourceView** m_ppcTexView11[2];
	/**
	* Shared copy of textures.
	***/
	ID3D11Texture2D* m_pcTex11Copy[2];
	/**
	* Temporary directx 11 device for the oculus sdk.
	***/
	ID3D11Device* m_pcDeviceTemporary;
	/**
	* Temporary directx 11 device context for the oculus sdk.
	***/
	ID3D11DeviceContext* m_pcContextTemporary;
	/**
	* Temporary directx 11 dxgi swapchain for the oculus sdk.
	* (keep that one for the mirror texture)
	***/
	IDXGISwapChain* m_pcSwapChainTemporary;
	/**
	* Temporary directx 11 back buffer for the oculus sdk.
	***/
	ID3D11Texture2D* m_pcBackBufferTemporary;
	/**
	* Temporary directx 11 back buffer render target view for the oculus sdk.
	***/
	ID3D11RenderTargetView * m_pcBackBufferRTVTemporary;
	/**
	* The 2D vertex shader.
	***/
	ID3D11VertexShader* m_pcVertexShader11;
	/**
	* The 2D pixel shader.
	***/
	ID3D11PixelShader* m_pcPixelShader11;
	/**
	* The 2D vertex layout.
	***/
	ID3D11InputLayout* m_pcVertexLayout11;
	/**
	* The 2D vertex buffer.
	***/
	ID3D11Buffer* m_pcVertexBuffer11;
	/**
	* The constant buffer for the vertex shader matrix.
	* Contains only ProjView matrix.
	***/
	ID3D11Buffer* m_pcConstantBufferDirect11;
	/**
	* Basic sampler state.
	***/
	ID3D11SamplerState* m_pcSamplerState;
	/**
	* The eventual frame textures to be rendered to the Oculus screens.
	***/
	ID3D11Texture2D* m_pcFrameTexture[2];
	/**
	* The eventual frame texture shader resource views to be rendered to the Oculus screens.
	***/
	ID3D11ShaderResourceView* m_pcFrameTextureSRView[2];
	/**
	* Hotkey switch.
	***/
	bool m_bHotkeySwitch;
	/**
	* Zoom out switch.
	***/
	BOOL* m_pbZoomOut;
	/**
	* Eye render poses, static to be used by submission thread.
	***/
	static ovrPosef asEyeRenderPose[2];
	/**
	* Timestamp for last drawn frame.
	***/
	static double sensorSampleTime;
	/**
	* Static thread handle.
	***/
	static HANDLE m_pThread;

};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	OculusDirectMode* pOculusDirectMode = new OculusDirectMode();
	return static_cast<AQU_Nodus*>(pOculusDirectMode);
}