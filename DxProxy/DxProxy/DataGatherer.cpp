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

#include "DataGatherer.h"
#include "Vireio.h"

#include "Version.h"

#define MATRIX_NAMES 17
#define AVOID_SUBSTRINGS 2
#define ANALYZE_FRAMES 500

using namespace vireio;

/**
* Simple helper to get the hash of a shader.
* @param pShader The input vertex shader.
* @return The hash code of the shader.
***/
uint32_t ShaderHash(LPDIRECT3DVERTEXSHADER9 pShader)
{
	if (!pShader) return 0;

	BYTE* pData = NULL;
	UINT pSizeOfData;
	pShader->GetFunction(NULL, &pSizeOfData);

	pData = new BYTE[pSizeOfData];
	pShader->GetFunction(pData, &pSizeOfData);

	uint32_t hash = 0;
	MurmurHash3_x86_32(pData, pSizeOfData, VIREIO_SEED, &hash);
	delete[] pData;
	return hash;
}

/**
* Simple helper to get the hash of a shader.
* @param pShader The input vertex shader.
* @return The hash code of the shader.
***/
uint32_t ShaderHash(LPDIRECT3DPIXELSHADER9 pShader)
{
	if (!pShader) return 0;

	BYTE* pData = NULL;
	UINT pSizeOfData;
	pShader->GetFunction(NULL, &pSizeOfData);

	pData = new BYTE[pSizeOfData];
	pShader->GetFunction(pData, &pSizeOfData);

	uint32_t hash = 0;
	MurmurHash3_x86_32(pData, pSizeOfData, VIREIO_SEED, &hash);
	delete[] pData;
	return hash;
}

/**
* Constructor, opens the dump file.
* @param pDevice Imbed actual device.
* @param pCreatedBy Pointer to the object that created the device.
***/
DataGatherer::DataGatherer(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy):D3DProxyDevice(pDevice, pCreatedBy),
	m_recordedVShaders(),
	m_recordedPShaders(),
	m_recordedSetVShaders(),
	m_activeVShaders(),
	m_activePShaders(),
	m_activeVShadersLastFrame(),
	m_activePShadersLastFrame(),
	m_excludedVShaders(),
	m_excludedPShaders(),
	m_bAvoidDraw(false),
	m_bAvoidDrawPS(false),
	m_startAnalyzingTool(false),
	m_analyzingFrameCounter(0)
{
	// create matrix name array 
	static std::string names[] = { "ViewProj", "viewproj", "viewProj", "wvp", "mvp", "WVP", "MVP", "wvP", "mvP", "matFinal", "matrixFinal", "MatrixFinal", "FinalMatrix", "finalMatrix", "m_VP", "m_P", "m_screen" };
	m_wvpMatrixConstantNames = names;
	static std::string avoid[] = { "Inv", "inv" };
	m_wvpMatrixAvoidedSubstrings = avoid;

	// set commands predefined before read from cfg file
	m_bOutputShaderCode = false;
	m_bTransposedRules = false;
	m_bTestForTransposed = true;
	m_bIgnoreCompare = false;

	// get potential matrix names
	ProxyHelper* helper = new ProxyHelper();
	std::stringstream sstm;
	sstm << helper->GetBaseDir() << "cfg\\shader_rules\\brassa.cfg";
	std::ifstream cfgFile;
	cfgFile.open(sstm.str(), std::ios::in);
	if (cfgFile.is_open())
	{
		enum CFG_FILEMODE
		{
			POTENTIAL_MATRIX_NAMES = 1,
			BRASSA_COMMANDS
		} cfgFileMode;

		// get names
		std::vector<std::string> vNames;
		UINT numLines = 0;
		std::string unused;
		while ( cfgFile.good() )
		{
			static std::string s;

			// read whole line
			s.clear();
			char ch;
			while (cfgFile.get(ch) && ch != '\n' && ch != '\r')
				s += ch;

			if (s.find('#')!=std::string::npos)
			{
				// comments
			}
			else if (s.compare("<Potential_Matrix_Names>")==0)
			{
				cfgFileMode = POTENTIAL_MATRIX_NAMES;
			} else if (s.compare("<BRASSA_Commands>")==0)
			{
				cfgFileMode = BRASSA_COMMANDS;
			} else
			{
				switch(cfgFileMode)
				{
				case POTENTIAL_MATRIX_NAMES:
					vNames.push_back(s);
					numLines++;
					break;
				case BRASSA_COMMANDS:
					if (s.compare("Output_Shader_Code")==0)
					{
						OutputDebugString("Output_Shader_Code");
						m_bOutputShaderCode = true;
					}
					if (s.compare("Do_Transpose_Matrices")==0)
					{
						OutputDebugString("Do_Transpose_Matrices");
						m_bTransposedRules = true;
						m_bTestForTransposed = false;
					}
					if (s.compare("Do_Not_Transpose_Matrices")==0)
					{
						OutputDebugString("Do_Not_Transpose_Matrices");
						m_bTransposedRules = false;
						m_bTestForTransposed = false;
					}
					if (s.compare("Show_All_Matrices")==0)
					{
						OutputDebugString("Show_All_Matrices");
						m_bIgnoreCompare = true;
					}
					break;
				}
			}
		}

		// create array out of vector
		m_wvpMatrixConstantNames = new std::string[numLines];
		for (UINT i = 0; i < numLines; i++)
			m_wvpMatrixConstantNames[i] = vNames[i];

		cfgFile.close();
		vNames.clear();
	}
	delete helper;

	// create shader dump file
	m_shaderDumpFile.open("shaderDump.csv", std::ios::out);
	m_shaderDumpFile << "Shader Hash,Constant Name,ConstantType,Start Register,Register Count,Vertex/Pixel Shader" << std::endl;
}

/**
* Destructor, closes the dump file.
***/
DataGatherer::~DataGatherer()
{
	m_shaderDumpFile.close();
	delete [] m_wvpMatrixConstantNames;
}

/**
* If F7 pressed, starts to analyze.
***/
HRESULT WINAPI DataGatherer::Present(CONST RECT* pSourceRect,CONST RECT* pDestRect,HWND hDestWindowOverride,CONST RGNDATA* pDirtyRegion)
{
	// analyze ?
	if (m_startAnalyzingTool)
	{
		// analyze the first time
		Analyze();
		m_startAnalyzingTool = false;

		// draw a rectangle to show beeing in analyze mode
		D3DRECT rec = {320, 320, 384, 384};
		ClearRect(vireio::RenderPosition::Left, rec, COLOR_RED);
	}

	// draw an indicator (colored rectangle) for each found rule
	UINT xPos = 320;
	for(auto itAddedConstants = m_addedVSConstants.begin();
	    itAddedConstants != m_addedVSConstants.end();
	    ++itAddedConstants)
	{
		// draw a rectangle to show beeing in analyze mode
		D3DRECT rec = {xPos, 288, xPos+16, 304};
		ClearRect(vireio::RenderPosition::Left, rec, COLOR_GREEN);

		xPos+=20;
	}

	return D3DProxyDevice::Present(pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

/**
* Sets last frame updates, calls super funtion.
***/
HRESULT WINAPI DataGatherer::BeginScene()
{
	if (m_isFirstBeginSceneOfFrame) {
		// create last frame active shader vectors, delete current frame ones
		m_activeVShadersLastFrame.clear();
		m_activePShadersLastFrame.clear();
		m_activeVShadersLastFrame = m_activeVShaders;
		m_activePShadersLastFrame = m_activePShaders;
		m_activeVShaders.clear();
		m_activePShaders.clear();
	}

	return D3DProxyDevice::BeginScene();
}

/**
* Skips draw call if specified bool is set.
***/
HRESULT WINAPI DataGatherer::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
	if ((m_bAvoidDraw) || (m_bAvoidDrawPS))
		return S_OK;
	else 
		return D3DProxyDevice::DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

/**
* Skips draw call if specified bool is set.
***/
HRESULT WINAPI DataGatherer::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	if ((m_bAvoidDraw) || (m_bAvoidDrawPS))
		return S_OK;
	else 
		return D3DProxyDevice::DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

/**
* Skips draw call if specified bool is set.
***/
HRESULT WINAPI DataGatherer::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	if ((m_bAvoidDraw) || (m_bAvoidDrawPS))
		return S_OK;
	else 
		return D3DProxyDevice::DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

/**
* Skips draw call if specified bool is set.
***/
HRESULT WINAPI DataGatherer::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	if ((m_bAvoidDraw) || (m_bAvoidDrawPS))
		return S_OK;
	else 
		return D3DProxyDevice::DrawIndexedPrimitiveUP(PrimitiveType, MinVertexIndex, NumVertices, PrimitiveCount, pIndexData, IndexDataFormat, pVertexStreamZeroData, VertexStreamZeroStride);
}

/**
* Creates proxy vertex shader and outputs relevant constant data to dump file.
* (if compiled for debug, output shader code to "VS(hash).txt")
***/
HRESULT WINAPI DataGatherer::CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader)
{
	// create proxy vertex shader
	HRESULT creationResult = D3DProxyDevice::CreateVertexShader(pFunction, ppShader);

	if (SUCCEEDED(creationResult)) {
		BaseDirect3DVertexShader9* pWrappedShader = static_cast<BaseDirect3DVertexShader9*>(*ppShader);
		IDirect3DVertexShader9* pActualShader = pWrappedShader->getActual();

		// insertion succeeded - record shader details.
		LPD3DXCONSTANTTABLE pConstantTable = NULL;

		BYTE* pData = NULL;
		UINT pSizeOfData;
		pActualShader->GetFunction(NULL, &pSizeOfData);

		pData = new BYTE[pSizeOfData];					
		HRESULT _hr = pActualShader->GetFunction(pData, &pSizeOfData);
		if(_hr == D3DERR_INVALIDCALL)
		{
			OutputDebugString("DATAGATHERER :: Invalid Call to IDirect3DVertexShader9->getFunction()");	
		}
			
		uint32_t hash = 0;
		MurmurHash3_x86_32(pData, pSizeOfData, VIREIO_SEED, &hash); 
		if (m_recordedVShaders.find(hash) == m_recordedVShaders.end() && 
			m_shaderDumpFile.is_open())
		{
			//This doesn't keep a ref count, doesn't need to, it isn't used
			m_recordedVShaders[hash] = *ppShader;

			_hr = D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(pData), &pConstantTable);
			if(_hr == D3DERR_INVALIDCALL)
				OutputDebugString("DATAGATHERER :: Invalid Call to D3DXGetShaderConstantTable");	
			else if(_hr == D3DXERR_INVALIDDATA)
				OutputDebugString("DATAGATHERER :: D3DXERR_INVALIDDATA to D3DXGetShaderConstantTable");	
			else if(_hr == E_OUTOFMEMORY)
				OutputDebugString("DATAGATHERER :: Out of Memory to D3DXGetShaderConstantTable");	
			
			D3DXCONSTANTTABLE_DESC pDesc;

			if(pConstantTable == NULL)
			{
				OutputDebugString("DATAGATHERER :: Vertex Shader Constant Table is Null");	
				debugf("Size of Data: %d Data Contents: %s\n", pSizeOfData, pData);
			}
			else
			{
				pConstantTable->GetDesc(&pDesc);
			
				D3DXCONSTANT_DESC pConstantDesc[512];
				UINT pConstantNum = 512;

				for(UINT i = 0; i < pDesc.Constants; i++)
				{
					D3DXHANDLE handle = pConstantTable->GetConstant(NULL,i);
					if(handle == NULL) continue;

					pConstantTable->GetConstantDesc(handle, pConstantDesc, &pConstantNum);
					if (pConstantNum >= 512) {
						OutputDebugString("Need larger constant description buffer");
					}
				
					// loop through constants, output relevant data
					for(UINT j = 0; j < pConstantNum; j++)
					{
						if ((pConstantDesc[j].RegisterSet == D3DXRS_FLOAT4) &&
							((pConstantDesc[j].Class == D3DXPC_VECTOR) || (pConstantDesc[j].Class == D3DXPC_MATRIX_ROWS) || (pConstantDesc[j].Class == D3DXPC_MATRIX_COLUMNS))  ) {

								m_shaderDumpFile << hash;
								m_shaderDumpFile << "," << pConstantDesc[j].Name;

								if (pConstantDesc[j].Class == D3DXPC_VECTOR) {
									m_shaderDumpFile << ",Vector";
								}
								else if (pConstantDesc[j].Class == D3DXPC_MATRIX_ROWS) {
									m_shaderDumpFile << ",MatrixR";
								}
								else if (pConstantDesc[j].Class == D3DXPC_MATRIX_COLUMNS) {
									m_shaderDumpFile << ",MatrixC";
								}

								m_shaderDumpFile << "," << pConstantDesc[j].RegisterIndex;
								m_shaderDumpFile << "," << pConstantDesc[j].RegisterCount << ",VS" << std::endl;

								// add constant to relevant constant vector
								ShaderConstant sc;
								sc.hash = hash;
								sc.desc = D3DXCONSTANT_DESC(pConstantDesc[j]);
								sc.name = std::string(pConstantDesc[j].Name);
								m_relevantVSConstants[hash].push_back(sc);
						}
					}
				}
			}

			// output shader code ?
			if (m_bOutputShaderCode)
			{
				// optionally, output shader code to "VS(hash).txt"
				char buf[32]; ZeroMemory(&buf[0],32);
				sprintf_s(buf, "VS%u.txt", hash);
				std::ofstream oLogFile(buf,std::ios::ate);

				if (oLogFile.is_open())
				{
					LPD3DXBUFFER bOut; 
					D3DXDisassembleShader(reinterpret_cast<DWORD*>(pData),NULL,NULL,&bOut); 
					oLogFile << static_cast<char*>(bOut->GetBufferPointer()) << std::endl;
					oLogFile << std::endl << std::endl;
					if(pConstantTable != NULL)
					{
						oLogFile << "// Shader Creator: " << pDesc.Creator << std::endl;
						oLogFile << "// Shader Version: " << pDesc.Version << std::endl;
					}
					oLogFile << "// Shader Hash   : " << hash << std::endl;
				}
			}

			_SAFE_RELEASE(pConstantTable);
		}
		// else shader already recorded

		if (pData) delete[] pData;
	}

	return creationResult;
}

/**
* Sets the shader and the current shader hash.
***/
HRESULT WINAPI DataGatherer::SetVertexShader(IDirect3DVertexShader9* pShader)
{
	// set the current vertex shader hash code for the call counter
	if (pShader)
	{
		m_currentVertexShaderHash = ShaderHash(pShader);

		// add to vector of active shaders if not present
		if (m_activeVShaders.find(m_currentVertexShaderHash) == m_activeVShaders.end()) {
			m_activeVShaders[m_currentVertexShaderHash] = pShader;
		}

		// avoid draw if shader present in excluded vector
		if (std::find(m_excludedVShaders.begin(), m_excludedVShaders.end(), m_currentVertexShaderHash) == m_excludedVShaders.end()) {
			m_bAvoidDraw = false;
		}
		else
		{
			/*if ((GetTickCount()%300)>150)
				m_bAvoidDraw = true;
			else m_bAvoidDraw = false;*/
			m_bAvoidDraw = true;
		}
	}
	else m_currentVertexShaderHash = 0;

#ifdef _DEBUG
	D3D9ProxyVertexShader* pWrappedVShaderData = static_cast<D3D9ProxyVertexShader*>(pShader);
	if (pWrappedVShaderData)
	{
		if (!pWrappedVShaderData->SquishViewport())
		{
			if (m_recordedSetVShaders.insert(pShader).second)
			{
				debugf("Set Vertex Shader: %u", m_currentVertexShaderHash);
			}

		}
	}
#endif

	return D3DProxyDevice::SetVertexShader(pShader);
}

/**
* Tests if the set constant is a transposed matrix and sets the relevant bool.
* Is Matrix transposed ?
* Affine transformation matrices have in the last row (0,0,0,1). World and view matrices are 
* usually affine, since they are a combination of affine transformations (rotation, scale, 
* translation ...).
* Perspective projection matrices have in the last column (0,0,1,0) if left-handed and 
* (0,0,-1,0) if right-handed.
* Orthographic projection matrices have in the last column (0,0,0,1).
* If those are transposed you find the entries in the last column/row.
**/
HRESULT WINAPI DataGatherer::SetVertexShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	// test for transposed rules
	if (m_bTestForTransposed)
	{
		if (m_relevantVSConstants.find(m_currentVertexShaderHash) != m_relevantVSConstants.end())
		{
			// loop through relevant vertex shader constants
			for(auto itShaderConstants = m_relevantVSConstants[m_currentVertexShaderHash].begin();
			    itShaderConstants != m_relevantVSConstants[m_currentVertexShaderHash].end();
			    ++itShaderConstants)
			{
				// is a constant of current shader ?
				// start register ? 
				if ((itShaderConstants->desc.RegisterIndex < (StartRegister+Vector4fCount)) &&
					(itShaderConstants->desc.RegisterIndex >= StartRegister))
				{	
					// is a matrix ?
					if (itShaderConstants->desc.Class == D3DXPARAMETER_CLASS::D3DXPC_MATRIX_ROWS)
					{
						// Perspective projection matrices have in the last column (0,0,1,0) if left-handed and 
						// * (0,0,-1,0) if right-handed.
						// Note that we DO NOT TEST here wether this is actually a projection matrix
						// (we do that in the analyze() method)
						D3DXMATRIX matrix = D3DXMATRIX(pConstantData+((itShaderConstants->desc.RegisterIndex-StartRegister)*4*sizeof(float)));

						// [14] for row matrix ??
						if ((!vireio::AlmostSame(matrix[14], 1.0f, 0.00001f)) && (!vireio::AlmostSame(matrix[14], -1.0f, 0.00001f)))
							m_bTransposedRules = true;

					}
					else if (itShaderConstants->desc.Class == D3DXPARAMETER_CLASS::D3DXPC_MATRIX_COLUMNS)
					{
						// Perspective projection matrices have in the last column (0,0,1,0) if left-handed and 
						// * (0,0,-1,0) if right-handed.
						// Note that we DO NOT TEST here wether this is actually a projection matrix
						// (we do that in the analyze() method)
						D3DXMATRIX matrix = D3DXMATRIX(pConstantData+((itShaderConstants->desc.RegisterIndex-StartRegister)*4*sizeof(float)));

						// [12] for column matrix ??
						if ((!vireio::AlmostSame(matrix[12], 1.0f, 0.00001f)) && (!vireio::AlmostSame(matrix[12], -1.0f, 0.00001f)))
							m_bTransposedRules = true;
					}
				}
			}
		}
	}

	return D3DProxyDevice::SetVertexShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

/**
*
***/
HRESULT WINAPI DataGatherer::CreatePixelShader(CONST DWORD* pFunction,IDirect3DPixelShader9** ppShader)
{
	// create proxy vertex shader
	HRESULT creationResult = D3DProxyDevice::CreatePixelShader(pFunction, ppShader);

	if (SUCCEEDED(creationResult)) {
		BaseDirect3DPixelShader9* pWrappedShader = static_cast<BaseDirect3DPixelShader9*>(*ppShader);
		IDirect3DPixelShader9* pActualShader = pWrappedShader->getActual();

		// insertion succeeded - record shader details.
		LPD3DXCONSTANTTABLE pConstantTable = NULL;

		BYTE* pData = NULL;
		UINT pSizeOfData;
		pActualShader->GetFunction(NULL, &pSizeOfData);

		pData = new BYTE[pSizeOfData];
		pActualShader->GetFunction(pData, &pSizeOfData);

		uint32_t hash = 0;
		MurmurHash3_x86_32(pData, pSizeOfData, VIREIO_SEED, &hash); 
		if (m_recordedPShaders.find(hash) == m_recordedPShaders.end() && 
			m_shaderDumpFile.is_open())
		{
			//This doesn't keep a ref count (no need, it basically isn't used)
			m_recordedPShaders[hash] = *ppShader;

			D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(pData), &pConstantTable);

			D3DXCONSTANTTABLE_DESC pDesc;
			if(pConstantTable == NULL)
			{
				OutputDebugString("DATAGATHERER :: Pixel Shader Constant Table is Null");	
				debugf("Size of Data: %d Data Contents: %s\n", pSizeOfData, pData);
			}
			else
			{
				pConstantTable->GetDesc(&pDesc);

				D3DXCONSTANT_DESC pConstantDesc[512];
				UINT pConstantNum = 512;

				for(UINT i = 0; i < pDesc.Constants; i++)
				{
					D3DXHANDLE handle = pConstantTable->GetConstant(NULL,i);
					if(handle == NULL) continue;

					pConstantTable->GetConstantDesc(handle, pConstantDesc, &pConstantNum);
					if (pConstantNum >= 512) {
						OutputDebugString("Need larger constant description buffer");
					}

					// loop through constants, output relevant data
					for(UINT j = 0; j < pConstantNum; j++)
					{
						if ((pConstantDesc[j].RegisterSet == D3DXRS_FLOAT4) &&
							((pConstantDesc[j].Class == D3DXPC_VECTOR) || (pConstantDesc[j].Class == D3DXPC_MATRIX_ROWS) || (pConstantDesc[j].Class == D3DXPC_MATRIX_COLUMNS))  ) {

								m_shaderDumpFile << hash;
								m_shaderDumpFile << "," << pConstantDesc[j].Name;

								if (pConstantDesc[j].Class == D3DXPC_VECTOR) {
									m_shaderDumpFile << ",Vector";
								}
								else if (pConstantDesc[j].Class == D3DXPC_MATRIX_ROWS) {
									m_shaderDumpFile << ",MatrixR";
								}
								else if (pConstantDesc[j].Class == D3DXPC_MATRIX_COLUMNS) {
									m_shaderDumpFile << ",MatrixC";
								}

								m_shaderDumpFile << "," << pConstantDesc[j].RegisterIndex;
								m_shaderDumpFile << "," << pConstantDesc[j].RegisterCount << ",PS" << std::endl;

								//// add constant to relevant constant vector - TODO !! pixel shader constants
								//ShaderConstant sc;
								//sc.hash = hash;
								//sc.desc = D3DXCONSTANT_DESC(pConstantDesc[j]);
								//sc.name = std::string(pConstantDesc[j].Name);
								//sc.hasRule = false;
								//m_relevantVSConstants[hash].push_back(sc);
						}
					}
				}
			}

			// output shader code ?
			if (m_bOutputShaderCode)
			{
				// optionally, output shader code to "PS(hash).txt"
				char buf[32]; ZeroMemory(&buf[0],32);
				sprintf_s(buf, "PS%u.txt", hash);
				std::ofstream oLogFile(buf,std::ios::ate);

				if (oLogFile.is_open())
				{
					LPD3DXBUFFER bOut; 
					D3DXDisassembleShader(reinterpret_cast<DWORD*>(pData),NULL,NULL,&bOut); 
					oLogFile << static_cast<char*>(bOut->GetBufferPointer()) << std::endl;
					oLogFile << std::endl << std::endl;
					if (pConstantTable != NULL)
					{
						oLogFile << "// Shader Creator: " << pDesc.Creator << std::endl;
						oLogFile << "// Shader Version: " << pDesc.Version << std::endl;
					}
					oLogFile << "// Shader Hash   : " << hash << std::endl;
				}
			}

			_SAFE_RELEASE(pConstantTable);
		}
		// else shader already recorded

		if (pData) delete[] pData;
	}

	return creationResult;
}

/**
* Sets the shader and the outputs current shader hash for debug reasons.
***/
HRESULT WINAPI DataGatherer::SetPixelShader(IDirect3DPixelShader9* pShader)
{
	uint32_t currentPixelShaderHash = 0;
	if (pShader)
	{
		currentPixelShaderHash = ShaderHash(pShader);

		// add to vector of active shaders if not present
		if (m_activePShaders.find(currentPixelShaderHash) == m_activePShaders.end()) {
			m_activePShaders[currentPixelShaderHash] = pShader;
		}

		// avoid draw if shader present in excluded vector
		if (std::find(m_excludedPShaders.begin(), m_excludedPShaders.end(), currentPixelShaderHash) == m_excludedPShaders.end()) {
			m_bAvoidDrawPS = false;
		}
		else
		{
			if ((GetTickCount()%300)>150)
				m_bAvoidDrawPS = true;
			else m_bAvoidDrawPS = false;
		}
	}

#ifdef _DEBUG
	debugf("Cur Vertex Shader: %u", m_currentVertexShaderHash);
	debugf("Set Pixel Shader: %u", currentPixelShaderHash);
#endif

	return D3DProxyDevice::SetPixelShader(pShader);
}

/**
* Calls the super init-method.
* Implemented here for possible future use.
* @param cfg The game configuration to init this class.
***/
void DataGatherer::Init(ProxyHelper::ProxyConfig& cfg)
{
	OutputDebugString("Special Proxy: Shader data gatherer created.\n");

	D3DProxyDevice::Init(cfg);
}


/**
* Shader Analyzer sub menu.
***/
void DataGatherer::VPMENU_ShaderSubMenu()
{
	UINT menuEntryCount = 6;
	
	enum
	{
		CREATE_SHADER_RULES = 0,
		CHANGE_SHADER_RULES,
		// PICK_RULES,
		SHOW_SHADERS,
		SAVE_RULES,
		BACK_VPMENU,
		BACK_GAME
	};

	VPMENU_NewFrame(menuEntryCount);

	// output menu
	VPMENU_StartDrawing("Shader Analyser");

	AddButtonMenuItem("Create new Shader Rules", [=]() {
		// create relevant shader constant table
		GetCurrentShaderRules(true);
		VPMENU_CloseWithoutSaving();
		Analyze();
	});
	AddButtonMenuItem("Change current Shader Rules", [=]() {
		// create menu names new
		GetCurrentShaderRules(false);
		VPMENU_NavigateTo([=]() {
			VPMENU_ChangeRules();
		});
	});
	//AddButtonMenuItem("Pick Rules by active Shaders", [=]() {
	//	VPMENU_NavigateTo([=]() {
	//		VPMENU_PickRules();
	//	});
	//});
	AddButtonMenuItem("Show and exclude active Shaders", [=]() {
		VPMENU_NavigateTo([=]() {
			VPMENU_ShowActiveShaders();
		});
		//Clear collections
		m_knownVShaders.clear();
		m_knownPShaders.clear();
	});
	AddButtonMenuItem("Save Shader Rules", [=]() {
		VPMENU_CloseWithoutSaving();
		// save data
		ProxyHelper* helper = new ProxyHelper();

		// get filename by target exe name
		std::string shaderRulesFileName = helper->GetTargetExe();
		auto ext = shaderRulesFileName.find("exe");
		if (ext!=std::string::npos)
			shaderRulesFileName.replace(ext,3,"xml");
		else
			shaderRulesFileName = "default.xml";

		// ... and add path, delete proxy helper
		std::stringstream sstm;
		sstm << helper->GetBaseDir() << "cfg\\shader_rules\\" << shaderRulesFileName;
		config.shaderRulePath = sstm.str();
		delete helper;

		// ... finally, save
		saveShaderRules();
	});
	
	AddButtonMenuItem("Back to Main Menu", [=]() { VPMENU_Back(); });
	AddButtonMenuItem("Back to Game", [=]() { VPMENU_CloseWithoutSaving(); });

	VPMENU_FinishDrawing();
}

/**
* Change current shader rules.
***/
void DataGatherer::VPMENU_ChangeRules()
{
	enum
	{
		FLAG_TRANSPOSITION = (1<<29),
		FLAG_RULENAME = (1<<30),
		FLAG_RULECLASS = (1<<31)
	};
	
	UINT menuEntryCount = 2;
	UINT constantIndex = 0;
	std::vector<std::string> menuEntries;
	std::vector<D3DCOLOR> menuColor;
	std::vector<DWORD> menuID;
	
	// loop through relevant vertex shader constants
	for(auto itShaderConstants = m_relevantVSConstantNames.begin();
	    itShaderConstants != m_relevantVSConstantNames.end();
	    itShaderConstants++)
	{
		D3DCOLOR entryColor = itShaderConstants->hasRule ? COLOR_MENU_ENABLED : COLOR_MENU_TEXT;
		menuColor.push_back(entryColor);
		menuID.push_back(constantIndex);
		menuEntries.push_back(itShaderConstants->name);
		if (itShaderConstants->nodeOpen)
		{
			// output the class
			menuColor.push_back(entryColor);
			menuID.push_back(constantIndex+FLAG_RULECLASS);
			// output shader constant + index 
			switch(itShaderConstants->desc.Class)
			{
			case D3DXPC_VECTOR:
				menuEntries.push_back("  D3DXPC_VECTOR");
				break;
			case D3DXPC_MATRIX_ROWS:
				menuEntries.push_back("  D3DXPC_MATRIX_ROWS");
				break;
			case D3DXPC_MATRIX_COLUMNS:
				menuEntries.push_back("  D3DXPC_MATRIX_COLUMNS");
				break;
			}
			menuEntryCount++;

			// output the class
			menuColor.push_back(entryColor);
			menuID.push_back(constantIndex+FLAG_RULENAME);
			if (itShaderConstants->hasRule)
				menuEntries.push_back("  "+itShaderConstants->ruleName);
			else
				menuEntries.push_back("  No Rule assigned");
			menuEntryCount++;

			// output wether transposed or not
			if ((itShaderConstants->hasRule) && (itShaderConstants->desc.Class != D3DXPC_VECTOR))
			{
				menuColor.push_back(entryColor);
				menuID.push_back(constantIndex+FLAG_TRANSPOSITION);
				if (itShaderConstants->isTransposed)
					menuEntries.push_back("  Transposed");
				else
					menuEntries.push_back("  Non-Transposed");
				menuEntryCount++;
			}
		}

		constantIndex++;
		menuEntryCount++;
	}

	VPMENU_NewFrame(menuEntryCount);
	UINT entryID = VPMENU_GetCurrentSelection();;
	
	// adjust border & menu due to menu scroll
	float borderDrawingHeight = borderTopHeight;
	if (menuVelocity == 0.0f)
		borderTopHeight = menuTop+menuEntryHeight*(float)entryID;
	if (borderTopHeight>(menuTop+(menuEntryHeight*12.0f)))
		borderDrawingHeight = menuTop+menuEntryHeight*12.0f;

	// down scroll border/menu adjustment
	if (menuTopHeight>=(borderDrawingHeight-borderTopHeight))
		menuTopHeight = (borderDrawingHeight-borderTopHeight);
	else
		borderDrawingHeight=borderTopHeight+menuTopHeight;

	// up scroll border/menu adjustment
	if (borderDrawingHeight<menuTop)
	{
		menuTopHeight+=menuTop-borderDrawingHeight;
		borderDrawingHeight = menuTop;
	}


	// draw border - total width due to shift correction
	D3DRECT rect;
	rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderDrawingHeight; rect.y2 = (int)(borderDrawingHeight+viewportHeight*0.04f);
	ClearEmptyRect(vireio::RenderPosition::Left, rect, COLOR_MENU_BORDER, 2);
	ClearEmptyRect(vireio::RenderPosition::Right, rect, COLOR_MENU_BORDER, 2);

	VPMENU_StartDrawing_NonMenu();

	menuHelperRect.left = 800; menuHelperRect.top = 350;
	menuHelperRect.top += (int)(menuTopHeight / fScaleY);
	
	for (UINT i=0; i<menuEntryCount-2; i++)
	AddMenuItem(menuEntries[i], menuColor[i], [&]()
	{
		// switch shader rule node
		if (VPMENU_Input_Selected() && HotkeysActive())
		{
			// constant node entry ?
			if ((menuID[entryID] & FLAG_RULECLASS) == FLAG_RULECLASS)
			{
				// no influence on class node entry
			}
			else if ((menuID[entryID] & FLAG_RULENAME) == FLAG_RULENAME) // add/delete rule
			{
				// no rule present, so add
				if (!m_relevantVSConstantNames[menuID[entryID]].hasRule)
				{
					for(auto itShaderConstantMap = m_relevantVSConstants.begin();
					    itShaderConstantMap != m_relevantVSConstants.end();
					    itShaderConstantMap++)
					{
						for(auto itShaderConstants = itShaderConstantMap->second.begin();
						    itShaderConstants != itShaderConstantMap->second.end();
						    itShaderConstants++)
						{
							// constant name in menu entries already present
							if (itShaderConstants->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) != 0)
								continue;
							
							// never assign "transposed" to vector
							if (itShaderConstants->desc.Class == D3DXPARAMETER_CLASS::D3DXPC_VECTOR)
								addRule(itShaderConstants->name, true, itShaderConstants->desc.RegisterIndex, itShaderConstants->desc.Class, 1, false);
							else
								addRule(itShaderConstants->name, true, itShaderConstants->desc.RegisterIndex, itShaderConstants->desc.Class, 1, m_bTransposedRules);
							itShaderConstants->hasRule = true;

							// set the menu output accordingly
							for(auto itShaderConstants1 = m_relevantVSConstantNames.begin();
							    itShaderConstants1 !=m_relevantVSConstantNames.end();
							    itShaderConstants1++)
							{
								// set rule bool for all relevant constant names
								if (itShaderConstants1->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) != 0)
									continue;
								
								UINT operation;
								itShaderConstants1->hasRule = m_pGameHandler->GetShaderModificationRepository()->ConstantHasRule(itShaderConstants1->name, itShaderConstants1->ruleName, operation, itShaderConstants1->isTransposed);
							}
						}
					}
				}
				else // rule present, so delete
				{
					deleteRule(m_relevantVSConstantNames[menuID[entryID]].name);

					// set the menu output accordingly
					for(auto itShaderConstants1 = m_relevantVSConstantNames.begin();
					    itShaderConstants1 != m_relevantVSConstantNames.end();
					    itShaderConstants1++)
					{
						// set rule bool for all relevant constant names
						if (itShaderConstants1->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) != 0)
							continue;
						
						UINT operation;
						itShaderConstants1->hasRule = m_pGameHandler->GetShaderModificationRepository()->ConstantHasRule(itShaderConstants1->name, itShaderConstants1->ruleName, operation, itShaderConstants1->isTransposed);
					}
				}
			}
			else if ((menuID[entryID] & FLAG_TRANSPOSITION) == FLAG_TRANSPOSITION)
			{
				bool newTrans = !m_relevantVSConstantNames[menuID[entryID]].isTransposed;
				// transposed or non-transposed
				for(auto itShaderConstantMap = m_relevantVSConstants.begin();
				    itShaderConstantMap != m_relevantVSConstants.end();
				    itShaderConstantMap++)
				{
					for(auto itShaderConstants = itShaderConstantMap->second.begin();
					    itShaderConstants != itShaderConstantMap->second.end();
					    itShaderConstants++)
					{
						// constant name in menu entries already present
						if (itShaderConstants->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) == 0)
						{
							// get the operation id
							UINT operation;
							m_pGameHandler->GetShaderModificationRepository()->ConstantHasRule(itShaderConstants->name, itShaderConstants->ruleName, operation, itShaderConstants->isTransposed);
							modifyRule(itShaderConstants->name, operation, newTrans);

							// set the menu output accordingly
							for(auto itShaderConstants1 = m_relevantVSConstantNames.begin();
							    itShaderConstants1 != m_relevantVSConstantNames.end();
							    itShaderConstants1++)
							{
								// set rule bool for all relevant constant names
								if (itShaderConstants1->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) != 0)
									continue;
								
								itShaderConstants1->hasRule = m_pGameHandler->GetShaderModificationRepository()->ConstantHasRule(itShaderConstants1->name, itShaderConstants1->ruleName, operation, itShaderConstants1->isTransposed);
							}
						}
					}
				}
			}
			else
			{
				// open or close node
				m_relevantVSConstantNames[menuID[entryID]].nodeOpen = !m_relevantVSConstantNames[menuID[entryID]].nodeOpen;

				for(auto itShaderConstantMap = m_relevantVSConstants.begin();
				    itShaderConstantMap != m_relevantVSConstants.end();
				    itShaderConstantMap++)
				{
					for(auto itShaderConstants = itShaderConstantMap->second.begin();
					    itShaderConstants != itShaderConstantMap->second.end();
					    itShaderConstants++)
					{
						// constant name in menu entries already present?
						if (itShaderConstants->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) != 0)
							continue;
						
						// show blinking if shader is drawn
						if (m_relevantVSConstantNames[menuID[entryID]].nodeOpen)
						{
							if (std::find(m_excludedVShaders.begin(), m_excludedVShaders.end(), itShaderConstants->hash) == m_excludedVShaders.end()) {
								m_excludedVShaders.push_back(itShaderConstants->hash);
							}
						}
						else
						{
							// erase all entries for that hash
							m_excludedVShaders.erase(std::remove(m_excludedVShaders.begin(), m_excludedVShaders.end(), itShaderConstants->hash), m_excludedVShaders.end()); 
						}
					}
				}
			}

			HotkeyCooldown(2.0f);
		}
		
		// switch shader rule node
		if (VPMENU_Input_IsAdjustment() && HotkeysActive())
		{
			if ((menuID[entryID] & FLAG_RULENAME) == FLAG_RULENAME) // rule node entry
			{
				// rule present, so modify
				if (m_relevantVSConstantNames[menuID[entryID]].hasRule)
				{
					// get the operation id
					UINT operation;
					m_pGameHandler->GetShaderModificationRepository()
						->ConstantHasRule(m_relevantVSConstantNames[menuID[entryID]].name,
							m_relevantVSConstantNames[menuID[entryID]].ruleName,
							operation, m_relevantVSConstantNames[menuID[entryID]].isTransposed);
					
					if (VPMENU_Input_Left())
					{
						if (operation > 0)
							operation--;
					}
					else if (VPMENU_Input_Right())
					{
						UINT maxValue;
						if (m_relevantVSConstantNames[menuID[entryID]].desc.Class == D3DXPARAMETER_CLASS::D3DXPC_VECTOR) {
							maxValue = (UINT)ShaderConstantModificationFactory::Vec4EyeShiftUnity;
						} else {
							maxValue = (UINT)ShaderConstantModificationFactory::MatConvergenceOffset;
						}
						
						if (operation < maxValue)
							operation++;
					}

					for(auto itShaderConstantMap = m_relevantVSConstants.begin();
					    itShaderConstantMap != m_relevantVSConstants.end();
					    ++itShaderConstantMap)
					{
						for(auto itShaderConstants = itShaderConstantMap->second.begin();
						    itShaderConstants != itShaderConstantMap->second.end();
						    itShaderConstants++)
						{
							// constant name in menu entries already present?
							if (itShaderConstants->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) != 0)
								continue;
							
							modifyRule(itShaderConstants->name, operation, itShaderConstants->isTransposed);

							// set the menu output accordingly
							for(auto itShaderConstants1 = m_relevantVSConstantNames.begin();
							    itShaderConstants1 != m_relevantVSConstantNames.end();
							    ++itShaderConstants1)
							{
								// set rule bool for all relevant constant names
								if (itShaderConstants1->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) != 0)
									continue;
								
								itShaderConstants1->hasRule = m_pGameHandler->GetShaderModificationRepository()->ConstantHasRule(itShaderConstants1->name, itShaderConstants1->ruleName, operation, itShaderConstants1->isTransposed);
							}
						}
					}
				}
			}
			HotkeyCooldown(2.0f);
		}
	});

	AddButtonMenuItem("Back to Main Menu", [=]() { VPMENU_Back(); });
	AddButtonMenuItem("Back to Game", [=]() { VPMENU_CloseWithoutSaving(); });

	VPMENU_FinishDrawing();
}

/**
* Pick shader rules out of active shaders.
***/
void DataGatherer::VPMENU_PickRules()
{
	// ERROR_CALL_NOT_IMPLEMENTED
}

/**
* Show currently used Shaders (Hash code) and exclude from rendering, if chosen.
***/
void DataGatherer::VPMENU_ShowActiveShaders()
{
	//Don't do anything if it is an empty collection
	if (m_activePShaders.size() == 0)
		return;

	UINT menuEntryCount = 2;
	std::vector<std::string> menuEntries;
	std::vector<D3DCOLOR> menuColor;
	std::vector<uint32_t> menuID;
	std::string menuEntry;

	//Local copy of known shaders, the main collection is too big
	if (m_knownVShaders.size() == 0)
		m_knownVShaders = m_activeVShaders;

	// loop through relevant vertex shaders
	for(auto itVShaderHash = m_knownVShaders.begin();
	    itVShaderHash != m_knownVShaders.end();
	    itVShaderHash++)
	{
		bool excluded = false;
		bool visible = true;
		// show colored if shader is drawn
		if (std::find(m_excludedVShaders.begin(), m_excludedVShaders.end(), itVShaderHash->first) != m_excludedVShaders.end()) {
			excluded = true;
		}

		if (m_activeVShaders.find(itVShaderHash->first) == m_activeVShaders.end())
			visible = false;

		menuID.push_back(itVShaderHash->first);

		if (!visible)
		{
			menuColor.push_back(COLOR_MENU_DISABLED);
			menuEntries.push_back(retprintf("VS : (%u)", itVShaderHash->first));
		}
		else
		{
			menuColor.push_back(excluded ? COLOR_MENU_TEXT : COLOR_MENU_ENABLED);
			menuEntries.push_back(retprintf("VS : %u", itVShaderHash->first));
		}

		menuEntryCount++;
	}

	// for next time, add in any we don;t know yet
	for(auto itVShaderCurrentHash = m_activeVShaders.begin();
	    itVShaderCurrentHash != m_activeVShaders.end();
	    itVShaderCurrentHash++)
	{
		if (m_knownVShaders.find(itVShaderCurrentHash->first) == m_knownVShaders.end())
			m_knownVShaders[itVShaderCurrentHash->first] = itVShaderCurrentHash->second;
	}

	UINT endOfVertexShaderEntries = menuEntryCount-2;
	
	//Local copy of known shaders, the main collection is too big
	if (m_knownPShaders.size() == 0)
		m_knownPShaders = m_activePShaders;

	// loop through relevant pixel shaders
	for(auto itPShaderHash = m_knownPShaders.begin();
	    itPShaderHash != m_knownPShaders.end();
	    ++itPShaderHash)
	{
		bool excluded = false;
		bool visible = true;
		// show colored if shader is drawn
		if (std::find(m_excludedPShaders.begin(), m_excludedPShaders.end(), itPShaderHash->first) != m_excludedPShaders.end()) {
			excluded = true;
		}
		if (m_activePShaders.find(itPShaderHash->first) == m_activePShaders.end())
			visible = false;

		menuID.push_back(itPShaderHash->first);

		if (!visible)
		{
			menuColor.push_back(COLOR_MENU_DISABLED);
			menuEntries.push_back(retprintf("PS : (%u)", itPShaderHash->first));
		}
		else
		{
			menuColor.push_back(excluded ? COLOR_MENU_TEXT : COLOR_MENU_ENABLED);
			menuEntries.push_back(retprintf("PS : %u", itPShaderHash->first));
		}

		menuEntryCount++;
	}
	
	// for next time, add in any we don;t know yet
	for(auto itPShaderCurrentHash = m_activePShaders.begin();
	    itPShaderCurrentHash != m_activePShaders.end();
	    itPShaderCurrentHash++)
	{
		if (m_knownPShaders.find(itPShaderCurrentHash->first) == m_knownPShaders.end())
			m_knownPShaders[itPShaderCurrentHash->first] = itPShaderCurrentHash->second;
	}

	VPMENU_NewFrame(menuEntryCount);
	UINT entryID = VPMENU_GetCurrentSelection();

	if ((entryID >= 0) && (entryID < menuEntryCount-2) && (menuEntryCount>2))
	{
		// switch shader node (drawn/not-drawn)
		if (VPMENU_Input_Selected())
		{
			if (entryID < endOfVertexShaderEntries)
			{
				// show colored if shader is drawn
				if (std::find(m_excludedVShaders.begin(), m_excludedVShaders.end(), menuID[entryID]) == m_excludedVShaders.end()) {
					m_excludedVShaders.push_back(menuID[entryID]);
				}
				else
				{
					// erase all entries for that hash
					m_excludedVShaders.erase(std::remove(m_excludedVShaders.begin(), m_excludedVShaders.end(), menuID[entryID]), m_excludedVShaders.end()); 
				}
			}
			else
			{
				// show colored if shader is drawn
				if (std::find(m_excludedPShaders.begin(), m_excludedPShaders.end(), menuID[entryID]) == m_excludedPShaders.end()) {
					m_excludedPShaders.push_back(menuID[entryID]);
				}
				else
				{
					// erase all entries for that hash
					m_excludedPShaders.erase(std::remove(m_excludedPShaders.begin(), m_excludedPShaders.end(), menuID[entryID]), m_excludedPShaders.end()); 
				}
			}

			HotkeyCooldown(2.0f);
		}
	}



	// output menu
	// adjust border & menu due to menu scroll
	float borderDrawingHeight = borderTopHeight;
	if (menuVelocity == 0.0f)
		borderTopHeight = menuTop+menuEntryHeight*(float)entryID;
	if (borderTopHeight>(menuTop+(menuEntryHeight*12.0f)))
		borderDrawingHeight = menuTop+menuEntryHeight*12.0f;

	// down scroll border/menu adjustment
	if (menuTopHeight>=(borderDrawingHeight-borderTopHeight))
		menuTopHeight = (borderDrawingHeight-borderTopHeight);
	else
		borderDrawingHeight=borderTopHeight+menuTopHeight;

	// up scroll border/menu adjustment
	if (borderDrawingHeight<menuTop)
	{
		menuTopHeight+=menuTop-borderDrawingHeight;
		borderDrawingHeight = menuTop;
	}


	// draw border - total width due to shift correction
	D3DRECT rect;
	rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderDrawingHeight; rect.y2 = (int)(borderDrawingHeight+viewportHeight*0.04f);
	ClearEmptyRect(vireio::RenderPosition::Left, rect, COLOR_MENU_BORDER, 2);
	ClearEmptyRect(vireio::RenderPosition::Right, rect, COLOR_MENU_BORDER, 2);

	VPMENU_StartDrawing_NonMenu();

	menuHelperRect.left = 800; menuHelperRect.top = 350;
	menuHelperRect.top += (int)(menuTopHeight / fScaleY);
	for (UINT i = 0; i < menuEntryCount-2; i++)
	{
		if ((menuHelperRect.top + 40) >= 0)
		{
			DrawMenuItem(menuEntries[i].c_str(), menuColor[i]);
		}

		//No point drawing anything off the bottom of the viewport!
		if (menuHelperRect.top > viewportHeight)
			break;
	}

	AddButtonMenuItem("Back to Main Menu", [=]() { VPMENU_Back(); });
	AddButtonMenuItem("Back to Game", [=]() { VPMENU_CloseWithoutSaving(); });

	VPMENU_FinishDrawing();
}


/**
* Analyzes the game and outputs a shader rule xml file.
***/
void DataGatherer::Analyze()
{
	OutputDebugString("DATA GATHERER: Analyzing");
	// loop through relevant vertex shader constants
	for(auto itShaderConstants = m_relevantVSConstantNames.begin();
	    itShaderConstants != m_relevantVSConstantNames.end();
	    itShaderConstants++)
	{
		OutputDebugString("DATA GATHERER: While Not at End");
		// loop through matrix constant name assumptions
		for (int i = 0; i < MATRIX_NAMES; i++)
		{
			OutputDebugString("DATA GATHERER: Matrix Name:");
			OutputDebugString(itShaderConstants->name.c_str());
			// test if assumption is found in constant name
			if (strstr(itShaderConstants->name.c_str(), m_wvpMatrixConstantNames[i].c_str()) != 0 || m_bIgnoreCompare == true)
			{
				// test for "to-be-avoided" assumptions
				for (int j = 0; j < AVOID_SUBSTRINGS; j++)
				{
					if (strstr(itShaderConstants->name.c_str(), m_wvpMatrixAvoidedSubstrings[j].c_str()) != 0)
					{
						// break loop
						i = MATRIX_NAMES;
						break;
					}
				}

				// still in loop ?
				if (i < MATRIX_NAMES)
				{
					// add this rule !!!!
					//Only Add the rule if it is a vector or has a register count of 1
					if((itShaderConstants->desc.Class != D3DXPC_VECTOR && itShaderConstants->desc.RegisterCount == 4) || itShaderConstants->desc.RegisterCount == 1)
					{
						if (addRule(itShaderConstants->name, true, itShaderConstants->desc.RegisterIndex, itShaderConstants->desc.Class, 2, m_bTransposedRules))
							m_addedVSConstants.push_back(*itShaderConstants);
					}
					
					// output debug data
					OutputDebugString("---Shader Rule");
					// output constant name
					OutputDebugString(itShaderConstants->desc.Name);
					// output shader constant + index 
					switch(itShaderConstants->desc.Class)
					{
					case D3DXPC_VECTOR:
						OutputDebugString("D3DXPC_VECTOR");
						break;
					case D3DXPC_MATRIX_ROWS:
						OutputDebugString("D3DXPC_MATRIX_ROWS");
						break;
					case D3DXPC_MATRIX_COLUMNS:
						OutputDebugString("D3DXPC_MATRIX_COLUMNS");
						break;
					}
					debugf("Register Index: %d", itShaderConstants->desc.RegisterIndex);
					debugf("Shader Hash: %u", itShaderConstants->hash);
					debugf("Transposed: %d", m_bTransposedRules);

					// end loop
					i = MATRIX_NAMES;
				}
			}
			else
				if (itShaderConstants->desc.RegisterIndex == 128)
				{
					OutputDebugString(itShaderConstants->name.c_str());
					OutputDebugString(m_wvpMatrixConstantNames[i].c_str());
				}
		}
	}

	// save data
	ProxyHelper* helper = new ProxyHelper();

	// get filename by target exe name
	std::string shaderRulesFileName = helper->GetTargetExe();
	auto ext = shaderRulesFileName.find("exe");
	if (ext!=std::string::npos)
		shaderRulesFileName.replace(ext,3,"xml");
	else
		shaderRulesFileName = "default.xml";

	// ... and add path, delete proxy helper
	std::stringstream sstm;
	sstm << helper->GetBaseDir() << "cfg\\shader_rules\\" << shaderRulesFileName;
	config.shaderRulePath = sstm.str();
	delete helper;

	// ... finally, save
	saveShaderRules();
}

/**
* Fills the data structure for the shader rule menu nodes.
* @param allStartRegisters True if an existing constant name is added with all possible start registers.
***/
void DataGatherer::GetCurrentShaderRules(bool allStartRegisters)
{
	ShaderModificationRepository* pModRep = m_pGameHandler->GetShaderModificationRepository();

	// clear name vector, loop through constants
	m_relevantVSConstantNames.clear();
	for(auto itShaderConstantMap = m_relevantVSConstants.begin();
	    itShaderConstantMap != m_relevantVSConstants.end();
	    ++itShaderConstantMap)
	{
		for(auto itShaderConstants = itShaderConstantMap->second.begin();
		    itShaderConstants != itShaderConstantMap->second.end();
		    ++itShaderConstants)
		{
			bool namePresent = false;
			bool registerPresent = !allStartRegisters;
			for(auto itShaderConstants1 = m_relevantVSConstantNames.begin();
			    itShaderConstants1 != m_relevantVSConstantNames.end();
			    itShaderConstants1++)
			{
				// constant name in menu entries already present
				if (itShaderConstants->name.compare(itShaderConstants1->name) == 0)
				{
					namePresent = true;
					if (itShaderConstants->desc.RegisterIndex==itShaderConstants1->desc.RegisterIndex)
						registerPresent = true;
				}
			}

			if ((!namePresent) || (!registerPresent))
			{
				// is a rule already applied to that constant name ?
				itShaderConstants->nodeOpen = false;
				UINT operation = 0;
				if (pModRep)
					itShaderConstants->hasRule = pModRep->ConstantHasRule(itShaderConstants->name, itShaderConstants->ruleName, operation, itShaderConstants->isTransposed);
				else
					itShaderConstants->hasRule = false;
				m_relevantVSConstantNames.push_back(*itShaderConstants);
			}
		}
	}
}


/**
* Adds a default shader rule to the game configuration.
* @return True if rule was added, false if rule already present.
***/
bool DataGatherer::addRule(std::string constantName, bool allowPartialNameMatch, UINT startRegIndex, D3DXPARAMETER_CLASS constantType, UINT operationToApply, bool transpose)
{
	SHOW_CALL("AddRule");
	
	return m_pGameHandler->AddRule(m_spShaderViewAdjustment, constantName, allowPartialNameMatch, startRegIndex, constantType, operationToApply, transpose);
}

/**
* Adds a default shader rule to the game configuration.
* @return True if rule was added, false if rule already present.
***/
bool DataGatherer::modifyRule(std::string constantName, UINT operationToApply, bool transpose)
{
	SHOW_CALL("ModifyRule");
	
	return m_pGameHandler->ModifyRule(m_spShaderViewAdjustment, constantName, operationToApply, transpose);
}

/**
* Delete rule.
* @return True if rule was deleted, false if rule not present.
***/
bool DataGatherer::deleteRule(std::string constantName)
{
	SHOW_CALL("DeleteRule");
	
	return m_pGameHandler->DeleteRule(m_spShaderViewAdjustment, constantName);
}

/*
* Saves current game shader rules (and game configuration).
***/
void DataGatherer::saveShaderRules()
{
	SHOW_CALL("SaveShaderRules");
	
	m_pGameHandler->Save(config, m_spShaderViewAdjustment);

	ProxyHelper* helper = new ProxyHelper();
	helper->SaveConfig(config);
	delete helper;
}
