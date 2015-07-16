/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Oculus Direct Mode - Oculus Rift DirectMode Node Plugin
Copyright (C) 2015 Denis Reischl

File <OculusDirectMode.cpp> and
Class <OculusDirectMode> :
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

#include"OculusDirectMode.h"

#define INTERFACE_IDIRECT3DDEVICE9 8
#define INTERFACE_IDIRECT3DSWAPCHAIN9 15

#define INTERFACE_IDIRECT3DDEVICE9           8
#define INTERFACE_IDIRECT3DSWAPCHAIN9       15
#define	METHOD_IDIRECT3DDEVICE9_PRESENT     17
#define	METHOD_IDIRECT3DDEVICE9_ENDSCENE    42
#define	METHOD_IDIRECT3DSWAPCHAIN9_PRESENT   3

/**
* Constructor.
***/
OculusDirectMode::OculusDirectMode() : AQU_Nodus(),
	m_pcDeviceTemporary(nullptr),
	m_pcContextTemporary(nullptr),
	m_pcBackBuffer(nullptr),
	m_pcVertexShaderDirect(nullptr),
	m_pcPixelShaderDirect(nullptr),
	m_pcVertexLayoutDirect(nullptr),
	m_pcVertexBufferDirect(nullptr),
	m_pcVertexShaderMirror(nullptr),
	m_pcPixelShaderMirror(nullptr),
	m_pcVertexLayoutMirror(nullptr),
	m_pcVertexBufferMirror(nullptr),
	m_pcTextureDirect(nullptr),
	m_pcBackBufferCopy(nullptr),
	m_pcMirrorCopy(nullptr),
	m_pcTextureViewDirect(nullptr),
	m_bInit(false),
	m_pcMirrorTexture(nullptr)
{	
}

/**
* Destructor.
***/
OculusDirectMode::~OculusDirectMode()
{
	if (m_pcContextTemporary) m_pcContextTemporary->Release();
	if (m_pcDeviceTemporary) m_pcDeviceTemporary->Release();
	if (m_pcBackBuffer) m_pcBackBuffer->Release();

	if (m_pcVertexShaderDirect) m_pcVertexShaderDirect->Release();
	if (m_pcPixelShaderDirect) m_pcPixelShaderDirect->Release();
	if (m_pcVertexLayoutDirect) m_pcVertexLayoutDirect->Release();
	if (m_pcVertexBufferDirect) m_pcVertexBufferDirect->Release();
	if (m_pcVertexShaderMirror) m_pcVertexShaderMirror->Release();
	if (m_pcPixelShaderMirror) m_pcPixelShaderMirror->Release();
	if (m_pcVertexLayoutMirror) m_pcVertexLayoutMirror->Release();
	if (m_pcVertexBufferMirror) m_pcVertexBufferMirror->Release();
	if (m_pcTextureDirect) m_pcTextureDirect->Release();
	if (m_pcTextureViewDirect) m_pcTextureViewDirect->Release();
	if (m_pcBackBufferCopy) m_pcBackBufferCopy->Release();
	if (m_pcMirrorCopy) m_pcMirrorCopy->Release();
}

/**
* Return the name of the  Oculus Renderer node.
***/
const char* OculusDirectMode::GetNodeType() 
{
	return "Oculus Direct Mode"; 
}

/**
* Returns a global unique identifier for the Oculus Renderer node.
***/
UINT OculusDirectMode::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#define MY_PLUGIN_IDENTIFIER 259
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Oculus Renderer node.
***/
LPWSTR OculusDirectMode::GetCategory()
{
	return L"Renderer";
}

/**
* Returns a logo to be used for the Oculus Renderer node.
***/
HBITMAP OculusDirectMode::GetLogo() 
{ 
	HMODULE hModule = GetModuleHandle(L"OculusDirectMode.dll");
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/** 
* Returns the updated control for the Oculus Renderer node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP OculusDirectMode::GetControl()
{
	//if (!m_hBitmapControl)
	//{
	// TODO : Design the Oculus direct mode node, both Logo and Control
	//}

	return nullptr;
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR OculusDirectMode::GetDecommanderName(DWORD dwDecommanderIndex)
{
	switch ((ORN_Decommanders)dwDecommanderIndex)
	{
	case ORN_Decommanders::LeftTexture:
		return L"Left Texture";
	case ORN_Decommanders::RightTexture:
		return L"Right Texture";
	}

	return L"";
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD OculusDirectMode::GetDecommanderType(DWORD dwDecommanderIndex) 
{
	//switch ((ORN_Decommanders)dwDecommanderIndex)
	//{
	//	/*case ORN_Decommanders::LeftTexture:
	//	return PNT_IDIRECT3DTEXTURE9_PLUG_TYPE;
	//	case ORN_Decommanders::RightTexture:
	//	return PNT_IDIRECT3DTEXTURE9_PLUG_TYPE;*/
	//}

	return 0;
}

/**
* Sets the input pointer for the requested decommander.
***/
void OculusDirectMode::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
}

/**
* Oculus Direct Mode supports D3D 9 - 11 Present() calls.
***/
bool OculusDirectMode::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)  
{ 
	/*if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_9_0) &&
	(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_9_29))
	{
	if (((nD3DInterface == INTERFACE_IDIRECT3DDEVICE9) &&
	(nD3DMethod == METHOD_IDIRECT3DDEVICE9_PRESENT)) || 
	((nD3DInterface == INTERFACE_IDIRECT3DDEVICE9) &&
	(nD3DMethod == METHOD_IDIRECT3DDEVICE9_ENDSCENE)) ||
	((nD3DInterface == INTERFACE_IDIRECT3DSWAPCHAIN9) &&
	(nD3DMethod == METHOD_IDIRECT3DSWAPCHAIN9_PRESENT)))
	return true;
	}*/
	return true; 
}

/**
* Render the Oculus Rift View.
***/
void* OculusDirectMode::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)	
{
	// cast swapchain
	IDXGISwapChain* pcSwapChain = (IDXGISwapChain*)pThis;
	if (!pcSwapChain) 
	{
		OutputDebugString(L"Oculus Direct Mode Node : No swapchain !");
		return nullptr;
	}
	// get device
	ID3D11Device* pcDevice = nullptr;
	pcSwapChain->GetDevice(__uuidof(ID3D11Device), (void**)&pcDevice);
	if (!pcDevice)
	{
		OutputDebugString(L"HelloWorld Node : No d3d 11 device !");
		return nullptr;
	}
	// get context
	ID3D11DeviceContext* pcContext = nullptr;
	pcDevice->GetImmediateContext(&pcContext);
	if (!pcContext)
	{
		OutputDebugString(L"HelloWorld Node : No device context !");
		return nullptr;
	}

	// backup all states
	D3DX11_STATE_BLOCK sStateBlock;
	CreateStateblock(pcContext, &sStateBlock);

	if (!m_bInit)
	{
#pragma region Init
		// first, get the back buffer surface
		D3D11_TEXTURE2D_DESC sDesc;
		if (SUCCEEDED(pcSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&m_pcBackBuffer )))
		{
			m_pcBackBuffer->GetDesc(&sDesc);
		}
		else
		{
			// release frame texture+view
			if (m_pcTextureViewDirect) { m_pcTextureViewDirect->Release(); m_pcTextureViewDirect = nullptr; }
			if (m_pcTextureDirect) { m_pcTextureDirect->Release(); m_pcTextureDirect = nullptr; }

			// release d3d11 device + context... 
			pcContext->Release();
			pcDevice->Release();

			return nullptr;
		}

		// create temporary (OVR) device
		IDXGIFactory * DXGIFactory;
		IDXGIAdapter * Adapter;
		if (FAILED(CreateDXGIFactory1(__uuidof(IDXGIFactory), (void**)(&DXGIFactory))))
			return(false);
		if (FAILED(DXGIFactory->EnumAdapters(0, &Adapter)))
			return(false);
		if (FAILED(D3D11CreateDevice(Adapter, Adapter ? D3D_DRIVER_TYPE_UNKNOWN : D3D_DRIVER_TYPE_HARDWARE,
			NULL, 0, NULL, 0, D3D11_SDK_VERSION, &m_pcDeviceTemporary, NULL, &m_pcContextTemporary)))
			return(false);

		// Set a standard blend state, ie transparency from alpha
		D3D11_BLEND_DESC bm;
		memset(&bm, 0, sizeof(bm));
		bm.RenderTarget[0].BlendEnable = TRUE;
		bm.RenderTarget[0].BlendOp = bm.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		bm.RenderTarget[0].SrcBlend = bm.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		bm.RenderTarget[0].DestBlend = bm.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		bm.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
		ID3D11BlendState * BlendState;
		m_pcDeviceTemporary->CreateBlendState(&bm, &BlendState);
		m_pcContextTemporary->OMSetBlendState(BlendState, NULL, 0xffffffff);

		// Set max frame latency to 1
		IDXGIDevice1* DXGIDevice1 = NULL;
		HRESULT hr = m_pcDeviceTemporary->QueryInterface(__uuidof(IDXGIDevice1), (void**)&DXGIDevice1);
		if (FAILED(hr) | (DXGIDevice1 == NULL)) return(false);
		DXGIDevice1->SetMaximumFrameLatency(1);
		DXGIDevice1->Release();

		// Initialize LibOVR, and the Rift... then create hmd handle
		ovrResult result = ovr_Initialize(nullptr);
		if (!OVR_SUCCESS(result)) 
		{
			OutputDebugString(L"Failed to initialize libOVR.");
			return nullptr;
		}
		ovrResult actualHMD = ovrHmd_Create(0, &m_hHMD);
		if (!OVR_SUCCESS(actualHMD)) result = ovrHmd_CreateDebug(ovrHmd_DK2, &m_hHMD); // Use debug one, if no genuine Rift available
		if (!OVR_SUCCESS(result)) OutputDebugString(L"Oculus Rift not detected.");
		if (m_hHMD->ProductName[0] != '\0') OutputDebugString(L"Rift detected, display not enabled.");

		ovrHmd_SetEnabledCaps(m_hHMD, ovrHmdCap_LowPersistence|ovrHmdCap_DynamicPrediction);

		// Start the sensor which informs of the Rift's pose and motion
		result = ovrHmd_ConfigureTracking(m_hHMD, ovrTrackingCap_Orientation | ovrTrackingCap_MagYawCorrection |
			ovrTrackingCap_Position, 0);

		// Make the eye render buffers (caution if actual size < requested due to HW limits). 
		for (int eye = 0; eye < 2; eye++)
		{
			OVR::Sizei idealSize = ovrHmd_GetFovTextureSize(m_hHMD, (ovrEyeType)eye, m_hHMD->DefaultEyeFov[eye], 1.0f);

			m_psEyeRenderTexture[eye]      = new OculusTexture(m_pcDeviceTemporary, m_hHMD, idealSize);
			m_psEyeDepthBuffer[eye]        = new DepthBuffer(m_pcDeviceTemporary, idealSize);
			m_psEyeRenderViewport[eye].Pos  = OVR::Vector2i(0, 0);
			m_psEyeRenderViewport[eye].Size = idealSize;
		}

		// Create a mirror to see on the monitor.
		D3D11_TEXTURE2D_DESC td = { };
		td.ArraySize        = 1;
		td.Format           = DXGI_FORMAT_B8G8R8A8_UNORM;
		td.Width            = (UINT)sDesc.Width;
		td.Height           = (UINT)sDesc.Height;
		td.Usage            = D3D11_USAGE_DEFAULT;
		td.SampleDesc.Count = 1;
		td.MipLevels        = 1;
		ovrHmd_CreateMirrorTextureD3D11(m_hHMD, m_pcDeviceTemporary, &td, &m_pcMirrorTexture);
		// create a "STAGING" texture as copy buffer
		D3D11_TEXTURE2D_DESC sDesc1 =
		{
			td.Width, td.Height, 1, 1, td.Format,
			{ 1, 0 }, 
			D3D11_USAGE_STAGING,
			0, D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE, 0
		};
		if (FAILED(m_pcDeviceTemporary->CreateTexture2D( &sDesc1, nullptr, &m_pcMirrorCopy )))
			OutputDebugString(L"Failed to create Texture (mirror copy).");


		// Setup VR components, filling out description
		m_psEyeRenderDesc[0] = ovrHmd_GetRenderDesc(m_hHMD, ovrEye_Left, m_hHMD->DefaultEyeFov[0]);
		m_psEyeRenderDesc[1] = ovrHmd_GetRenderDesc(m_hHMD, ovrEye_Right, m_hHMD->DefaultEyeFov[1]);

		// create vertex shader
		if (!m_pcVertexShaderDirect)
		{
			ID3D10Blob* pcShader;

			// compile and create shader
			if (SUCCEEDED(D3DX10CompileFromMemory(VS2D,strlen(VS2D),NULL,NULL,NULL,"VS","vs_4_0",NULL,NULL,NULL, &pcShader,NULL,NULL)))
			{
				OutputDebugString(L"HelloWorldDx10 Node : Vertex Shader compiled !");

				// create direct mode vertex shader
				m_pcDeviceTemporary->CreateVertexShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, &m_pcVertexShaderDirect);

				if (m_pcVertexShaderDirect) 
				{
					OutputDebugString(L"HelloWorldDx10 Node : Create Input Layout !");
					// Define the input layout
					D3D11_INPUT_ELEMENT_DESC layout[] =
					{
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					};
					UINT numElements = sizeof( layout ) / sizeof( layout[0] );

					m_pcDeviceTemporary->CreateInputLayout( layout, numElements, pcShader->GetBufferPointer(), pcShader->GetBufferSize(), &m_pcVertexLayoutDirect );
				}

				// create mirror vertex shader (same as direct mode shader, but for game device)
				pcDevice->CreateVertexShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, &m_pcVertexShaderMirror);

				if (m_pcVertexShaderMirror) 
				{
					OutputDebugString(L"HelloWorldDx10 Node : Create Input Layout !");
					// Define the input layout
					D3D11_INPUT_ELEMENT_DESC layout[] =
					{
						{ "POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
						{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
					};
					UINT numElements = sizeof( layout ) / sizeof( layout[0] );

					pcDevice->CreateInputLayout( layout, numElements, pcShader->GetBufferPointer(), pcShader->GetBufferSize(), &m_pcVertexLayoutMirror );
				}

				pcShader->Release();
			}
		}

		// create pixel shader
		if (!m_pcPixelShaderDirect)
		{
			ID3D10Blob* pcShader;

			// compile and create shader
			if (SUCCEEDED(D3DX10CompileFromMemory(PS2D,strlen(PS2D),NULL,NULL,NULL,"PS","ps_4_0",NULL,NULL,NULL, &pcShader,NULL,NULL)))
			{
				OutputDebugString(L"HelloWorldDx10 Node : Pixel Shader compiled !");
				// create both, direct mode and mirror pixel shader
				m_pcDeviceTemporary->CreatePixelShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, &m_pcPixelShaderDirect);
				pcDevice->CreatePixelShader(pcShader->GetBufferPointer(), pcShader->GetBufferSize(), NULL, &m_pcPixelShaderMirror);
				pcShader->Release();
			}
		}

		// Create vertex buffer
		if (!m_pcVertexBufferDirect)
		{
			// Create vertex buffer
			TexturedVertex vertices[] =
			{
				{ D3DXVECTOR4( -1.0f, 1.0f, 0.5f, 1.0f ), D3DXVECTOR2( 0.0f, 0.0f ) }, // top left
				{ D3DXVECTOR4(  1.0f, 1.0f, 0.5f, 1.0f  ), D3DXVECTOR2( 1.0f, 0.0f ) }, // top right
				{ D3DXVECTOR4( -1.0f, -1.0f, 0.5f, 1.0f), D3DXVECTOR2( 0.0f, 1.0f ) }, // bottom left

				{ D3DXVECTOR4( -1.0f, -1.0f, 0.5f, 1.0f ), D3DXVECTOR2( 0.0f, 1.0f ) }, // bottom left
				{ D3DXVECTOR4(  1.0f, 1.0f, 0.5f, 1.0f  ), D3DXVECTOR2( 1.0f, 0.0f ) }, // top right
				{ D3DXVECTOR4( 1.0f, -1.0f, 0.5f, 1.0f), D3DXVECTOR2( 1.0f, 1.0f ) }, // bottom right
			};
			D3D11_BUFFER_DESC bd;
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.ByteWidth = sizeof( TexturedVertex ) * 6;
			bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
			bd.CPUAccessFlags = 0;
			bd.MiscFlags = 0;
			D3D11_SUBRESOURCE_DATA InitData;
			InitData.pSysMem = vertices;

			// create both, direct mode and mirror buffer
			m_pcDeviceTemporary->CreateBuffer( &bd, &InitData, &m_pcVertexBufferDirect );
			pcDevice->CreateBuffer( &bd, &InitData, &m_pcVertexBufferMirror );
		}

		// create the back buffer copy texture
		if (!m_pcTextureDirect)
		{
			// create a "STAGING" texture as copy buffer
			D3D11_TEXTURE2D_DESC sDesc1 =
			{
				sDesc.Width, sDesc.Height, 1, 1, sDesc.Format,
				{ 1, 0 }, 
				D3D11_USAGE_STAGING,
				0, D3D11_CPU_ACCESS_READ | D3D11_CPU_ACCESS_WRITE, 0
			};
			if (FAILED(pcDevice->CreateTexture2D( &sDesc1, nullptr, &m_pcBackBufferCopy )))
				OutputDebugString(L"Failed to create Texture (back buffer copy).");
		}

		m_bInit = true;
#pragma endregion
	}
	else
	{
#pragma region Render
		// basic code here taken from OculusRoomTiny example
		ovrVector3f      HmdToEyeViewOffset[2] = { m_psEyeRenderDesc[0].HmdToEyeViewOffset,
			m_psEyeRenderDesc[1].HmdToEyeViewOffset };
		ovrPosef         EyeRenderPose[2];
		ovrPosef         ZeroPose; ZeroMemory(&ZeroPose, sizeof(ovrPosef));
		ovrFrameTiming   ftiming  = ovrHmd_GetFrameTiming(m_hHMD, 0);
		ovrTrackingState hmdState = ovrHmd_GetTrackingState(m_hHMD, ftiming.DisplayMidpointSeconds);
		ovr_CalcEyePoses(hmdState.HeadPose.ThePose, HmdToEyeViewOffset, EyeRenderPose);

		// copy the current frame to the (cpu write/read enabled) back buffer copy texture
		pcContext->CopyResource((ID3D11Resource*)m_pcBackBufferCopy, (ID3D11Resource*)m_pcBackBuffer);

		// map the back buffer copy texture and create the current frame texture from the raw data
		D3D11_MAPPED_SUBRESOURCE sMappedResource;
		m_pcTextureViewDirect = nullptr;
		m_pcTextureDirect = nullptr;
		if (SUCCEEDED(pcContext->Map((ID3D11Resource*)m_pcBackBufferCopy, 0, D3D11_MAP_READ, 0, &sMappedResource)))
		{
			// get the back buffer surface
			if (SUCCEEDED(pcSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&m_pcBackBuffer )))
			{
				// create the frame texture by back buffer description
				D3D11_TEXTURE2D_DESC sDesc;
				m_pcBackBuffer->GetDesc(&sDesc);
				sDesc.Usage = D3D11_USAGE_DEFAULT;
				sDesc.CPUAccessFlags = 0;
				sDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
				D3D11_SUBRESOURCE_DATA sData;
				ZeroMemory(&sData, sizeof(sData));
				sData.pSysMem = sMappedResource.pData;
				sData.SysMemPitch = sMappedResource.RowPitch;
				if (FAILED(m_pcDeviceTemporary->CreateTexture2D( &sDesc, &sData, &m_pcTextureDirect )))
					OutputDebugString(L"Failed to create Texture.");

				// release the back buffer
				m_pcBackBuffer->Release();

				// create the frame texture view
				if (m_pcTextureDirect)
				{
					D3D11_SHADER_RESOURCE_VIEW_DESC sDesc1;
					ZeroMemory(&sDesc1, sizeof(sDesc1));
					sDesc1.Format = sDesc.Format;
					sDesc1.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
					sDesc1.Texture2D.MostDetailedMip = 0;
					sDesc1.Texture2D.MipLevels = 1;

					if ((FAILED(m_pcDeviceTemporary->CreateShaderResourceView((ID3D11Resource*)m_pcTextureDirect, &sDesc1, &m_pcTextureViewDirect))))
						OutputDebugString(L"Failed to create texture view! (direct)");
				}
			}
			pcContext->Unmap((ID3D11Resource*)m_pcBackBufferCopy, 0);
		}

		// render
		for (int eye = 0; eye < 2; eye++)
		{
			// Increment to use next texture, just before writing
			m_psEyeRenderTexture[eye]->AdvanceToNextTexture();

			// clear and set render target
			int texIndex = m_psEyeRenderTexture[eye]->TextureSet->CurrentIndex;
			float ClearColor[4] = { 0.1f, 0.125f, 0.3f, 0.0f }; // red,green,blue,alpha
			m_pcContextTemporary->ClearRenderTargetView(m_psEyeRenderTexture[eye]->TexRtv[texIndex], ClearColor);
			m_pcContextTemporary->OMSetRenderTargets(1, &m_psEyeRenderTexture[eye]->TexRtv[texIndex], nullptr);

			// set viewport
			D3D11_VIEWPORT D3Dvp;
			D3Dvp.Width = (float)m_psEyeRenderViewport[eye].Size.w;    
			D3Dvp.Height = (float)m_psEyeRenderViewport[eye].Size.h;
			D3Dvp.MinDepth = 0;           
			D3Dvp.MaxDepth = 1;
			D3Dvp.TopLeftX = (float)m_psEyeRenderViewport[eye].Pos.x; 
			D3Dvp.TopLeftY = (float)m_psEyeRenderViewport[eye].Pos.y;
			m_pcContextTemporary->RSSetViewports(1, &D3Dvp);

			// Set the input layout
			m_pcContextTemporary->IASetInputLayout( m_pcVertexLayoutDirect );

			// Set vertex buffer
			UINT stride = sizeof( TexturedVertex );
			UINT offset = 0;
			m_pcContextTemporary->IASetVertexBuffers( 0, 1, &m_pcVertexBufferDirect, &stride, &offset );

			// Set primitive topology
			m_pcContextTemporary->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

			// set texture
			m_pcContextTemporary->PSSetShaderResources(0, 1, &m_pcTextureViewDirect);

			// set shaders
			m_pcContextTemporary->VSSetShader(m_pcVertexShaderDirect, 0, 0);
			m_pcContextTemporary->PSSetShader(m_pcPixelShaderDirect, 0, 0);

			// Render a triangle
			m_pcContextTemporary->Draw( 6, 0 );
		}

		// Initialize our single full screen Fov layer.
		ovrLayerEyeFov ld;
		ld.Header.Type  = ovrLayerType_EyeFov;
		ld.Header.Flags = 0;

		for (int eye = 0; eye < 2; eye++)
		{
			ld.ColorTexture[eye] = m_psEyeRenderTexture[eye]->TextureSet;
			ld.Viewport[eye]     = m_psEyeRenderViewport[eye];
			ld.Fov[eye]          = m_hHMD->DefaultEyeFov[eye];
			ld.RenderPose[eye]   = EyeRenderPose[eye];
		}

		// Set up positional data.
		ovrViewScaleDesc viewScaleDesc;
		viewScaleDesc.HmdSpaceToWorldScaleInMeters = 1.0f;
		viewScaleDesc.HmdToEyeViewOffset[0] = HmdToEyeViewOffset[0];
		viewScaleDesc.HmdToEyeViewOffset[1] = HmdToEyeViewOffset[1];

		// submit the frame
		ovrLayerHeader* layers = &ld.Header;
		ovrResult result = ovrHmd_SubmitFrame(m_hHMD, 0, nullptr/*&viewScaleDesc*/, &layers, 1);
	}

	// mirror the oculus screen to the main window game screen
	if (true)
	{
		// get render target view
		ID3D11RenderTargetView* ppRenderTargetView[D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT];
		ID3D11DepthStencilView* pDepthStencilView = nullptr;
		pcContext->OMGetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, ppRenderTargetView, &pDepthStencilView);
		UINT dwNumViewports = 1;
		D3D11_VIEWPORT sViewport[16];
		pcContext->RSGetViewports(&dwNumViewports, sViewport);

		// no render target present ? get from swapchain
		if (!ppRenderTargetView[0])
		{
			ID3D11Texture2D* pcBackBuffer = NULL;
			pcSwapChain->GetBuffer( 0, __uuidof( ID3D11Texture2D ), ( LPVOID* )&pcBackBuffer );
			if (pcBackBuffer)
			{
				pcDevice->CreateRenderTargetView( pcBackBuffer, NULL, &ppRenderTargetView[0] );
				pcBackBuffer->Release();
			}
		}

		// clear all states, set targets
		pcContext->ClearState();

		// clear and set render target
		float ClearColor[4] = { 0.1f, 0.125f, 0.3f, 0.0f }; // red,green,blue,alpha
		pcContext->ClearRenderTargetView(ppRenderTargetView[0], ClearColor);
		pcContext->OMSetRenderTargets(D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT, ppRenderTargetView, nullptr);

		// set viewport
		pcContext->RSSetViewports(dwNumViewports, sViewport);

		// Set the input layout
		pcContext->IASetInputLayout( m_pcVertexLayoutMirror );

		// Set vertex buffer
		UINT stride = sizeof( TexturedVertex );
		UINT offset = 0;
		pcContext->IASetVertexBuffers( 0, 1, &m_pcVertexBufferMirror, &stride, &offset );

		// Set primitive topology
		pcContext->IASetPrimitiveTopology( D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST );

		// create and set texture
		ovrD3D11Texture* tex = (ovrD3D11Texture*)m_pcMirrorTexture;
		m_pcContextTemporary->CopyResource((ID3D11Resource*)m_pcMirrorCopy, (ID3D11Resource*)tex->D3D11.pTexture);
		D3D11_MAPPED_SUBRESOURCE sMappedResource;
		ID3D11ShaderResourceView* pcTextureViewMirror(nullptr);
		ID3D11Texture2D* pcTextureMirror(nullptr);
		if (SUCCEEDED(m_pcContextTemporary->Map((ID3D11Resource*)m_pcMirrorCopy, 0, D3D11_MAP_READ, 0, &sMappedResource)))
		{
			// create the frame mirror texture by back buffer description
			D3D11_TEXTURE2D_DESC sDesc;
			tex->D3D11.pTexture->GetDesc(&sDesc);
			sDesc.Usage = D3D11_USAGE_DEFAULT;
			sDesc.CPUAccessFlags = 0;
			D3D11_SUBRESOURCE_DATA sData;
			ZeroMemory(&sData, sizeof(sData));
			sData.pSysMem = sMappedResource.pData;
			sData.SysMemPitch = sMappedResource.RowPitch;
			if (FAILED(pcDevice->CreateTexture2D( &sDesc, &sData, &pcTextureMirror )))
				OutputDebugString(L"Failed to create Texture.");

			// create the frame mirror texture view
			if (pcTextureMirror)
			{
				D3D11_SHADER_RESOURCE_VIEW_DESC sDesc1;
				ZeroMemory(&sDesc1, sizeof(sDesc1));
				sDesc1.Format = sDesc.Format;
				sDesc1.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
				sDesc1.Texture2D.MostDetailedMip = 0;
				sDesc1.Texture2D.MipLevels = 1;

				if ((FAILED(pcDevice->CreateShaderResourceView((ID3D11Resource*)pcTextureMirror, &sDesc1, &pcTextureViewMirror))))
					OutputDebugString(L"Failed to create texture view! (mirror)");
			}

			m_pcContextTemporary->Unmap((ID3D11Resource*)m_pcMirrorCopy, 0);
		}
		pcContext->PSSetShaderResources(0, 1, &pcTextureViewMirror);

		// set shaders
		pcContext->VSSetShader(m_pcVertexShaderMirror, 0, 0);
		pcContext->PSSetShader(m_pcPixelShaderMirror, 0, 0);

		// Render a triangle
		pcContext->Draw( 6, 0 );

		// release textures
		if (pcTextureViewMirror) pcTextureViewMirror->Release();
		if (pcTextureMirror) pcTextureMirror->Release();

		// finish up
		for (int i = 0; i < D3D11_SIMULTANEOUS_RENDER_TARGET_COUNT; i++)
		{
			if (ppRenderTargetView[i]) { ppRenderTargetView[i]->Release(); ppRenderTargetView[i] = nullptr; }
		}
		if (pDepthStencilView) { pDepthStencilView->Release(); pDepthStencilView = nullptr; }
#pragma endregion
	}

	// release frame texture+view
	if (m_pcTextureViewDirect) { m_pcTextureViewDirect->Release(); m_pcTextureViewDirect = nullptr; }
	if (m_pcTextureDirect) { m_pcTextureDirect->Release(); m_pcTextureDirect = nullptr; }

	// apply state block
	ApplyStateblock(pcContext, &sStateBlock);

	// release d3d11 device + context... 
	pcContext->Release();
	pcDevice->Release();

	return nullptr;
}