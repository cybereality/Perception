/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Constructor - Vireio Perception Direct 3D Device Handler (DX11)
Copyright (C) 2015 Denis Reischl

File <VireioConstructorDx11.h> and
Class <VireioConstructorDx11> :
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

#include"AQU_Nodus.h"
#include"Resources.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include"..\..\VireioMatrixModifier\VireioMatrixModifier\VireioMatrixModifierDataStructures.h"

#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include"..\..\..\Include\Vireio_GUIDs.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"

#define	PROVOKING_TYPE                                 2                     /**< Provoking type is 2 - just invoker, no provoker **/
#define METHOD_REPLACEMENT                         false                     /**< This node does NOT replace the D3D call (default) **/

#define NUMBER_OF_DECOMMANDERS                         12

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }

/**
* Node Commander Enumeration.
***/
enum STS_Decommanders
{
	pShaderBytecode_VertexShader,            /**< ID3D11Device::CreateVertexShader ***/
	BytecodeLength_VertexShader,             /**< ID3D11Device::CreateVertexShader ***/
	pClassLinkage_VertexShader,              /**< ID3D11Device::CreateVertexShader ***/
	ppVertexShader_DX11,                     /**< ID3D11Device::CreateVertexShader ***/
	pShaderBytecode_PixelShader,             /**< ID3D11Device::CreatePixelShader ***/
	BytecodeLength_PixelShader,              /**< ID3D11Device::CreatePixelShader ***/
	pClassLinkage_PixelShader,               /**< ID3D11Device::CreatePixelShader **/
	ppPixelShader_DX11,                      /**< ID3D11Device::CreatePixelShader ***/
	pDesc_DX11,                              /**< ID3D11Device::CreateBuffer ***/
	pInitialData_DX11,                       /**< ID3D11Device::CreateBuffer ***/
	ppBuffer_DX11,                           /**< ID3D11Device::CreateBuffer ***/
	asShaderData,                            /**< The shader data vector initialized in the matrix modifier ***/
};

/**
* Vireio Perception Direct 3D Device Handler (DX11).
* Vireio Perception Constructor handling all D3D Device methods.
***/
class VireioConstructorDx11 : public AQU_Nodus
{
public:
	VireioConstructorDx11();
	virtual ~VireioConstructorDx11();

	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4 + 256 + 4; }
	virtual DWORD           GetNodeHeight() { return 128; }
	virtual int             GetProvokingType() { return PROVOKING_TYPE; }
	virtual bool            GetMethodReplacement() { return METHOD_REPLACEMENT; }
	virtual DWORD           GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex);
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);

	/**
	* Return value pointer (HRESULT).
	***/
	void* m_pvReturn;

private:

	/*** Constructor private methods ***/
	void CreateStereoConstantBuffer(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, ID3D11Buffer* pcBuffer, D3D11_BUFFER_DESC *pDesc, D3D11_SUBRESOURCE_DATA *pInitialData, bool bCopyData);

	/*** Constructor input pointers ***/
	void** m_ppvShaderBytecode_VertexShader;
	SIZE_T* m_pnBytecodeLength_VertexShader;
	ID3D11ClassLinkage** m_ppcClassLinkage_VertexShader;
	ID3D11VertexShader*** m_pppcVertexShader_DX11;
	void** m_ppvShaderBytecode_PixelShader;
	SIZE_T* m_pnBytecodeLength_PixelShader;
	ID3D11ClassLinkage** m_ppcClassLinkage_PixelShader;
	ID3D11PixelShader*** m_pppcPixelShader_DX11;
	ID3D11VertexShader** m_ppcVertexShader_11;
	ID3D11VertexShader** m_ppcPixelShader_11;
	D3D11_BUFFER_DESC** m_ppsDesc_DX11;
	D3D11_SUBRESOURCE_DATA** m_ppsInitialData_DX11;
	ID3D11Buffer*** m_pppcBuffer_DX11;

	/**
	* The d3d11 shader description vector.
	* Contains all enumerated shader data structures.
	***/
	std::vector<Vireio_D3D11_Shader>* m_pasShaders;
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	VireioConstructorDx11* pVireioConstructorDx11 = new VireioConstructorDx11();
	return static_cast<AQU_Nodus*>(pVireioConstructorDx11);
}
