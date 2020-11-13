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
#ifndef AQU_NODES
#define AQU_NODES

#pragma region Node Macros
#define AQU_D3D_NODE_HEADER___(cl,w,h) class cl : public NOD_Basic { public: cl(LONG nX, LONG nY):NOD_Basic(nX, nY, w, h) { m_paCommanders.clear();
#define AQU_D3D_NODE_TITLE____(t) { m_szTitle = t; m_eNodeProvokingType = AQU_NodeProvokingType::OnlyProvoker; }
#define AQU_D3D_NODE_COMMANDER(pt,t) { NOD_Commander * pC = new NOD_Commander(); pC->m_ePlugtype = NOD_Plugtype::pt; pC->m_szTitle = t;	pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC);	}
#define AQU_D3D_NODE_METHOD___(d,i,m) { m_cProvoker.m_eD3D = AQU_Direct3DVersion::d; m_cProvoker.m_eD3DInterface = i; m_cProvoker.m_eD3DMethod = m; } }  virtual UINT GetNodeTypeId() { return (((UINT)d)<<24)+(((UINT)i)<<16)+(UINT)m; }
#define AQU_D3D_NODE_FOOTER___ virtual const char* GetNodeType() { return typeid(this).name(); } };
#pragma endregion

#pragma region Elementary Nodes Data
/**
* Elementary Nodes Enumeration.
***/
enum AQU_ElementaryNodes
{
	EN_FALSE,
	EN_TRUE,
	EN_FIXEDFLOAT,
	EN_FIXEDWSTRING,
	EN_FIXEDPOINTER,
	EN_FIXEDINT,
	EN_CONFIGURATION,
	EN_HELLOWORLDDX9,
	EN_HELLOWORLDDX10,
	EN_MATHLOOKAT,
	EN_DEBUGD3DVECTOR,
	EN_DEBUGD3DMATRIX,
	EN_FRAMEDECALDX9,
	EN_D3DX9,
	EN_NUMBER_OF_ELEMENTARY_NODES = 14
};

#pragma endregion

#include"NOD_Basic.h"
#include"NOD_FixedValues.h"
#include"NOD_MathOperations.h"
#include"NOD_DebugOutput.h"
#include"NOD_Plugin.h"
#include"NOD_HelloWorldDx9.h"
#include"NOD_HelloWorldDx10.h"
#include"NOD_FrameDecalDx9.h"
#include"NOD_IDirect3DDevice9.h"
#include"NOD_IDirect3DBaseTexture9.h"
#include"NOD_IDirect3DCubeTexture9.h"
#include"NOD_IDirect3DDevice9Ex.h"
#include"NOD_IDirect3DIndexBuffer9.h"
#include"NOD_IDirect3DPixelShader9.h"
#include"NOD_IDirect3DQuery9.h"
#include"NOD_IDirect3DResource9.h"
#include"NOD_IDirect3DStateBlock9.h"
#include"NOD_IDirect3DSurface9.h"
#include"NOD_IDirect3DSwapChain9.h"
#include"NOD_IDirect3DTexture9.h"
#include"NOD_IDirect3DVertexBuffer9.h"
#include"NOD_IDirect3DVertexDeclaration9.h"
#include"NOD_IDirect3DVertexShader9.h"
#include"NOD_IDirect3DVolume9.h"
#include"NOD_IDirect3DVolumeTexture9.h"
#include"NOD_D3DX9.h"
#include"NOD_ID3D10Device.h"
#include"NOD_ID3D10Device1.h"
#include"NOD_ID3D11Device.h"
#include"NOD_ID3D11Device1.h"
#include"NOD_ID3D11Device2.h" //#include"NOD_ID3D11Device3.h"
#include"NOD_IDXGISwapChain.h"
#include"NOD_IDXGISwapChain1.h"
#include"NOD_IDXGISwapChain2.h"	//#include"NOD_IDXGISwapChain3.h"
#include"NOD_ID3D11DeviceContext.h"
#include"NOD_ID3D11DeviceContext1.h"
#include"NOD_ID3D11DeviceContext2.h" //#include"NOD_ID3D11DeviceContext3.h"

/**
* All interfaces node provider.
***/
class AQU_NodeProvider
{
public:
	/**
	* Provides a direct3d node by index.
	***/
	NOD_Basic* Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces eInterface, UINT dwIndex, LONG nX, LONG nY)
	{
		NOD_Basic* ret = nullptr;

		switch (eInterface)
		{
		case AQU_SUPPORTEDINTERFACES::IDirect3DBaseTexture9:
			{
				NOD_IDirect3DBaseTexture9* pInterfaceNodeProvider = new NOD_IDirect3DBaseTexture9();
				ret = pInterfaceNodeProvider->Get_IDirect3DBaseTexture9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DCubeTexture9:
			{
				NOD_IDirect3DCubeTexture9* pInterfaceNodeProvider = new NOD_IDirect3DCubeTexture9();
				ret = pInterfaceNodeProvider->Get_IDirect3DCubeTexture9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DDevice9:
			{
				NOD_IDirect3DDevice9* pInterfaceNodeProvider = new NOD_IDirect3DDevice9();
				ret = pInterfaceNodeProvider->Get_IDirect3DDevice9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DIndexBuffer9:
			{
				NOD_IDirect3DIndexBuffer9* pInterfaceNodeProvider = new NOD_IDirect3DIndexBuffer9();
				ret = pInterfaceNodeProvider->Get_IDirect3DIndexBuffer9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DPixelShader9:
			{
				NOD_IDirect3DPixelShader9* pInterfaceNodeProvider = new NOD_IDirect3DPixelShader9();
				ret = pInterfaceNodeProvider->Get_IDirect3DPixelShader9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DQuery9:
			{
				NOD_IDirect3DQuery9* pInterfaceNodeProvider = new NOD_IDirect3DQuery9();
				ret = pInterfaceNodeProvider->Get_IDirect3DQuery9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DResource9:
			{
				NOD_IDirect3DResource9* pInterfaceNodeProvider = new NOD_IDirect3DResource9();
				ret = pInterfaceNodeProvider->Get_IDirect3DResource9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DStateBlock9:
			{
				NOD_IDirect3DStateBlock9* pInterfaceNodeProvider = new NOD_IDirect3DStateBlock9();
				ret = pInterfaceNodeProvider->Get_IDirect3DStateBlock9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DSurface9:
			{
				NOD_IDirect3DSurface9* pInterfaceNodeProvider = new NOD_IDirect3DSurface9();
				ret = pInterfaceNodeProvider->Get_IDirect3DSurface9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DSwapChain9:
			{
				NOD_IDirect3DSwapChain9* pInterfaceNodeProvider = new NOD_IDirect3DSwapChain9();
				ret = pInterfaceNodeProvider->Get_IDirect3DSwapChain9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DTexture9:
			{
				NOD_IDirect3DTexture9* pInterfaceNodeProvider = new NOD_IDirect3DTexture9();
				ret = pInterfaceNodeProvider->Get_IDirect3DTexture9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DVertexBuffer9:
			{
				NOD_IDirect3DVertexBuffer9* pInterfaceNodeProvider = new NOD_IDirect3DVertexBuffer9();
				ret = pInterfaceNodeProvider->Get_IDirect3DVertexBuffer9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DVertexDeclaration9:
			{
				NOD_IDirect3DVertexDeclaration9* pInterfaceNodeProvider = new NOD_IDirect3DVertexDeclaration9();
				ret = pInterfaceNodeProvider->Get_IDirect3DVertexDeclaration9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DVertexShader9:
			{
				NOD_IDirect3DVertexShader9* pInterfaceNodeProvider = new NOD_IDirect3DVertexShader9();
				ret = pInterfaceNodeProvider->Get_IDirect3DVertexShader9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DVolume9:
			{
				NOD_IDirect3DVolume9* pInterfaceNodeProvider = new NOD_IDirect3DVolume9();
				ret = pInterfaceNodeProvider->Get_IDirect3DVolume9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DVolumeTexture9:
			{
				NOD_IDirect3DVolumeTexture9* pInterfaceNodeProvider = new NOD_IDirect3DVolumeTexture9();
				ret = pInterfaceNodeProvider->Get_IDirect3DVolumeTexture9_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDirect3DDevice9Ex:
			{
				NOD_IDirect3DDevice9Ex* pInterfaceNodeProvider = new NOD_IDirect3DDevice9Ex();
				ret = pInterfaceNodeProvider->Get_IDirect3DDevice9Ex_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::ID3D10Device:
			{
				NOD_ID3D10Device* pInterfaceNodeProvider = new NOD_ID3D10Device();
				ret = pInterfaceNodeProvider->Get_ID3D10Device_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::ID3D10Device1:
			break;
		case AQU_SUPPORTEDINTERFACES::ID3D11Device:
			{
				NOD_ID3D11Device* pInterfaceNodeProvider = new NOD_ID3D11Device();
				ret = pInterfaceNodeProvider->Get_ID3D11Device_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::ID3D11Device1:
			break;
		case AQU_SUPPORTEDINTERFACES::ID3D11Device2:
			break;
		case AQU_SUPPORTEDINTERFACES::ID3D11Device3:
			break;
		case AQU_SUPPORTEDINTERFACES::IDXGISwapChain:
			{
				NOD_IDXGISwapChain* pInterfaceNodeProvider = new NOD_IDXGISwapChain();
				ret = pInterfaceNodeProvider->Get_IDXGISwapChain_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::IDXGISwapChain1:
			break;
		case AQU_SUPPORTEDINTERFACES::IDXGISwapChain2:
			break;
		case AQU_SUPPORTEDINTERFACES::IDXGISwapChain3:
			break;
		case AQU_SUPPORTEDINTERFACES::ID3D11DeviceContext:
			{
				NOD_ID3D11DeviceContext* pInterfaceNodeProvider = new NOD_ID3D11DeviceContext();
				ret = pInterfaceNodeProvider->Get_ID3D11DeviceContext_Node(dwIndex, nX, nY);
				delete pInterfaceNodeProvider;
			}
			return ret;
			break;
		case AQU_SUPPORTEDINTERFACES::ID3D11DeviceContext1:
			break;
		case AQU_SUPPORTEDINTERFACES::ID3D11DeviceContext2:
			break;
		case AQU_SUPPORTEDINTERFACES::ID3D11DeviceContext3:
			break;
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides an elementary node.
	***/
	NOD_Basic* Get_Elementary_Node(AQU_ElementaryNodes eName, LONG nX, LONG nY)
	{
		switch (eName)
		{
		case AQU_ElementaryNodes::EN_FALSE:
			return (NOD_Basic*)new NOD_False(nX, nY);
		case AQU_ElementaryNodes::EN_TRUE:
			return (NOD_Basic*)new NOD_True(nX, nY);
		case AQU_ElementaryNodes::EN_FIXEDFLOAT:
			return (NOD_Basic*)new NOD_FixedFloat(nX, nY);
		case AQU_ElementaryNodes::EN_FIXEDWSTRING:
			return (NOD_Basic*)new NOD_FixedWString(nX, nY);
		case AQU_ElementaryNodes::EN_FIXEDINT:
			return (NOD_Basic*)new NOD_FixedInt(nX, nY);
		case AQU_ElementaryNodes::EN_FIXEDPOINTER:
			return (NOD_Basic*)new NOD_FixedPointer(nX, nY);
		case AQU_ElementaryNodes::EN_CONFIGURATION:
			return (NOD_Basic*)new NOD_AquilinusConfig(nX, nY);
		case AQU_ElementaryNodes::EN_HELLOWORLDDX9:
			return (NOD_Basic*)new NOD_HelloWorldDx9(nX, nY);
		case AQU_ElementaryNodes::EN_HELLOWORLDDX10:
			return (NOD_Basic*)new NOD_HelloWorldDx10(nX, nY);
		case AQU_ElementaryNodes::EN_MATHLOOKAT:
			return (NOD_Basic*)new NOD_MathLookat(nX, nY);
		case AQU_ElementaryNodes::EN_DEBUGD3DVECTOR:
			return (NOD_Basic*)new NOD_OutputDebugVector(nX, nY);
		case AQU_ElementaryNodes::EN_DEBUGD3DMATRIX:
			return (NOD_Basic*)new NOD_OutputDebugMatrix(nX, nY);
		case AQU_ElementaryNodes::EN_FRAMEDECALDX9:
			return (NOD_Basic*)new NOD_FrameDecalDx9(nX, nY);
		case AQU_ElementaryNodes::EN_D3DX9:
			return (NOD_Basic*)new NOD_D3DX9(nX, nY);
		default:
			break;
		}

		return nullptr;
	}

	/**
	* Provides a node by id.
	* For Plugin nodes the pluginId and filename are also needed.
	***/
	HRESULT Get_Node(NOD_Basic* &psNode, UINT dwID, LONG nX, LONG nY, UINT dwPluginID, LPCWSTR szFileName)
	{
		// set null pointer for safety
		psNode = nullptr;

		// is a plugin node ?
		if (dwID == ELEMENTARY_NODE_PLUGIN)
		{
			// create plugin node
			psNode = new NOD_Plugin(nX, nY, (LPWSTR)szFileName);

			// is the plugin id correct ?
			if (((NOD_Plugin*)psNode)->GetPluginNodeTypeId() != dwPluginID)
			{
				delete psNode;
				psNode = nullptr;
			}

			return S_OK;
		}

		// read the interface
		UINT d = dwID >> 24;
		UINT i = (dwID >> 16) & 0x00ff;
		UINT j = dwID & 0x0000ffff;

		// get the node (D3D)
		if (d == DirectX_9_0)
		{
			switch(i)
			{
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DBaseTexture9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DBaseTexture9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DCubeTexture9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DCubeTexture9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DIndexBuffer9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DIndexBuffer9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DPixelShader9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DPixelShader9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DQuery9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DQuery9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DResource9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DResource9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DStateBlock9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DStateBlock9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSurface9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSurface9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DSwapChain9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DSwapChain9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DTexture9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DTexture9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVertexBuffer9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexBuffer9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVertexDeclaration9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexDeclaration9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVertexShader9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVertexShader9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVolume9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVolume9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DVolumeTexture9:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DVolumeTexture9, j, nX, nY);
				break;
			case (UINT)ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9Ex:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDirect3DDevice9Ex, j, nX, nY);
				break;
			}
		} 
		else if (d == DirectX_10)
		{
			switch(i)
			{
			case (UINT)ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D10Device, j, nX, nY);
				break;
			case (UINT)ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain, j, nX, nY);
				break;
			case (UINT)ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain1:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain1, j, nX, nY);
				break;
			case (UINT)ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain2:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain2, j, nX, nY);
				break;
			case (UINT)ITA_DXGIINTERFACES::ITA_DXGIInterfaces::IDXGISwapChain3:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::IDXGISwapChain3, j, nX, nY);
				break;
			}
		}
		else if(d == DirectX_10_1)
		{
			switch(i)
			{
			case (UINT)ITA_D3D10INTERFACES::ITA_D3D10Interfaces::ID3D10Device1:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D10Device1, j, nX, nY);
				break;
			}
		}
		else if (d == DirectX_11)
		{
			switch(i)
			{
			case (UINT)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device, j, nX, nY);
				break;
			case (UINT)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext, j, nX, nY);
				break;
			}
		}
		else if (d == DirectX_11_1)
		{
			switch(i)
			{
			case (UINT)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device1:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device1, j, nX, nY);
				break;
			case (UINT)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext1:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext1, j, nX, nY);
				break;
			}
		}
		else if (d == DirectX_11_2)
		{
			switch(i)
			{
			case (UINT)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11Device2:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11Device2, j, nX, nY);
				break;
			case (UINT)ITA_D3D11INTERFACES::ITA_D3D11Interfaces::ID3D11DeviceContext2:
				psNode = Get_D3D_Node(AQU_SUPPORTEDINTERFACES::AQU_SupportedInterfaces::ID3D11DeviceContext2, j, nX, nY);
				break;
			}
		}

		// get the node (fixed)
		if (psNode)	return S_OK; 
		else
		{
			switch(dwID)
			{
			case ELEMENTARY_NODE_FALSE:
				psNode = new NOD_False(nX, nY);
				return S_OK;
			case ELEMENTARY_NODE_TRUE:
				psNode = new NOD_True(nX, nY);
				return S_OK;
			case ELEMENTARY_NODE_FIXED_FLOAT:
				psNode = new NOD_FixedFloat(nX, nY);
				return S_OK;
			case ELEMENTARY_NODE_FIXED_WSTRING:
				psNode = new NOD_FixedWString(nX, nY);
				return S_OK;
			case ELEMENTARY_NODE_FIXED_INT:
				psNode = new NOD_FixedInt(nX, nY);
				return S_OK;
			case ELEMENTARY_NODE_FIXED_POINTER:
				psNode = new NOD_FixedPointer(nX, nY);
				return S_OK;
			case ELEMENTARY_NODE_CONFIGURATION:
				psNode = new NOD_AquilinusConfig(nX, nY);
				return S_OK;
			case ELEMENTARY_NODE_HELLO_WORLD_DX9:
				psNode = new NOD_HelloWorldDx9(nX, nY);
				return S_OK;
			case ELEMENTARY_NODE_HELLO_WORLD_DX10:
				psNode = new NOD_HelloWorldDx10(nX, nY);
				return S_OK;
			case ELEMENTARY_NODE_MATH_LOOKAT:
				psNode = new NOD_MathLookat(nX, nY);
				return S_OK;
			case ELEMENTARY_NODE_DEBUG_D3DVECTOR:
				psNode = new NOD_OutputDebugVector(nX, nY);
				return S_OK;
			case ELEMENTARY_NODE_DEBUG_D3DMATRIX:
				psNode = new NOD_OutputDebugMatrix(nX, nY);
				return S_OK;
			case D3DOPS_NODE_FRAMEDECAL:
				psNode = new NOD_FrameDecalDx9(nX, nY);
				return S_OK;
			case D3DOPS_NODE_D3DX9:
				psNode = new NOD_D3DX9(nX, nY);
				return S_OK;
			}
			return E_FAIL;
		}
	}

	/**
	* Provides the name of an elementary node.
	***/
	LPCWSTR Get_Elementary_Node_Name(AQU_ElementaryNodes eName)
	{
		switch (eName)
		{
		case AQU_ElementaryNodes::EN_FALSE:
			return L"False Node";
		case AQU_ElementaryNodes::EN_TRUE:
			return L"True Node";
		case AQU_ElementaryNodes::EN_FIXEDFLOAT:
			return L"Fixed Float";
		case AQU_ElementaryNodes::EN_FIXEDWSTRING:
			return L"Fixed W String";
		case AQU_ElementaryNodes::EN_FIXEDINT:
			return L"Fixed Int32";
		case AQU_ElementaryNodes::EN_FIXEDPOINTER:
			return L"Fixed Pointer";
		case AQU_ElementaryNodes::EN_CONFIGURATION:
			return L"Game Configuration";
		case AQU_ElementaryNodes::EN_HELLOWORLDDX9:
			return L"Hello World D3D 9.x";
		case AQU_ElementaryNodes::EN_HELLOWORLDDX10:
			return L"Hello World D3D 10.x";
		case AQU_ElementaryNodes::EN_MATHLOOKAT:
			return L"Math Lookat";
		case AQU_ElementaryNodes::EN_DEBUGD3DVECTOR:
			return L"Debug D3DVector";
		case AQU_ElementaryNodes::EN_DEBUGD3DMATRIX:
			return L"Debug D3DMatrix";
		case AQU_ElementaryNodes::EN_FRAMEDECALDX9:
			return L"Frame Decal D3D 9.x";
		case AQU_ElementaryNodes::EN_D3DX9:
			return L"D3DX9";
		default:
			break;
		}

		return L"Unknown Node Type";
	}
};

#endif