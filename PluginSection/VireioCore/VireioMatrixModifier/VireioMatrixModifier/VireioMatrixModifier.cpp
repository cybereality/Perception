/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Matrix Modifier - Vireio Stereo Matrix Modification Node
Copyright (C) 2015 Denis Reischl

File <VireioMatrixModifier.cpp> and
Class <VireioMatrixModifier> :
Copyright (C) 2015 Denis Reischl

Parts of this class directly derive from Vireio source code originally
authored by Chris Drain (v1.1.x 2013).

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

#define DEBUG_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%u", a); OutputDebugString(buf); }
#define DEBUG_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%x", a); OutputDebugString(buf); }

#include"VireioMatrixModifier.h"

#define INTERFACE_ID3D11DEVICE                                               6
#define INTERFACE_ID3D10DEVICE                                               7
#define INTERFACE_ID3D11DEVICECONTEXT                                        11
#define INTERFACE_IDXGISWAPCHAIN                                             29

#define METHOD_IDXGISWAPCHAIN_PRESENT                                        8
#define METHOD_ID3D11DEVICE_CREATEBUFFER                                     3
#define METHOD_ID3D11DEVICE_CREATEVERTEXSHADER                               12
#define METHOD_ID3D11DEVICECONTEXT_VSSETCONSTANTBUFFERS                      7
#define METHOD_ID3D11DEVICECONTEXT_PSSETSHADER                               9
#define METHOD_ID3D11DEVICECONTEXT_VSSETSHADER                               11
#define METHOD_ID3D11DEVICECONTEXT_PSSETCONSTANTBUFFERS                      16
#define METHOD_ID3D11DEVICECONTEXT_COPYSUBRESOURCEREGION                     46
#define METHOD_ID3D11DEVICECONTEXT_COPYRESOURCE                              47
#define METHOD_ID3D11DEVICECONTEXT_UPDATESUBRESOURCE                         48
#define METHOD_ID3D11DEVICECONTEXT_VSGETCONSTANTBUFFERS                      72
#define METHOD_ID3D11DEVICECONTEXT_PSGETCONSTANTBUFFERS                      77
#define METHOD_ID3D10DEVICE_COPYSUBRESOURCEREGION                            32
#define METHOD_ID3D10DEVICE_COPYRESOURCE                                     33
#define METHOD_ID3D10DEVICE_UPDATESUBRESOURCE                                34

#define SAFE_RELEASE(a) if (a) { a->Release(); a = nullptr; }

/**
* Constructor.
***/
MatrixModifier::MatrixModifier() : AQU_Nodus()
{
	// create a new HRESULT pointer
	m_pvReturn = (void*)new HRESULT();

	// create first matrices.... TODO !! include class from main driver
	float fAspectRatio = 1920.0f / 1080.0f;
	float n = 0.1f;     /**< Minimum z-value of the view volume. */
	float f = 10.0f;    /**< Maximum z-value of the view volume. */
	float l = -0.5f;    /**< Minimum x-value of the view volume. */
	float r = 0.5f;     /**< Maximum x-value of the view volume. */
	float t = 0.5f / fAspectRatio;   /**< Minimum y-value of the view volume. */
	float b = -0.5f / fAspectRatio;  /**< Maximum y-value of the view volume. */
	D3DXMatrixPerspectiveOffCenterLH(&matProjection, l, r, b, t, n, f);
	D3DXMatrixInverse(&matProjectionInv, 0, &matProjection);

	// drawing side set to left... changed only in VireioStereioSplitter::SetDrawingSide()
	m_eCurrentRenderingSide = RenderPosition::Left;

	// ALL stated in meters here ! screen size = horizontal size

	// assumption here :
	// end user is placed 1 meter away from screen
	// end user screen is 1 meter in horizontal size
	float nearClippingPlaneDistance = 1;
	float physicalScreenSizeInMeters = 1;
	float convergence = 3.0f;
	float ipd = 0.064f;

	// convergence frustum adjustment, based on NVidia explanations
	//
	// It is evident that the ratio of frustum shift to the near clipping plane is equal to the ratio of 
	// IOD/2 to the distance from the screenplane. (IOD=IPD) 
	// frustumAsymmetryInMeters = ((IPD/2) * nearClippingPlaneDistance) / convergence
	// <http://www.orthostereo.com/geometryopengl.html>
	//
	// (near clipping plane distance = physical screen distance)
	// (convergence = virtual screen distance)
	if (convergence <= nearClippingPlaneDistance) convergence = nearClippingPlaneDistance + 0.001f;
	float frustumAsymmetryInMeters = ((ipd / 2) * nearClippingPlaneDistance) / convergence;

	// divide the frustum asymmetry by the assumed physical size of the physical screen
	float frustumAsymmetryLeftInMeters = (frustumAsymmetryInMeters * -1.0f) / physicalScreenSizeInMeters;
	float frustumAsymmetryRightInMeters = (frustumAsymmetryInMeters * 1.0f) / physicalScreenSizeInMeters;

	// get the horizontal screen space size and compute screen space adjustment
	float screenSpaceXSize = abs(l) + abs(r);
	float multiplier = screenSpaceXSize / 1; // = 1 meter
	float frustumAsymmetryLeft = frustumAsymmetryLeftInMeters * multiplier;
	float frustumAsymmetryRight = frustumAsymmetryRightInMeters * multiplier;

	// now, create the re-projection matrices for both eyes using this frustum asymmetry
	D3DXMatrixPerspectiveOffCenterLH(&projectLeft, l + frustumAsymmetryLeft, r + frustumAsymmetryLeft, b, t, n, f);
	D3DXMatrixPerspectiveOffCenterLH(&projectRight, l + frustumAsymmetryRight, r + frustumAsymmetryRight, b, t, n, f);

	// compute view transforms
	ComputeViewTransforms(-1.0f);

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	// create buffer vectors
	m_apcActiveConstantBuffers11 = std::vector<ID3D11Buffer*>(D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT, nullptr);

	// create output pointers
	m_pvOutput[STS_Commanders::ppActiveConstantBuffers_DX11_VertexShader] = (void*)&m_apcActiveConstantBuffers11[0];
#endif
}

/**
* Destructor.
***/
MatrixModifier::~MatrixModifier()
{
}

/**
* Return the name of the Matrix Modifier node.
***/
const char* MatrixModifier::GetNodeType()
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	return "Matrix ModifierDx10";
#elif defined(VIREIO_D3D9)
	return "Matrix Modifier";
#endif
}

/**
* Returns a global unique identifier for the Matrix Modifier node.
***/
UINT MatrixModifier::GetNodeTypeId()
{
#define DEVELOPER_IDENTIFIER 2006
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
#define MY_PLUGIN_IDENTIFIER 76
#elif defined(VIREIO_D3D9)
#define MY_PLUGIN_IDENTIFIER 75
#endif
	return ((DEVELOPER_IDENTIFIER << 16) + MY_PLUGIN_IDENTIFIER);
}

/**
* Returns the name of the category for the Matrix Modifier node.
***/
LPWSTR MatrixModifier::GetCategory()
{
	return L"Vireio Core";
}

/**
* Returns a logo to be used for the Matrix Modifier node.
***/
HBITMAP MatrixModifier::GetLogo()
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	HMODULE hModule = GetModuleHandle(L"VireioMatrixModifierDx10.dll");
#elif defined(VIREIO_D3D9)
	HMODULE hModule = GetModuleHandle(L"VireioMatrixModifier.dll");
#endif
	HBITMAP hBitmap = LoadBitmap(hModule, MAKEINTRESOURCE(IMG_LOGO01));
	return hBitmap;
}

/**
* Returns the updated control for the Matrix Modifier node.
* Allways return >nullptr< if there is no update for the control !!
***/
HBITMAP MatrixModifier::GetControl()
{
	return nullptr;
}

/**
* Provides the name of the requested commander.
***/
LPWSTR MatrixModifier::GetCommanderName(DWORD dwCommanderIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Commanders)dwCommanderIndex)
	{
		case eDrawingSide:
			return L"Stereo Drawing Side";
		case ppActiveConstantBuffers_DX10_VertexShader:
			return L"ppConstantBuffers_DX10_VS";
		case ppActiveConstantBuffers_DX11_VertexShader:
			return L"ppConstantBuffers_DX11_VS";
		case ppActiveConstantBuffers_DX10_PixelShader:
			return L"ppConstantBuffers_DX10_PS";
		case ppActiveConstantBuffers_DX11_PixelShader:
			return L"ppConstantBuffers_DX11_PS";
		default:
			break;
	}
#elif defined(VIREIO_D3D9)
#endif

	return L"UNTITLED";
}

/**
* Provides the name of the requested decommander.
***/
LPWSTR MatrixModifier::GetDecommanderName(DWORD dwDecommanderIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShaderBytecode_VertexShader:
			return L"pShaderBytecode_VS";
		case BytecodeLength_VertexShader:
			return L"BytecodeLength_VS";
		case pClassLinkage_VertexShader:
			return L"pClassLinkage_VertexShader";
		case ppVertexShader_DX10:
			return L"ppVertexShader_DX10";
		case ppVertexShader_DX11:
			return L"ppVertexShader_DX11";
		case pShaderBytecode_PixelShader:
			return L"pShaderBytecode_PS";
		case BytecodeLength_PixelShader:
			return L"BytecodeLength_PS";
		case pClassLinkage_PixelShader:
			return L"pClassLinkage_PixelShader";
		case ppPixelShader_DX10:
			return L"ppPixelShader_DX10";
		case ppPixelShader_DX11:
			return L"ppPixelShader_DX11";
		case pVertexShader_10:
			return L"pVertexShader_10";
		case pVertexShader_11:
			return L"pVertexShader_11";
		case pPixelShader_10:
			return L"pPixelShader_10";
		case pPixelShader_11:
			return L"pPixelShader_11";
		case pDesc_DX10:
			return L"pDesc_DX10";
		case pInitialData_DX10:
			return L"pInitialData_DX10";
		case ppBuffer_DX10:
			return L"ppBuffer_DX10";
		case pDesc_DX11:
			return L"pDesc_DX11";
		case pInitialData_DX11:
			return L"pInitialData_DX11";
		case ppBuffer_DX11:
			return L"ppBuffer_DX11";
		case StartSlot_VertexShader:
			return L"StartSlot_VS";
		case NumBuffers_VertexShader:
			return L"NumBuffers_VS";
		case ppConstantBuffers_DX10_VertexShader:
			return L"ppConstantBuffers_DX10_VS";
		case ppConstantBuffers_DX11_VertexShader:
			return L"ppConstantBuffers_DX11_VS";
		case StartSlot_PixelShader:
			return L"StartSlot_PS";
		case NumBuffers_PixelShader:
			return L"NumBuffers_PS";
		case ppConstantBuffers_DX10_PixelShader:
			return L"ppConstantBuffers_DX10_PS";
		case ppConstantBuffers_DX11_PixelShader:
			return L"ppConstantBuffers_DX11_PS";
		case pDstResource_DX10:
			return L"pDstResource_DX10";
		case pDstResource_DX11:
			return L"pDstResource_DX11";
		case DstSubresource:
			return L"DstSubresource";
		case pDstBox_DX10:
			return L"pDstBox_DX10";
		case pDstBox_DX11:
			return L"pDstBox_DX11";
		case pSrcData:
			return L"pSrcData";
		case SrcRowPitch:
			return L"SrcRowPitch";
		case SrcDepthPitch:
			return L"SrcDepthPitch";
		case pDstResource_DX10_Copy:
			return L"pDstResource_DX10_Copy";
		case pSrcResource_DX10_Copy:
			return L"pSrcResource_DX10_Copy";
		case pDstResource_DX11_Copy:
			return L"pDstResource_DX11_Copy";
		case pSrcResource_DX11_Copy:
			return L"pSrcResource_DX11_Copy";
		case pDstResource_DX10_CopySub:
			return L"pDstResource_DX10_CopySub";
		case pDstResource_DX11_CopySub:
			return L"pDstResource_DX11_CopySub";
		case DstSubresource_CopySub:
			return L"DstSubresource_CopySub";
		case DstX:
			return L"DstX";
		case DstY:
			return L"DstY";
		case DstZ:
			return L"DstZ";
		case pSrcResource_DX10_CopySub:
			return L"pSrcResource_DX10_CopySub";
		case pSrcResource_DX11_CopySub:
			return L"pSrcResource_DX11_CopySub";
		case SrcSubresource:
			return L"SrcSubresource";
		case pSrcBox_DX10:
			return L"pSrcBox_DX10";
		case pSrcBox_DX11:
			return L"pSrcBox_DX11";
		default:
			return L"UNTITLED";
	}
#elif defined(VIREIO_D3D9)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShader_Vertex:
			return L"pShader_Vertex";
		case pShader_Pixel:
			return L"pShader_Pixel";
		case State:
			return L"State";
		case pMatrix:
			return L"pMatrix";
		case State_Multiply:
			return L"State_Multiply";
		case pMatrix_Multiply:
			return L"pMatrix_Multiply";
		case StartRegister_VertexShader:
			return L"StartRegister_VS";
		case pConstantData_VertexShader:
			return L"pConstantData_VS";
		case Vector4fCount_VertexShader:
			return L"Vector4fCount_VS";
		case StartRegister_PixelShader:
			return L"StartRegister_PS";
		case pConstantData_PixelShader:
			return L"pConstantData_PS";
		case Vector4fCount_PixelShader:
			return L"Vector4fCount_PS";
	}
#endif
	return L"UNTITLED";
}

/**
* Returns the plug type for the requested commander.
***/
DWORD MatrixModifier::GetCommanderType(DWORD dwCommanderIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Commanders)dwCommanderIndex)
	{
		case eDrawingSide:
			return NOD_Plugtype::AQU_INT;
		case ppActiveConstantBuffers_DX10_VertexShader:
			return NOD_Plugtype::AQU_PPNT_ID3D10BUFFER;
		case ppActiveConstantBuffers_DX11_VertexShader:
			return NOD_Plugtype::AQU_PPNT_ID3D11BUFFER;
		case ppActiveConstantBuffers_DX10_PixelShader:
			return NOD_Plugtype::AQU_PPNT_ID3D10BUFFER;
		case ppActiveConstantBuffers_DX11_PixelShader:
			return NOD_Plugtype::AQU_PPNT_ID3D11BUFFER;
		default:
			break;
	}
#elif defined(VIREIO_D3D9)
#endif

	return NULL;
}

/**
* Returns the plug type for the requested decommander.
***/
DWORD MatrixModifier::GetDecommanderType(DWORD dwDecommanderIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShaderBytecode_VertexShader:
			return NOD_Plugtype::AQU_PNT_VOID;
		case BytecodeLength_VertexShader:
			return NOD_Plugtype::AQU_SIZE_T;
		case pClassLinkage_VertexShader:
			return NOD_Plugtype::AQU_PNT_ID3D11CLASSLINKAGE;
		case ppVertexShader_DX10:
			return NOD_Plugtype::AQU_PPNT_ID3D10VERTEXSHADER;
		case ppVertexShader_DX11:
			return NOD_Plugtype::AQU_PPNT_ID3D11VERTEXSHADER;
		case pShaderBytecode_PixelShader:
			return NOD_Plugtype::AQU_PNT_VOID;
		case BytecodeLength_PixelShader:
			return NOD_Plugtype::AQU_SIZE_T;
		case pClassLinkage_PixelShader:
			return NOD_Plugtype::AQU_PNT_ID3D11CLASSLINKAGE;
		case ppPixelShader_DX10:
			return NOD_Plugtype::AQU_PPNT_ID3D10PIXELSHADER;
		case ppPixelShader_DX11:
			return NOD_Plugtype::AQU_PPNT_ID3D11PIXELSHADER;
		case pVertexShader_10:
			return NOD_Plugtype::AQU_PNT_ID3D10VERTEXSHADER;
		case pVertexShader_11:
			return NOD_Plugtype::AQU_PNT_ID3D11VERTEXSHADER;
		case pPixelShader_10:
			return NOD_Plugtype::AQU_PNT_ID3D10PIXELSHADER;
		case pPixelShader_11:
			return NOD_Plugtype::AQU_PNT_ID3D11VERTEXSHADER;
		case pDesc_DX10:
			return NOD_Plugtype::AQU_PNT_D3D10_BUFFER_DESC;
		case pInitialData_DX10:
			return NOD_Plugtype::AQU_PNT_D3D10_SUBRESOURCE_DATA;
		case ppBuffer_DX10:
			return NOD_Plugtype::AQU_PNT_ID3D10BUFFER;
		case pDesc_DX11:
			return NOD_Plugtype::AQU_PNT_D3D11_BUFFER_DESC;
		case pInitialData_DX11:
			return NOD_Plugtype::AQU_PNT_D3D11_SUBRESOURCE_DATA;
		case ppBuffer_DX11:
			return NOD_Plugtype::AQU_PPNT_ID3D11BUFFER;
		case StartSlot_VertexShader:
			return NOD_Plugtype::AQU_UINT;
		case NumBuffers_VertexShader:
			return NOD_Plugtype::AQU_UINT;
		case ppConstantBuffers_DX10_VertexShader:
			return NOD_Plugtype::AQU_PPNT_ID3D10BUFFER;
		case ppConstantBuffers_DX11_VertexShader:
			return NOD_Plugtype::AQU_PPNT_ID3D11BUFFER;
		case StartSlot_PixelShader:
			return NOD_Plugtype::AQU_UINT;
		case NumBuffers_PixelShader:
			return NOD_Plugtype::AQU_UINT;
		case ppConstantBuffers_DX10_PixelShader:
			return NOD_Plugtype::AQU_PPNT_ID3D10BUFFER;
		case ppConstantBuffers_DX11_PixelShader:
			return NOD_Plugtype::AQU_PPNT_ID3D11BUFFER;
		case pDstResource_DX10:
			return NOD_Plugtype::AQU_PNT_ID3D10RESOURCE;
		case pDstResource_DX11:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case DstSubresource:
			return NOD_Plugtype::AQU_UINT;
		case pDstBox_DX10:
			return NOD_Plugtype::AQU_PNT_D3D10_BOX;
		case pDstBox_DX11:
			return NOD_Plugtype::AQU_PNT_D3D11_BOX;
		case pSrcData:
			return NOD_Plugtype::AQU_PNT_VOID;
		case SrcRowPitch:
			return NOD_Plugtype::AQU_UINT;
		case SrcDepthPitch:
			return NOD_Plugtype::AQU_UINT;
		case pDstResource_DX10_Copy:
			return NOD_Plugtype::AQU_PNT_ID3D10RESOURCE;
		case pSrcResource_DX10_Copy:
			return NOD_Plugtype::AQU_PNT_ID3D10RESOURCE;
		case pDstResource_DX11_Copy:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case pSrcResource_DX11_Copy:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case pDstResource_DX10_CopySub:
			return NOD_Plugtype::AQU_PNT_ID3D10RESOURCE;
		case pDstResource_DX11_CopySub:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case DstSubresource_CopySub:
			return NOD_Plugtype::AQU_UINT;
		case DstX:
			return NOD_Plugtype::AQU_UINT;
		case DstY:
			return NOD_Plugtype::AQU_UINT;
		case DstZ:
			return NOD_Plugtype::AQU_UINT;
		case pSrcResource_DX10_CopySub:
			return NOD_Plugtype::AQU_PNT_ID3D10RESOURCE;
		case pSrcResource_DX11_CopySub:
			return NOD_Plugtype::AQU_PNT_ID3D11RESOURCE;
		case SrcSubresource:
			return NOD_Plugtype::AQU_UINT;
		case pSrcBox_DX10:
			return NOD_Plugtype::AQU_PNT_D3D10_BOX;
		case pSrcBox_DX11:
			return NOD_Plugtype::AQU_PNT_D3D11_BOX;
		default:
			break;
	}
#elif defined(VIREIO_D3D9)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShader_Vertex:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DVERTEXSHADER9;
		case pShader_Pixel:
			return NOD_Plugtype::AQU_PNT_IDIRECT3DPIXELSHADER9;
		case State:
			return NOD_Plugtype::AQU_D3DTRANSFORMSTATETYPE;
		case pMatrix:
			return NOD_Plugtype::AQU_PNT_D3DMATRIX;
		case State_Multiply:
			return NOD_Plugtype::AQU_D3DTRANSFORMSTATETYPE;
		case pMatrix_Multiply:
			return NOD_Plugtype::AQU_PNT_D3DMATRIX;
		case StartRegister_VertexShader:
			return NOD_Plugtype::AQU_UINT;
		case pConstantData_VertexShader:
			return NOD_Plugtype::AQU_PNT_FLOAT;
		case Vector4fCount_VertexShader:
			return NOD_Plugtype::AQU_UINT;
		case StartRegister_PixelShader:
			return NOD_Plugtype::AQU_UINT;
		case pConstantData_PixelShader:
			return NOD_Plugtype::AQU_PNT_FLOAT;
		case Vector4fCount_PixelShader:
			return NOD_Plugtype::AQU_UINT;
	}
#endif
	return 0;
}

/**
* Provides the output pointer for the requested commander.
***/
void* MatrixModifier::GetOutputPointer(DWORD dwCommanderIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Commanders)dwCommanderIndex)
	{
		case eDrawingSide:
			return (void*)&m_eCurrentRenderingSide;
			break;
		case ppActiveConstantBuffers_DX10_VertexShader:
			break;
		case ppActiveConstantBuffers_DX11_VertexShader:
			return (void*)&m_pvOutput[STS_Commanders::ppActiveConstantBuffers_DX11_VertexShader];
		case ppActiveConstantBuffers_DX10_PixelShader:
			break;
		case ppActiveConstantBuffers_DX11_PixelShader:
			break;
		default:
			break;
	}
#elif defined(VIREIO_D3D9)
#endif

	return nullptr;
}

/**
* Sets the input pointer for the requested decommander.
***/
void MatrixModifier::SetInputPointer(DWORD dwDecommanderIndex, void* pData)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShaderBytecode_VertexShader:
			m_ppvShaderBytecode_VertexShader = (void**)pData;
			break;
		case BytecodeLength_VertexShader:
			m_pnBytecodeLength_VertexShader = (SIZE_T*)pData;
			break;
		case pClassLinkage_VertexShader:
			m_ppcClassLinkage_VertexShader = (ID3D11ClassLinkage**)pData;
			break;
		case ppVertexShader_DX10:
			m_pppcVertexShader_DX10 = (ID3D10VertexShader***)pData;
			break;
		case ppVertexShader_DX11:
			m_pppcVertexShader_DX11 = (ID3D11VertexShader***)pData;
			break;
		case pShaderBytecode_PixelShader:
			m_ppvShaderBytecode_PixelShader = (void**)pData;
			break;
		case BytecodeLength_PixelShader:
			m_pnBytecodeLength_PixelShader = (SIZE_T*)pData;
			break;
		case pClassLinkage_PixelShader:
			m_ppcClassLinkage_PixelShader = (ID3D11ClassLinkage**)pData;
			break;
		case ppPixelShader_DX10:
			m_pppcPixelShader_DX10 = (ID3D10PixelShader***)pData;
			break;
		case ppPixelShader_DX11:
			m_pppcPixelShader_DX11 = (ID3D11PixelShader***)pData;
			break;
		case pVertexShader_10:
			m_ppcVertexShader_10 = (ID3D10VertexShader**)pData;
			break;
		case pVertexShader_11:
			m_ppcVertexShader_11 = (ID3D11VertexShader**)pData;
			break;
		case pPixelShader_10:
			m_ppcPixelShader_10 = (ID3D10PixelShader**)pData;
			break;
		case pPixelShader_11:
			m_ppcPixelShader_11 = (ID3D11VertexShader**)pData;
			break;
		case pDesc_DX10:
			m_ppsDesc_DX10 = (D3D10_BUFFER_DESC**)pData;
			break;
		case pInitialData_DX10:
			m_ppsInitialData_DX10 = (D3D10_SUBRESOURCE_DATA**)pData;
			break;
		case ppBuffer_DX10:
			m_pppcBuffer_DX10 = (ID3D10Buffer***)pData;
			break;
		case pDesc_DX11:
			m_ppsDesc_DX11 = (D3D11_BUFFER_DESC**)pData;
			break;
		case pInitialData_DX11:
			m_ppsInitialData_DX11 = (D3D11_SUBRESOURCE_DATA**)pData;
			break;
		case ppBuffer_DX11:
			m_pppcBuffer_DX11 = (ID3D11Buffer***)pData;
			break;
		case StartSlot_VertexShader:
			m_pdwStartSlot_VertexShader = (UINT*)pData;
			break;
		case NumBuffers_VertexShader:
			m_pdwNumBuffers_VertexShader = (UINT*)pData;
			break;
		case ppConstantBuffers_DX10_VertexShader:
			m_pppcConstantBuffers_DX10_VertexShader = (ID3D10Buffer***)pData;
			break;
		case ppConstantBuffers_DX11_VertexShader:
			m_pppcConstantBuffers_DX11_VertexShader = (ID3D11Buffer***)pData;
			break;
		case StartSlot_PixelShader:
			m_pdwStartSlot_PixelShader = (UINT*)pData;
			break;
		case NumBuffers_PixelShader:
			m_pdwNumBuffers_PixelShader = (UINT*)pData;
			break;
		case ppConstantBuffers_DX10_PixelShader:
			m_pppcConstantBuffers_DX10_PixelShader = (ID3D10Buffer***)pData;
			break;
		case ppConstantBuffers_DX11_PixelShader:
			m_pppcConstantBuffers_DX11_PixelShader = (ID3D11Buffer***)pData;
			break;
		case pDstResource_DX10:
			m_ppcDstResource_DX10 = (ID3D10Resource**)pData;
			break;
		case pDstResource_DX11:
			m_ppcDstResource_DX11 = (ID3D11Resource**)pData;
			break;
		case DstSubresource:
			m_pdwDstSubresource = (UINT*)pData;
			break;
		case pDstBox_DX10:
			m_ppsDstBox_DX10 = (D3D10_BOX**)pData;
			break;
		case pDstBox_DX11:
			m_ppsDstBox_DX11 = (D3D11_BOX**)pData;
			break;
		case pSrcData:
			m_ppvSrcData = (void**)pData;
			break;
		case SrcRowPitch:
			m_pdwSrcRowPitch = (UINT*)pData;
			break;
		case SrcDepthPitch:
			m_pdwSrcDepthPitch = (UINT*)pData;
			break;
		case pDstResource_DX10_Copy:
			m_ppcDstResource_DX10_Copy = (ID3D10Resource**)pData;
			break;
		case pSrcResource_DX10_Copy:
			m_ppcSrcResource_DX10_Copy = (ID3D10Resource**)pData;
			break;
		case pDstResource_DX11_Copy:
			m_ppcDstResource_DX11_Copy = (ID3D11Resource**)pData;
			break;
		case pSrcResource_DX11_Copy:
			m_ppcSrcResource_DX11_Copy = (ID3D11Resource**)pData;
			break;
		case pDstResource_DX10_CopySub:
			m_ppcDstResource_DX10_CopySub = (ID3D10Resource**)pData;
			break;
		case pDstResource_DX11_CopySub:
			m_ppcDstResource_DX11_CopySub = (ID3D11Resource**)pData;
			break;
		case DstSubresource_CopySub:
			m_pdwDstSubresource_CopySub = (UINT*)pData;
			break;
		case DstX:
			m_pdwDstX = (UINT*)pData;
			break;
		case DstY:
			m_pdwDstY = (UINT*)pData;
			break;
		case DstZ:
			m_pdwDstZ = (UINT*)pData;
			break;
		case pSrcResource_DX10_CopySub:
			m_ppcSrcResource_DX10_CopySub = (ID3D10Resource**)pData;
			break;
		case pSrcResource_DX11_CopySub:
			m_ppcSrcResource_DX11_CopySub = (ID3D11Resource**)pData;
			break;
		case SrcSubresource:
			m_pdwSrcSubresource = (UINT*)pData;
			break;
		case pSrcBox_DX10:
			m_ppsSrcBox_DX10 = (D3D10_BOX**)pData;
			break;
		case pSrcBox_DX11:
			m_ppsSrcBox_DX11 = (D3D11_BOX**)pData;
			break;
		default:
			break;
	}
#elif defined(VIREIO_D3D9)
	switch ((STS_Decommanders)dwDecommanderIndex)
	{
		case pShader_Vertex:
			m_ppcShader_Vertex = (IDirect3DVertexShader9**)pData;
			break;
		case pShader_Pixel:
			m_ppcShader_Pixel = (IDirect3DPixelShader9**)pData;
			break;
		case State:
			m_psState = (D3DTRANSFORMSTATETYPE*)pData;
			break;
		case pMatrix:
			m_ppsMatrix = (D3DMATRIX**)pData;
			break;
		case State_Multiply:
			m_psState_Multiply = (D3DTRANSFORMSTATETYPE*)pData;
			break;
		case pMatrix_Multiply:
			m_ppsMatrix_Multiply = (D3DMATRIX**)pData;
			break;
		case StartRegister_VertexShader:
			m_pdwStartRegister_VertexShader = (UINT*)pData;
			break;
		case pConstantData_VertexShader:
			m_ppfConstantData_VertexShader = (float**)pData;
			break;
		case Vector4fCount_VertexShader:
			m_pdwVector4fCount_VertexShader = (UINT*)pData;
			break;
		case StartRegister_PixelShader:
			m_pdwStartRegister_PixelShader = (UINT*)pData;
			break;
		case pConstantData_PixelShader:
			m_ppfConstantData_PixelShader = (float**)pData;
			break;
		case Vector4fCount_PixelShader:
			m_pdwVector4fCount_PixelShader = (UINT*)pData;
			break;
	}
#endif
}

/**
* Matrix Modifier supports various D3D10 + D3D11 calls.
***/
bool MatrixModifier::SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_10) &&
		(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_10_1))
	{
		if (nD3DInterface == INTERFACE_ID3D10DEVICE)
		{

		}
		else if (nD3DInterface == INTERFACE_IDXGISWAPCHAIN)
		{

		}
	}
	else if ((nD3DVersion >= (int)AQU_DirectXVersion::DirectX_11) &&
		(nD3DVersion <= (int)AQU_DirectXVersion::DirectX_11_2))
	{
		if (nD3DInterface == INTERFACE_ID3D11DEVICE)
		{
			if ((nD3DMethod == METHOD_ID3D11DEVICE_CREATEVERTEXSHADER) ||
				(nD3DMethod == METHOD_ID3D11DEVICE_CREATEBUFFER))
				return true;
		}
		else if (nD3DInterface == INTERFACE_ID3D11DEVICECONTEXT)
		{
			if ((nD3DMethod == METHOD_ID3D11DEVICECONTEXT_VSSETSHADER) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_VSSETCONSTANTBUFFERS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_COPYSUBRESOURCEREGION) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_COPYRESOURCE) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_UPDATESUBRESOURCE) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_VSGETCONSTANTBUFFERS) ||
				(nD3DMethod == METHOD_ID3D11DEVICECONTEXT_PSGETCONSTANTBUFFERS))
				return true;
		}
		else if (nD3DInterface == INTERFACE_IDXGISWAPCHAIN)
		{

		}
	}
#elif defined(VIREIO_D3D9)
#endif
	return false;
}

/**
* Handle Stereo Render Targets (+Depth Buffers).
* Main entry point.
***/
void* MatrixModifier::Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex)
{
#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
	switch (eD3DInterface)
	{
		case INTERFACE_ID3D11DEVICE:
			switch (eD3DMethod)
			{
#pragma region ID3D11Device::CreateVertexShader
				case METHOD_ID3D11DEVICE_CREATEVERTEXSHADER:
					// CreateVertexShader(const void *pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage *pClassLinkage, ID3D11VertexShader **ppVertexShader);

					if (!pThis) return nullptr;
					if (!m_ppvShaderBytecode_VertexShader) return nullptr;
					if (!m_pnBytecodeLength_VertexShader) return nullptr;
					if (!m_ppcClassLinkage_VertexShader) return nullptr;
					if (!m_pppcVertexShader_DX11) return nullptr;

					{
						// create the shader
						*(HRESULT*)m_pvReturn = ((ID3D11Device*)pThis)->CreateVertexShader(*m_ppvShaderBytecode_VertexShader,
							*m_pnBytecodeLength_VertexShader,
							*m_ppcClassLinkage_VertexShader,
							*m_pppcVertexShader_DX11);

						// get the shader pointer
						ID3D11VertexShader* pcShader = nullptr;
						if (*m_pppcVertexShader_DX11)
							if (**m_pppcVertexShader_DX11)
								pcShader = **m_pppcVertexShader_DX11;
						if (pcShader)
						{
							// get the hash code
							DWORD dwHashCode = GetHashCode((BYTE*)*m_ppvShaderBytecode_VertexShader, (DWORD)*m_pnBytecodeLength_VertexShader);

							// is this shader already enumerated ?
							for (size_t nShaderDescIndex = 0; nShaderDescIndex < m_asShaders.size(); nShaderDescIndex++)
							{
								if (dwHashCode == m_asShaders[nShaderDescIndex].dwHashCode)
								{
									// create and set private shader data
									Vireio_Shader_Private_Data sPrivateData;
									sPrivateData.dwHash = dwHashCode;
									sPrivateData.dwIndex = (UINT)nShaderDescIndex;

									pcShader->SetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, sizeof(sPrivateData), (void*)&sPrivateData);

									// method replaced, immediately return (= behavior -16)
									nProvokerIndex = -16;
									return m_pvReturn;
								}
							}

							// create reflection class
							ID3D11ShaderReflection* pcReflector = NULL;
							if (SUCCEEDED(D3DReflect(*m_ppvShaderBytecode_VertexShader,
								*m_pnBytecodeLength_VertexShader,
								IID_ID3D11ShaderReflection,
								(void**)&pcReflector)))
							{
								// get desc
								D3D11_SHADER_DESC sDesc;
								pcReflector->GetDesc(&sDesc);

								// fill shader data
								Vireio_D3D11_Shader sShaderData;
								sShaderData.dwConstantBuffers = sDesc.ConstantBuffers;
								sShaderData.dwVersion = sDesc.Version;
								sShaderData.dwBoundResources = sDesc.BoundResources;
								sShaderData.dwInputParameters = sDesc.InputParameters;
								sShaderData.dwOutputParameters = sDesc.OutputParameters;
								sShaderData.dwHashCode = dwHashCode;

								// get name size, max to VIREIO_MAX_VARIABLE_NAME_LENGTH
								UINT dwLen = (UINT)strnlen_s(sDesc.Creator, VIREIO_MAX_VARIABLE_NAME_LENGTH - 1);
								CopyMemory(sShaderData.szCreator, sDesc.Creator, sizeof(CHAR)*dwLen);
								sShaderData.szCreator[dwLen] = 0;

								for (UINT dwIndex = 0; dwIndex < sDesc.ConstantBuffers; dwIndex++)
								{
									// get next constant buffer
									ID3D11ShaderReflectionConstantBuffer* pcConstantBuffer = pcReflector->GetConstantBufferByIndex(dwIndex);
									if (pcConstantBuffer)
									{
										// get desc
										D3D11_SHADER_BUFFER_DESC sDescBuffer;
										pcConstantBuffer->GetDesc(&sDescBuffer);

										// fill buffer data
										Vireio_D3D11_Constant_Buffer sBufferData;
										sBufferData.eType = sDescBuffer.Type;
										sBufferData.dwVariables = sDescBuffer.Variables;
										sBufferData.dwSize = sDescBuffer.Size;
										sBufferData.dwFlags = sDescBuffer.uFlags;

										// get name size, max to VIREIO_MAX_VARIABLE_NAME_LENGTH
										dwLen = (UINT)strnlen_s(sDescBuffer.Name, VIREIO_MAX_VARIABLE_NAME_LENGTH - 1);
										CopyMemory(sBufferData.szName, sDescBuffer.Name, sizeof(CHAR)*dwLen);
										sBufferData.szName[dwLen] = 0;

										// enumerate variables
										for (UINT dwIndexVariable = 0; dwIndexVariable < sDescBuffer.Variables; dwIndexVariable++)
										{
											ID3D11ShaderReflectionVariable* pcVariable = pcConstantBuffer->GetVariableByIndex(dwIndexVariable);
											if (pcVariable)
											{
												// get desc
												D3D11_SHADER_VARIABLE_DESC sDescVariable;
												pcVariable->GetDesc(&sDescVariable);

												// fill variable data
												Vireio_D3D11_Shader_Variable sVariableData;
												sVariableData.dwSize = sDescVariable.Size;
												sVariableData.dwStartOffset = sDescVariable.StartOffset;

												// get name size, max to VIREIO_MAX_VARIABLE_NAME_LENGTH
												dwLen = (UINT)strnlen_s(sDescVariable.Name, VIREIO_MAX_VARIABLE_NAME_LENGTH - 1);
												CopyMemory(sVariableData.szName, sDescVariable.Name, sizeof(CHAR)*dwLen);
												sVariableData.szName[dwLen] = 0;

												// TODO !! FILL DEFAULT VALUE sVariableData.pcDefaultValue

												// quickly search for projection matrices here
												//if (std::strstr(sDescVariable.Name, "roj"))
												/*if (dwIndexVariable == 0)
												OutputDebugStringA(sDescVariable.Name);*/

												// and add to buffer desc
												sBufferData.asVariables.push_back(sVariableData);
											}
										}

										// and add to shader desc
										sShaderData.asBuffers.push_back(sBufferData);
									}
								}

								// and add to shader vector
								m_asShaders.push_back(sShaderData);

								// create and set private shader data
								Vireio_Shader_Private_Data sPrivateData;
								sPrivateData.dwHash = dwHashCode;
								sPrivateData.dwIndex = (UINT)m_asShaders.size() - 1;

								pcShader->SetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, sizeof(sPrivateData), (void*)&sPrivateData);

								pcReflector->Release();
							}
						}
						else OutputDebugString(L"MatrixModifier: Failed to reflect vertex shader !");

						// method replaced, immediately return (= behavior -16)
						nProvokerIndex = -16;
						return m_pvReturn;
					}
#pragma endregion
#pragma region ID3D11Device::CreateBuffer
				case METHOD_ID3D11DEVICE_CREATEBUFFER:
					if (!m_ppsDesc_DX11) return nullptr;
					if (!m_ppsInitialData_DX11) return nullptr;
					if (!m_pppcBuffer_DX11) return nullptr;

					// is this a constant buffer ?
					if (!*m_ppsDesc_DX11) return nullptr;
					if (((*m_ppsDesc_DX11)->BindFlags & D3D11_BIND_CONSTANT_BUFFER) == D3D11_BIND_CONSTANT_BUFFER)
					{
						// create the buffer
						*(HRESULT*)m_pvReturn = ((ID3D11Device*)pThis)->CreateBuffer(*m_ppsDesc_DX11,
							*m_ppsInitialData_DX11,
							*m_pppcBuffer_DX11);

						// succeeded ?
						if (SUCCEEDED(*(HRESULT*)m_pvReturn))
						{
							// create stereo buffers
							CreateStereoConstantBuffer((ID3D11Device*)pThis, nullptr, (**m_pppcBuffer_DX11), *m_ppsDesc_DX11, *m_ppsInitialData_DX11, false);
						}

						// method replaced, immediately return (= behavior -16)
						nProvokerIndex = -16;
						return m_pvReturn;
					}
					else return nullptr;
#pragma endregion		
			}
			return nullptr;
		case INTERFACE_ID3D11DEVICECONTEXT:
			switch (eD3DMethod)
			{
#pragma region ID3D11DeviceContext::UpdateSubresource
				case METHOD_ID3D11DEVICECONTEXT_UPDATESUBRESOURCE:
					/*///// TEST - TO BE DELETED
					{
					time_t t = time(0);   // get time now
					struct tm * now = localtime(&t);
					float fSeparation = (float)(now->tm_sec % 5);
					fSeparation /= 2.0f;
					ComputeViewTransforms(fSeparation);
					}
					///// TEST END */
					if (!m_ppcDstResource_DX11) return nullptr;
					if (!m_pdwDstSubresource) return nullptr;
					if (!m_ppsDstBox_DX11) return nullptr;
					if (!m_ppvSrcData) return nullptr;
					if (!m_pdwSrcRowPitch) return nullptr;
					if (!m_pdwSrcDepthPitch) return nullptr;
					if (!*m_ppcDstResource_DX11) return nullptr;
					if (!*m_ppvSrcData) return nullptr;

					// is this a buffer ?
					D3D11_RESOURCE_DIMENSION eDimension;
					(*m_ppcDstResource_DX11)->GetType(&eDimension);
					if (eDimension == D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_BUFFER)
					{
						// is this a constant buffer ?
						D3D11_BUFFER_DESC sDesc;
						((ID3D11Buffer*)*m_ppcDstResource_DX11)->GetDesc(&sDesc);
						if ((sDesc.BindFlags & D3D11_BIND_CONSTANT_BUFFER) == D3D11_BIND_CONSTANT_BUFFER)
						{
							// set constants as private data here
							((ID3D11Buffer*)*m_ppcDstResource_DX11)->SetPrivateData(PDID_ID3D11Buffer_Vireio_Data, sDesc.ByteWidth, *m_ppvSrcData);

							// loop through active buffers, if this one is active update stereo buffers
							for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
								if (m_apcActiveConstantBuffers11[dwIndex])
								{
									if (*m_ppcDstResource_DX11 == m_apcActiveConstantBuffers11[dwIndex])
									{
										//  and update the stereo buffers
										UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwIndex], 0, NULL, *m_ppvSrcData, 0, 0, dwIndex, sDesc.ByteWidth);
									}
								}
						}
					}
					else if (eDimension <= D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_TEXTURE3D)
					{
						// get the stereo twin of the resource (texture)
						ID3D11Resource* pcResourceTwin = nullptr;
						UINT dwSize = sizeof(pcResourceTwin);
						((ID3D11Resource*)*m_ppcDstResource_DX11)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwin);
						if (pcResourceTwin)
						{
							// update stereo twin
							((ID3D11DeviceContext*)pThis)->UpdateSubresource(pcResourceTwin, *m_pdwDstSubresource, *m_ppsDstBox_DX11, *m_ppvSrcData, *m_pdwSrcRowPitch, *m_pdwSrcDepthPitch);
							pcResourceTwin->Release();
						}

					}
					return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::CopySubresourceRegion
				case METHOD_ID3D11DEVICECONTEXT_COPYSUBRESOURCEREGION:
					if (!m_ppcDstResource_DX11_CopySub) return nullptr;
					if (!m_pdwDstSubresource_CopySub) return nullptr;
					if (!m_pdwDstX) return nullptr;
					if (!m_pdwDstY) return nullptr;
					if (!m_pdwDstZ) return nullptr;
					if (!m_ppcSrcResource_DX11_CopySub) return nullptr;
					if (!m_pdwSrcSubresource) return nullptr;
					if (!m_ppsSrcBox_DX11) return nullptr;
					if (!*m_ppcDstResource_DX11_CopySub) return nullptr;
					if (!*m_ppcSrcResource_DX11_CopySub) return nullptr;
					{
						// get destination resource type
						D3D11_RESOURCE_DIMENSION eDimension;
						(*m_ppcDstResource_DX11_CopySub)->GetType(&eDimension);

						// if buffer, get desc
						if (eDimension == D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_BUFFER)
						{
							if (!*m_ppsSrcBox_DX11) return nullptr;

							D3D11_BUFFER_DESC sDescDst;
							((ID3D11Buffer*)*m_ppcDstResource_DX11_CopySub)->GetDesc(&sDescDst);

							// if constant buffer, continue
							if ((sDescDst.BindFlags & D3D11_BIND_CONSTANT_BUFFER) == D3D11_BIND_CONSTANT_BUFFER)
							{
								// get description from source buffer
								D3D11_BUFFER_DESC sDescSrc;
								((ID3D11Buffer*)*m_ppcSrcResource_DX11_CopySub)->GetDesc(&sDescSrc);

								// can we map this surface ? in case update private data field
								if ((sDescSrc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) == D3D11_CPU_ACCESS_WRITE)
								{
									OutputDebugString(L"Mappable resource constant buffer !");
								}

								// get private data from source buffer
								UINT dwSize = sDescSrc.ByteWidth;
								((ID3D11Buffer*)*m_ppcSrcResource_DX11_CopySub)->GetPrivateData(PDID_ID3D11Buffer_Vireio_Data, &dwSize, m_pchBuffer11);

								if (dwSize)
								{
									// get (byte) offsets and length
									UINT dwOffsetSrc = (*m_ppsSrcBox_DX11)->left;
									UINT dwOffsetDst = (*m_pdwDstX);
									UINT dwLength = (*m_ppsSrcBox_DX11)->right;

									// return if out of bounds
									if ((dwOffsetSrc + dwLength) > sDescSrc.ByteWidth) return nullptr;
									if ((dwOffsetDst + dwLength) > sDescDst.ByteWidth) return nullptr;

									// get private data from destination buffer
									dwSize = sDescDst.ByteWidth;
									((ID3D11Buffer*)*m_ppcDstResource_DX11_CopySub)->GetPrivateData(PDID_ID3D11Buffer_Vireio_Data, &dwSize, m_pchBuffer11Temp);

									// clear buffer if no private data present
									if (!dwSize) ZeroMemory(m_pchBuffer11Temp, sDescDst.ByteWidth);

									// perform memory copy
									memcpy(&m_pchBuffer11Temp[dwOffsetDst], &m_pchBuffer11[dwOffsetSrc], dwLength);

									// set back private data for the destination buffer
									((ID3D11Buffer*)*m_ppcDstResource_DX11_CopySub)->SetPrivateData(PDID_ID3D11Buffer_Vireio_Data, sDescDst.ByteWidth, m_pchBuffer11Temp);

									// loop through active buffers, if this one is active update stereo buffers
									for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
										if (m_apcActiveConstantBuffers11[dwIndex])
										{
											if (*m_ppcDstResource_DX11_CopySub == m_apcActiveConstantBuffers11[dwIndex])
											{
												//  and update the stereo buffers
												UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwIndex], 0, NULL, m_pchBuffer11Temp, 0, 0, dwIndex, sDescDst.ByteWidth);
											}
										}
								}
							}
						}
						// is this a texture ?
						else if (eDimension <= D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_TEXTURE3D)
						{
							// get the stereo twin of the resource (texture)
							ID3D11Resource* pcResourceTwinSrc = nullptr;
							UINT dwSize = sizeof(pcResourceTwinSrc);
							((ID3D11Resource*)*m_ppcSrcResource_DX11_CopySub)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinSrc);
							if (pcResourceTwinSrc)
							{
								// get the stereo twin of the destination
								ID3D11Resource* pcResourceTwinDst = nullptr;
								dwSize = sizeof(pcResourceTwinDst);
								((ID3D11Resource*)*m_ppcDstResource_DX11_CopySub)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinDst);
								if (pcResourceTwinDst)
								{
									// do the copy call on the stereo twins of these textures
									((ID3D11DeviceContext*)pThis)->CopySubresourceRegion(pcResourceTwinDst,
										*m_pdwDstSubresource_CopySub,
										*m_pdwDstX,
										*m_pdwDstY,
										*m_pdwDstZ,
										pcResourceTwinSrc,
										*m_pdwSrcSubresource,
										*m_ppsSrcBox_DX11);

									pcResourceTwinDst->Release();
								}
								else
								{
									// set private data "create new" BOOL
									BOOL bTrue = TRUE;
									((ID3D11Resource*)*m_ppcDstResource_DX11_CopySub)->SetPrivateData(PDID_ID3D11TextureXD_ShaderResouceView_Create_New, sizeof(BOOL), &bTrue);
								}
								pcResourceTwinSrc->Release();
							}
							else
							{
								// get the stereo twin of the destination
								ID3D11Resource* pcResourceTwinDst = nullptr;
								dwSize = sizeof(pcResourceTwinDst);
								((ID3D11Resource*)*m_ppcDstResource_DX11_CopySub)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinDst);
								if (pcResourceTwinDst)
								{
									// do the copy call on the stereo twins of these textures
									((ID3D11DeviceContext*)pThis)->CopySubresourceRegion(pcResourceTwinDst,
										*m_pdwDstSubresource_CopySub,
										*m_pdwDstX,
										*m_pdwDstY,
										*m_pdwDstZ,
										*m_ppcSrcResource_DX11_CopySub,
										*m_pdwSrcSubresource,
										*m_ppsSrcBox_DX11);

									pcResourceTwinDst->Release();
								}
							}
						}
					}
					return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::CopyResource
				case METHOD_ID3D11DEVICECONTEXT_COPYRESOURCE:
					if (!m_ppcDstResource_DX11_Copy) return nullptr;
					if (!m_ppcSrcResource_DX11_Copy) return nullptr;
					if (!*m_ppcDstResource_DX11_Copy) return nullptr;
					if (!*m_ppcSrcResource_DX11_Copy) return nullptr;
					{
						// get destination resource type
						D3D11_RESOURCE_DIMENSION eDimension;
						(*m_ppcDstResource_DX11_Copy)->GetType(&eDimension);

						// if buffer, get desc
						if (eDimension == D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_BUFFER)
						{
							D3D11_BUFFER_DESC sDescDst;
							((ID3D11Buffer*)*m_ppcDstResource_DX11_Copy)->GetDesc(&sDescDst);

							// if constant buffer, continue
							if ((sDescDst.BindFlags & D3D11_BIND_CONSTANT_BUFFER) == D3D11_BIND_CONSTANT_BUFFER)
							{
								// get description from source buffer
								D3D11_BUFFER_DESC sDescSrc;
								((ID3D11Buffer*)*m_ppcSrcResource_DX11_Copy)->GetDesc(&sDescSrc);

								// if source size not equal to destination size, return
								if (sDescSrc.ByteWidth != sDescDst.ByteWidth) return nullptr;

								// can we map this surface ? in case update private data field
								if ((sDescSrc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) == D3D11_CPU_ACCESS_WRITE)
								{
									OutputDebugString(L"Mappable resource constant buffer !");
								}

								// get private data from source buffer
								UINT dwSize = sDescSrc.ByteWidth;
								((ID3D11Buffer*)*m_ppcSrcResource_DX11_Copy)->GetPrivateData(PDID_ID3D11Buffer_Vireio_Data, &dwSize, m_pchBuffer11);

								if (dwSize)
								{
									// set private data for the destination buffer
									((ID3D11Buffer*)*m_ppcDstResource_DX11_Copy)->SetPrivateData(PDID_ID3D11Buffer_Vireio_Data, sDescDst.ByteWidth, m_pchBuffer11);

									// loop through active buffers, if this one is active update stereo buffers
									for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
										if (m_apcActiveConstantBuffers11[dwIndex])
										{
											if (*m_ppcDstResource_DX11_Copy == m_apcActiveConstantBuffers11[dwIndex])
											{
												//  and update the stereo buffers
												UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwIndex], 0, NULL, m_pchBuffer11, 0, 0, dwIndex, sDescDst.ByteWidth);
											}
										}
								}
							}
						}
						// is this a texture ?
						else if (eDimension <= D3D11_RESOURCE_DIMENSION::D3D11_RESOURCE_DIMENSION_TEXTURE3D)
						{
							// get the stereo twin of the resource (texture)
							ID3D11Resource* pcResourceTwinSrc = nullptr;
							UINT dwSize = sizeof(pcResourceTwinSrc);
							((ID3D11Resource*)*m_ppcSrcResource_DX11_Copy)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinSrc);
							if (pcResourceTwinSrc)
							{
								// get the stereo twin of the destination
								ID3D11Resource* pcResourceTwinDst = nullptr;
								dwSize = sizeof(pcResourceTwinDst);
								((ID3D11Resource*)*m_ppcDstResource_DX11_Copy)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinDst);
								if (pcResourceTwinDst)
								{
									// do the copy call on the stereo twins of these textures
									((ID3D11DeviceContext*)pThis)->CopyResource(pcResourceTwinDst, pcResourceTwinSrc);
									pcResourceTwinDst->Release();
								}
								else
								{
									// set private data "create new" BOOL
									BOOL bTrue = TRUE;
									((ID3D11Resource*)*m_ppcDstResource_DX11_Copy)->SetPrivateData(PDID_ID3D11TextureXD_ShaderResouceView_Create_New, sizeof(BOOL), &bTrue);
								}
								pcResourceTwinSrc->Release();
							}
							else
							{
								// get the stereo twin of the destination
								ID3D11Resource* pcResourceTwinDst = nullptr;
								dwSize = sizeof(pcResourceTwinDst);
								((ID3D11Resource*)*m_ppcDstResource_DX11_Copy)->GetPrivateData(PDIID_ID3D11TextureXD_Stereo_Twin, &dwSize, (void*)&pcResourceTwinDst);
								if (pcResourceTwinDst)
								{
									// do the copy call on the stereo twins of these textures
									((ID3D11DeviceContext*)pThis)->CopyResource(pcResourceTwinDst, *m_ppcSrcResource_DX11_Copy);
									pcResourceTwinDst->Release();
								}								
							}
						}
					}
					return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::VSSetShader
				case METHOD_ID3D11DEVICECONTEXT_VSSETSHADER:
					if (!m_ppcVertexShader_11)
					{
						m_pcActiveVertexShader11 = nullptr;
						return nullptr;
					}
					if (!*m_ppcVertexShader_11)
					{
						m_pcActiveVertexShader11 = nullptr;
						return nullptr;
					}
					else
					{
						// set the active vertex shader
						m_pcActiveVertexShader11 = *m_ppcVertexShader_11;

						// loop through active constant buffers, get private data and update them accordingly to the new shader
						for (UINT dwIndex = 0; dwIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT; dwIndex++)
							if (m_apcActiveConstantBuffers11[dwIndex])
							{
								// get description from buffer
								D3D11_BUFFER_DESC sDesc;
								m_apcActiveConstantBuffers11[dwIndex]->GetDesc(&sDesc);

								// can we map this surface ? in case update private data field
								if ((sDesc.CPUAccessFlags & D3D11_CPU_ACCESS_WRITE) == D3D11_CPU_ACCESS_WRITE)
								{
									OutputDebugString(L"Mappable resource constant buffer !");
								}

								// get private data from buffer
								UINT dwSize = sDesc.ByteWidth;
								m_apcActiveConstantBuffers11[dwIndex]->GetPrivateData(PDID_ID3D11Buffer_Vireio_Data, &dwSize, m_pchBuffer11);

								//  and update the stereo buffers
								if (dwSize)
									UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwIndex], 0, NULL, &m_pchBuffer11[0], 0, 0, dwIndex, sDesc.ByteWidth);
							}
					}
					return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::VSSetConstantBuffers
				case METHOD_ID3D11DEVICECONTEXT_VSSETCONSTANTBUFFERS:
					if (!m_pdwStartSlot_VertexShader) return nullptr;
					if (!m_pdwNumBuffers_VertexShader) return nullptr;
					if (!m_pppcConstantBuffers_DX11_VertexShader) return nullptr;
					if (!*m_pppcConstantBuffers_DX11_VertexShader) return nullptr;
					if (!**m_pppcConstantBuffers_DX11_VertexShader) return nullptr;

					// loop through the new buffers
					for (UINT dwIndex = 0; dwIndex < *m_pdwNumBuffers_VertexShader; dwIndex++)
					{
						// get internal index
						UINT dwInternalIndex = dwIndex + *m_pdwStartSlot_VertexShader;

						// in range ? 
						if (dwInternalIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
						{
							// set buffer internally 
							m_apcActiveConstantBuffers11[dwInternalIndex] = ((*m_pppcConstantBuffers_DX11_VertexShader)[dwIndex]);

							if (m_apcActiveConstantBuffers11[dwInternalIndex])
							{
								// get private data from buffer
								D3D11_BUFFER_DESC sDesc;
								m_apcActiveConstantBuffers11[dwInternalIndex]->GetDesc(&sDesc);
								UINT dwSize = sDesc.ByteWidth;
								m_apcActiveConstantBuffers11[dwInternalIndex]->GetPrivateData(PDID_ID3D11Buffer_Vireio_Data, &dwSize, m_pchBuffer11);

								//  and update the stereo buffers
								if (dwSize)
									UpdateConstantBuffer((ID3D11DeviceContext*)pThis, (ID3D11Resource*)m_apcActiveConstantBuffers11[dwInternalIndex], 0, NULL, &m_pchBuffer11[0], 0, 0, dwInternalIndex, sDesc.ByteWidth);
							}
						}
					}

					{
						// get a copy of the active buffers
						std::vector<ID3D11Buffer*> apcActiveConstantBuffers11 = std::vector<ID3D11Buffer*>(m_apcActiveConstantBuffers11);

						// loop through the new buffers
						for (UINT dwIndex = 0; dwIndex < *m_pdwNumBuffers_VertexShader; dwIndex++)
						{
							// get internal index
							UINT dwInternalIndex = dwIndex + *m_pdwStartSlot_VertexShader;

							// in range ? 
							if (dwInternalIndex < D3D11_COMMONSHADER_CONSTANT_BUFFER_API_SLOT_COUNT)
							{
								if ((m_apcActiveConstantBuffers11)[dwInternalIndex])
								{
									// get the private data interfaces and set the current drawing side
									ID3D11Buffer* pcBuffer = nullptr;
									UINT dwSize = sizeof(pcBuffer);
									if (m_eCurrentRenderingSide == RenderPosition::Left)
										m_apcActiveConstantBuffers11[dwInternalIndex]->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Left, &dwSize, (void*)&pcBuffer);
									else
										m_apcActiveConstantBuffers11[dwInternalIndex]->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSize, (void*)&pcBuffer);

									if (pcBuffer)
									{
										apcActiveConstantBuffers11[dwInternalIndex] = pcBuffer;

										// "If the data returned is a pointer to an IUnknown, or one of its derivative classes,
										// previously set by IDXGIObject::SetPrivateDataInterface, you must call::Release() on 
										// the pointer before the pointer is freed to decrement the reference count." (Microsoft)
										pcBuffer->Release();
									}
								}
							}
						}

						// call super method
						((ID3D11DeviceContext*)pThis)->VSSetConstantBuffers(*m_pdwStartSlot_VertexShader,
							*m_pdwNumBuffers_VertexShader,
							(ID3D11Buffer**)&apcActiveConstantBuffers11[*m_pdwStartSlot_VertexShader]);

						// method replaced, immediately return (= behavior -16)
						nProvokerIndex = -16;
					}
					return nullptr;
#pragma endregion
#pragma region ID3D11DeviceContext::VSGetConstantBuffers
				case METHOD_ID3D11DEVICECONTEXT_VSGETCONSTANTBUFFERS:
					OutputDebugString(L"METHOD_ID3D11DEVICECONTEXT_VSGETCONSTANTBUFFERS");
					// ID3D11DeviceContext::VSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers);
					// return a poiner to the active constant buffers
					break;
#pragma endregion
#pragma region ID3D11DeviceContext::PSGetConstantBuffers
				case METHOD_ID3D11DEVICECONTEXT_PSGETCONSTANTBUFFERS:
					// ID3D11DeviceContext::PSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D11Buffer **ppConstantBuffers);
					OutputDebugString(L"METHOD_ID3D11DEVICECONTEXT_PSGETCONSTANTBUFFERS");
					// return a poiner to the active constant buffers
					break;
#pragma endregion
			}
			return nullptr;
#pragma region ID3D10Device::VSGetConstantBuffers
			// ID3D10Device::VSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers);
#pragma endregion
#pragma region ID3D10Device::PSGetConstantBuffers
			// ID3D10Device::PSGetConstantBuffers(UINT StartSlot, UINT NumBuffers, ID3D10Buffer **ppConstantBuffers);
#pragma endregion
	}
#elif defined(VIREIO_D3D9)
#endif

	return nullptr;
	}

#if defined(VIREIO_D3D11) || defined(VIREIO_D3D10)
/**
* Modifies all constants and updates the stereo constant buffer.
***/
void MatrixModifier::UpdateConstantBuffer(ID3D11DeviceContext* pcContext, ID3D11Resource *pcDstResource, UINT dwDstSubresource, const D3D11_BOX *psDstBox, const void *pvSrcData, UINT dwSrcRowPitch, UINT dwSrcDepthPitch, UINT dwBufferIndex, UINT dwBufferSize)
{
	// first get the current shader data
	Vireio_Shader_Private_Data sPrivateData;
	UINT dwDataSize = sizeof(sPrivateData);
	if (m_pcActiveVertexShader11)
		m_pcActiveVertexShader11->GetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, &dwDataSize, (void*)&sPrivateData);
	else return;

	// no private data ? in this case the active shader was created
	// before the Vireio profile injected... get shader data from
	// the buffer (actually set in StereoSplitterDx10->SetDrawingSide() )
	if (!dwDataSize)
	{
		dwDataSize = sizeof(sPrivateData);
		pcDstResource->GetPrivateData(PDID_ID3D11VertexShader_Vireio_Data, &dwDataSize, (void*)&sPrivateData);
	}

	// get private data interfaces (stereo constant buffers)
	ID3D11Buffer* pcBufferLeft = nullptr;
	ID3D11Buffer* pcBufferRight = nullptr;

	// get the stereo private data interfaces
	UINT dwSizeLeft = sizeof(pcBufferLeft);
	UINT dwSizeRight = sizeof(pcBufferLeft);
	pcDstResource->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Left, &dwSizeLeft, &pcBufferLeft);
	pcDstResource->GetPrivateData(PDIID_ID3D11Buffer_Constant_Buffer_Right, &dwSizeRight, &pcBufferRight);

	// copy the data to left and right constant buffer data buffers
	memcpy(m_pchBuffer11Left, pvSrcData, dwBufferSize);
	memcpy(m_pchBuffer11Right, pvSrcData, dwBufferSize);

	// private data present ?
	if (dwDataSize)
	{
		// shader has this buffer index ?
		if (dwBufferIndex < m_asShaders[sPrivateData.dwIndex].asBuffers.size())
		{
			// loop through the shader constants
			for (size_t nConstant = 0; nConstant < m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables.size(); nConstant++)
			{
				// test for projection matrix
				if ((std::strstr(m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].szName, "ProjectionMatrix")) &&
					(!std::strstr(m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].szName, "Inv")))
				{
					D3DXMATRIX sMatrix;
					if (m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwSize == sizeof(D3DMATRIX))
					{
						// is this modification in range ?
						if ((dwSizeLeft) && (dwSizeRight) && (pcBufferLeft) && (pcBufferRight) && (dwBufferSize >= (m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset + sizeof(D3DMATRIX))))
						{
							bool bTranspose = false;

							// get pointers to the matrix (left+right)
							UINT_PTR pv = (UINT_PTR)pvSrcData + m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset;
							UINT_PTR pvLeft = (UINT_PTR)m_pchBuffer11Left + m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset;
							UINT_PTR pvRight = (UINT_PTR)m_pchBuffer11Right + m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwStartOffset;

							// get the matrix
							D3DXMATRIX sMatrix = D3DXMATRIX((CONST FLOAT*)pv);
							D3DXMATRIX sMatrixModified;

							if (bTranspose) D3DXMatrixTranspose(&sMatrix, &sMatrix);

							// apply left 
							sMatrixModified = sMatrix * matViewProjTransformLeft * matProjectionInv * matProjection;
							if (bTranspose) D3DXMatrixTranspose(&sMatrixModified, &sMatrixModified);
							memcpy((void*)pvLeft, &sMatrixModified, sizeof(D3DXMATRIX));

							// apply right
							sMatrixModified = sMatrix * matViewProjTransformRight *	matProjectionInv * matProjection;
							if (bTranspose) D3DXMatrixTranspose(&sMatrixModified, &sMatrixModified);
							memcpy((void*)pvRight, &sMatrixModified, sizeof(D3DXMATRIX));
						}
					}
					else
					{
						OutputDebugString(L"MatrixModifier: Wrong variable size.... NOT the size of a matrix !");
						wchar_t buf[128];
						wsprintf(buf, L"This size %u ; Matrix size %u", m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].dwSize, sizeof(D3DMATRIX));
						OutputDebugString(buf);
						OutputDebugStringA(m_asShaders[sPrivateData.dwIndex].asBuffers[dwBufferIndex].asVariables[nConstant].szName);
					}
				}
			}
		}
	}
	else
	{
		// Both Vertex Shader and Constant buffer have no private shader data !
		OutputDebugString(L"MatrixModifier: Both Vertex Shader and Constant buffer have no private shader data ! ");
	}

	if ((!dwSizeLeft) || (!dwSizeRight))
	{
		// create stereo buffer, first get description
		D3D11_BUFFER_DESC sDesc;
		((ID3D11Buffer*)pcDstResource)->GetDesc(&sDesc);

		// is constant buffer ? actually must be
		if ((sDesc.BindFlags & D3D11_BIND_CONSTANT_BUFFER) == D3D11_BIND_CONSTANT_BUFFER)
		{
			// get device
			ID3D11Device* pcDevice = nullptr;
			pcDstResource->GetDevice(&pcDevice);
			if (pcDevice)
			{
				CreateStereoConstantBuffer(pcDevice, pcContext, (ID3D11Buffer*)pcDstResource, &sDesc, NULL, true);
				pcDevice->Release();
			}
		}
		else
			OutputDebugString(L"MatrixModifier: Code error, not a constant buffer !");
	}

	// finally, update the stereo constant buffers
	if (pcBufferLeft)
		pcContext->UpdateSubresource((ID3D11Resource*)pcBufferLeft, dwDstSubresource, psDstBox, m_pchBuffer11Left, dwSrcRowPitch, dwSrcDepthPitch);
	if (pcBufferRight)
		pcContext->UpdateSubresource((ID3D11Resource*)pcBufferRight, dwDstSubresource, psDstBox, m_pchBuffer11Right, dwSrcRowPitch, dwSrcDepthPitch);

	// and release them
	SAFE_RELEASE(pcBufferLeft);
	SAFE_RELEASE(pcBufferRight);
}

/**
* Creates a stereo buffer out of a buffer.
* Assigns both left and right buffer to the main buffer
* as private data.
* @param pcDevice The d3d11 device.
* @param pcContext The d3d11 device context.
* @param pcBuffer The mono constant buffer to assign stereo constant buffers.
* @param pDesc Pointer to the buffer description.
* @param pInitialData Pointer to the initial data, NULL if bCopyData is true.
* @param bCopyData True if data from main buffer is to be copied to stereo buffers.
***/
void MatrixModifier::CreateStereoConstantBuffer(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, ID3D11Buffer* pcBuffer, D3D11_BUFFER_DESC *pDesc, D3D11_SUBRESOURCE_DATA *pInitialData, bool bCopyData)
{
	// create left buffer
	ID3D11Buffer* pcBufferLeft = nullptr;
	if (FAILED(pcDevice->CreateBuffer(pDesc,
		pInitialData,
		&pcBufferLeft)))
		OutputDebugString(L"MatrixModifier: Failed to create left buffer!");

	// create right buffer
	ID3D11Buffer* pcBufferRight = nullptr;
	if (FAILED(pcDevice->CreateBuffer(pDesc,
		pInitialData,
		&pcBufferRight)))
		OutputDebugString(L"MatrixModifier: Failed to create right buffer!");

	// copy resource ?
	if (bCopyData)
	{
		pcContext->CopyResource(pcBufferLeft, pcBuffer);
		pcContext->CopyResource(pcBufferRight, pcBuffer);
	}

	// set as private data interface to the main buffer
	pcBuffer->SetPrivateDataInterface(PDIID_ID3D11Buffer_Constant_Buffer_Left, pcBufferLeft);
	pcBuffer->SetPrivateDataInterface(PDIID_ID3D11Buffer_Constant_Buffer_Right, pcBufferRight);

	// reference counter must be 1 now (reference held by the main buffer)
	ULONG nRef = pcBufferLeft->Release();
	if (nRef != 1) OutputDebugString(L"MatrixModifier: Reference counter invalid !");
	nRef = pcBufferRight->Release();
	if (nRef != 1) OutputDebugString(L"MatrixModifier: Reference counter invalid !");
}

#elif defined(VIREIO_D3D9)
#endif

/**
* Computes the view transform.
* To be deleted after including "ViewAdjustment" class from base driver.
***/
void MatrixModifier::ComputeViewTransforms(float fSeparation)
{
	// separation settings are overall (HMD and desktop), since they are based on physical IPD
	D3DXMatrixTranslation(&transformLeft, fSeparation * -1.0f, 0, 0);
	D3DXMatrixTranslation(&transformRight, fSeparation * 1.0f, 0, 0);

	// projection transform
	matViewProjTransformLeft = matProjectionInv * transformLeft * projectLeft;
	matViewProjTransformRight = matProjectionInv * transformRight * projectRight;
}