/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

OSVR Direct Mode - Open Source Virtual Reality Direct Mode Node
Copyright (C) 2016 Denis Reischl

File <OSVR-DirectMode.h> and
Class <OSVR-DirectMode> :
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

#include <osvr/ClientKit/Context.h>
#include <osvr/ClientKit/Interface.h>
#include <osvr/RenderKit/RenderManager.h>
#pragma comment(lib, "osvrClientKit.lib")
#pragma comment(lib, "osvrRenderManager.lib")

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

// This must come after we include <d3d11.h> so its pointer types are defined.
#include <osvr/RenderKit/GraphicsLibraryD3D11.h>

#include"AQU_Nodus.h"
#include"Resources.h"

#include"..\..\..\Include\Vireio_DX11Basics.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"


#define PNT_FLOAT_PLUG_TYPE                          104
#define PNT_INT_PLUG_TYPE                            107 
#define PNT_UINT_PLUG_TYPE                           112

#define NUMBER_OF_COMMANDERS                           0
#define NUMBER_OF_DECOMMANDERS                         2

/**
* Node Commander Enumeration.
***/
enum OSVR_Commanders
{};

/**
* Node Decommander Enumeration.
***/
enum OSVR_Decommanders
{
	LeftTexture,
	RightTexture,
};

/**
* Direct Mode methods.
***/
enum OSVR_DirectModeMethods
{
	OSVR_undefined,
	OSVR_D3D11_use_Game_Device,
	OSVR_D3D11_own_Device,
	OSVR_D3D10_own_Device,
	OSVR_D3D9_own_Device
};

/**
* Vireio Open Source VR DirectMode Node Plugin.
***/
class OSVR_DirectMode : public AQU_Nodus
{
public:
	OSVR_DirectMode();
	virtual ~OSVR_DirectMode();

	/*** AQU_Nodus public methods ***/
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

	/*** OSVR_DirectMode private methods ***/
	static void SetupDisplay(void* userData, osvr::renderkit::GraphicsLibrary cLibrary, osvr::renderkit::RenderBuffer cBuffers);
	static void DrawWorld(void* userData, osvr::renderkit::GraphicsLibrary cLibrary, osvr::renderkit::RenderBuffer cBuffers,
		osvr::renderkit::OSVR_ViewportDescription sViewport, OSVR_PoseState pose, osvr::renderkit::OSVR_ProjectionMatrix sProjection, OSVR_TimeValue deadline);

	/**
	* The OSVR render manager.
	***/
	osvr::renderkit::RenderManager* m_pcRenderManager;
	/**
	* The 2D vertex shader.
	***/
	static ID3D11VertexShader* m_pcVertexShader11;
	/**
	* The 2D pixel shader.
	***/
	static ID3D11PixelShader* m_pcPixelShader11;
	/**
	* The 2D vertex layout.
	***/
	static ID3D11InputLayout* m_pcVertexLayout11;
	/**
	* The 2D vertex buffer.
	***/
	static ID3D11Buffer* m_pcVertexBuffer11;
	/**
	* The constant buffer for the vertex shader matrix.
	* Contains only ProjView matrix.
	***/
	static ID3D11Buffer* m_pcConstantBufferDirect11;
	/**
	* Basic sampler state.
	***/
	static ID3D11SamplerState* m_pcSamplerState;
	/**
	* Stereo Textures input. (DX11)
	***/
	static struct StereoTextureViews
	{
		ID3D11ShaderResourceView** m_ppcTexView11[2];
	} m_sStereoTextureViews;
	/**
	* The DX mode method this node works.
	***/
	static OSVR_DirectModeMethods m_eMethod;
	/**
	* DirectX 11 device used (if not the own device of the game is used).
	***/
	ID3D11Device* m_pcDevice;
	/**
	* DirectX 11 device condtext used (if not the own device of the game is used).
	***/
	ID3D11DeviceContext* m_pcDeviceContext;
	/**
	* DirectX 11 game device.
	***/
	static ID3D11Device* m_pcGameDevice;
	/**
	* DirectX 11 game device context.
	***/
	static ID3D11DeviceContext* m_pcGameDeviceContext;
	/**
	* Shared copy of textures.
	***/
	static struct StereoTextureCopies
	{
		ID3D11Texture2D* m_pcTex11Copy[2];
	} m_sStereoTextureCopies;
	/**
	* The eventual frame textures to be rendered to the Oculus screens.
	***/
	static struct StereoFrameTextures
	{
		ID3D11Texture2D* m_pcFrameTexture[2];
	} m_sStereoFrameTextures;
	/**
	* The eventual frame texture shader resource views to be rendered to the Oculus screens.
	***/
	static struct StereoFrameTextureSRViews
	{
		ID3D11ShaderResourceView* m_pcFrameTextureSRView[2];
	} m_sSteroFrameTextureSRViews;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	OSVR_DirectMode* pOSVR_DirectMode = new OSVR_DirectMode();
	return static_cast<AQU_Nodus*>(pOSVR_DirectMode);
}
