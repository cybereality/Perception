/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <DataGatherer.cpp> and
Class <DataGatherer> :
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

#ifndef DATAGATHERER_H_INCLUDED
#define DATAGATHERER_H_INCLUDED

#include "Direct3DDevice9.h"
#include "D3DProxyDevice.h"
#include "ProxyHelper.h"
#include "MurmurHash3.h"
#include "Direct3DVertexShader9.h"

/**
* Data gatherer class, outputs relevant shader data to dump file (.csv format) .
* Outputs Shader Hash,Constant Name,ConstantType,Start Register,Register Count to "shaderDump.csv".
* Used ".csv" file format to easily open and sort using OpenOffice (for example). These informations let 
* you create new shader rules.
* (if compiled to debug, it outputs shader code to "VS(hash).txt" or "PS(hash).txt")
*/
class DataGatherer : public D3DProxyDevice
{
public:
	DataGatherer(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy);
	virtual ~DataGatherer();

	/*** IDirect3DDevice9 methods ***/
	virtual HRESULT WINAPI Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion);
	virtual HRESULT WINAPI BeginScene();
	virtual HRESULT WINAPI DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount);
	virtual HRESULT WINAPI DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount);
	virtual HRESULT WINAPI DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	virtual HRESULT WINAPI DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride);
	virtual HRESULT WINAPI CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader);
	virtual HRESULT WINAPI SetVertexShader(IDirect3DVertexShader9* pShader);
	virtual HRESULT WINAPI SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	virtual HRESULT WINAPI CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader);
	virtual HRESULT WINAPI SetPixelShader(IDirect3DPixelShader9* pShader);

	/*** DataGatherer public methods ***/
	virtual void Init(ProxyHelper::ProxyConfig& cfg);

protected:
	/*** DataGatherer protected methods ***/
	virtual void BRASSA_ShaderSubMenu();
	virtual void BRASSA_ChangeRules();
	virtual void BRASSA_PickRules();
	virtual void BRASSA_ShowActiveShaders();

private:
	/*** DataGatherer private methods ***/
	void Analyze();
	void GetCurrentShaderRules(bool allStartRegisters);

	/**
	* Describes a shader constant.
	***/
	struct ShaderConstant
	{
		std::string name;
		UINT hash;              /**< The shader hash. */
		D3DXCONSTANT_DESC desc; /**< The constant description. */
		bool nodeOpen;          /**< True if menu node open for that constant. */
		bool hasRule;           /**< True if shader rule present for that constant. */
		bool isTransposed;      /**< True if shader rule present for that constant. */
		std::string ruleName;   /**< The name of the associated rule. */
	};
	/**
	* True if analyzing will output transposed shader rules.
	***/
	bool m_bTransposedRules;
	/**
	* True if analyzer tests for transposed matrices.
	***/
	bool m_bTestForTransposed;
	/**
	* Vector of all relevant vertex shader constants.
	***/
	std::vector<ShaderConstant> m_relevantVSConstants;
	/**
	* Vector of all added vertex shader constants (rules).
	***/
	std::vector<ShaderConstant> m_addedVSConstants;
	/**
	* Vector of all relevant vertex shader constants, each name only once.
	***/
	std::vector<ShaderConstant> m_relevantVSConstantNames;
	/**
	* Vector of all active vertex shader hash codes.
	***/
	std::vector<uint32_t> m_activeVShaders;
	/**
	* Vector of all active pixel shader hash codes.
	***/
	std::vector<uint32_t> m_activePShaders;
	/**
	* Vector of all active vertex shader hash codes (last frame).
	***/
	std::vector<uint32_t> m_activeVShadersLastFrame;
	/**
	* Vector of all active pixel shader hash codes (last frame).
	***/
	std::vector<uint32_t> m_activePShadersLastFrame;
	/**
	* Vector of all excluded vertex shader hash codes.
	* Vertex shaders are excluded from being drawn.
	***/
	std::vector<uint32_t> m_excludedVShaders;
	/**
	* Vector of all excluded pixel shader hash codes.
	* Pixel shaders are excluded from being drawn.
	***/
	std::vector<uint32_t> m_excludedPShaders;
	/**
	* True if Draw() calls should be skipped currently.
	***/
	bool m_bAvoidDraw;
	/**
	* Pixel shader helper for m_bAvoidDraw.
	***/
	bool m_bAvoidDrawPS;
	/**
	* Array of possible world-view-projection matrix shader constant names.
	***/
	std::string* m_wvpMatrixConstantNames;
	/**
	* Array of matrix substring names to be avoided.
	***/
	std::string* m_wvpMatrixAvoidedSubstrings;
	/**
	* True if analyzing tool is activated.
	***/
	bool m_startAnalyzingTool;
	/**
	* Frame counter for analyzing.
	***/
	UINT m_analyzingFrameCounter;
	/**
	* Set of recorded vertex shaders, to avoid double output.
	***/
	std::unordered_set<IDirect3DVertexShader9*> m_recordedVShaders;
	/**
	* Set of recorded shaders, to avoid double output.
	***/
	std::unordered_set<IDirect3DPixelShader9*> m_recordedPShaders;
	/**
	* Set of recorded vertex shaders, to avoid double debug log output.
	***/
	std::unordered_set<IDirect3DVertexShader9*> m_recordedSetVShaders;
	/**
	* The shader dump file (.csv format).
	***/
	std::ofstream m_shaderDumpFile;
	/**
	* The hash code of the vertex shader currently set.
	***/
	uint32_t m_currentVertexShaderHash;
	/**
	* True if data gatherer should output (ALL!) shader code.
	***/
	bool m_bOutputShaderCode;
};

#endif