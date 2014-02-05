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

#define MATRIX_NAMES 17
#define AVOID_SUBSTRINGS 2
#define ANALYZE_FRAMES 500

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
		ClearRect(vireio::RenderPosition::Left, rec, D3DCOLOR_ARGB(255,255,0,0));
	}

	// draw an indicator (colored rectangle) for each found rule
	UINT xPos = 320;
	auto itAddedConstants = m_addedVSConstants.begin();
	while (itAddedConstants != m_addedVSConstants.end())
	{
		// draw a rectangle to show beeing in analyze mode
		D3DRECT rec = {xPos, 288, xPos+16, 304};
		ClearRect(vireio::RenderPosition::Left, rec, D3DCOLOR_ARGB(255,0,255,0));

		xPos+=20;
		++itAddedConstants;
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
		m_activeVShadersLastFrame = std::vector<uint32_t>(m_activeVShaders);
		m_activePShadersLastFrame = std::vector<uint32_t>(m_activePShaders);
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

		// No idea what happens if the same vertex shader is created twice. Pointer to the same shader or two
		// separate instances? guessing separate in which case m_recordedShader as is is pointless. 
		// TODO Replace pointer check with check on hash of shader data. (and print hash with data)
		if (m_recordedVShaders.insert(pActualShader).second && m_shaderDumpFile.is_open()) {

			// insertion succeeded - record shader details.
			LPD3DXCONSTANTTABLE pConstantTable = NULL;

			BYTE* pData = NULL;
			UINT pSizeOfData;
			pActualShader->GetFunction(NULL, &pSizeOfData);

			pData = new BYTE[pSizeOfData];
			pActualShader->GetFunction(pData, &pSizeOfData);

			uint32_t hash = 0;
			MurmurHash3_x86_32(pData, pSizeOfData, VIREIO_SEED, &hash); 

			D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(pData), &pConstantTable);

			if(pConstantTable == NULL) 
				return creationResult;

			D3DXCONSTANTTABLE_DESC pDesc;
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
							m_relevantVSConstants.push_back(sc);
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
					oLogFile << "// Shader Creator: " << pDesc.Creator << std::endl;
					oLogFile << "// Shader Version: " << pDesc.Version << std::endl;
					oLogFile << "// Shader Hash   : " << hash << std::endl;
				}
			}

			_SAFE_RELEASE(pConstantTable);
			if (pData) delete[] pData;
		}
		// else shader already recorded
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
		if (std::find(m_activeVShaders.begin(), m_activeVShaders.end(), m_currentVertexShaderHash) == m_activeVShaders.end()) {
			m_activeVShaders.push_back(m_currentVertexShaderHash);
		}

		// avoid draw if shader present in excluded vector
		if (std::find(m_excludedVShaders.begin(), m_excludedVShaders.end(), m_currentVertexShaderHash) == m_excludedVShaders.end()) {
			m_bAvoidDraw = false;
		}
		else
		{
			if ((GetTickCount()%300)>150)
				m_bAvoidDraw = true;
			else m_bAvoidDraw = false;
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
				char buf[32];
				sprintf_s(buf,"Set Vertex Shader: %u", m_currentVertexShaderHash);
				OutputDebugString(buf);
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
		// loop through relevant vertex shader constants
		auto itShaderConstants = m_relevantVSConstants.begin();
		while (itShaderConstants != m_relevantVSConstants.end())
		{
			// is a constant of current shader ?
			// start register ? 
			if ((itShaderConstants->hash == m_currentVertexShaderHash) &&
				(itShaderConstants->desc.RegisterIndex < (StartRegister+Vector4fCount)) &&
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
					if ((!vireio::AlmostSame(matrix[12], 1.0f, 0.00001f)) && (vireio::AlmostSame(matrix[12], -1.0f, 0.00001f)))
						m_bTransposedRules = true;
				}
			}
			++itShaderConstants;
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

		// No idea what happens if the same vertex shader is created twice. Pointer to the same shader or two
		// separate instances? guessing separate in which case m_recordedShader as is is pointless. 
		// TODO Replace pointer check with check on hash of shader data. (and print hash with data)
		if (m_recordedPShaders.insert(pActualShader).second && m_shaderDumpFile.is_open()) {

			// insertion succeeded - record shader details.
			LPD3DXCONSTANTTABLE pConstantTable = NULL;

			BYTE* pData = NULL;
			UINT pSizeOfData;
			pActualShader->GetFunction(NULL, &pSizeOfData);

			pData = new BYTE[pSizeOfData];
			pActualShader->GetFunction(pData, &pSizeOfData);

			uint32_t hash = 0;
			MurmurHash3_x86_32(pData, pSizeOfData, VIREIO_SEED, &hash); 

			D3DXGetShaderConstantTable(reinterpret_cast<DWORD*>(pData), &pConstantTable);

			if(pConstantTable == NULL) 
				return creationResult;

			D3DXCONSTANTTABLE_DESC pDesc;
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
							//m_relevantVSConstants.push_back(sc);
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
					oLogFile << "// Shader Creator: " << pDesc.Creator << std::endl;
					oLogFile << "// Shader Version: " << pDesc.Version << std::endl;
					oLogFile << "// Shader Hash   : " << hash << std::endl;
				}
			}

			_SAFE_RELEASE(pConstantTable);
			if (pData) delete[] pData;
		}
		// else shader already recorded
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
		if (std::find(m_activePShaders.begin(), m_activePShaders.end(), currentPixelShaderHash) == m_activePShaders.end()) {
			m_activePShaders.push_back(currentPixelShaderHash);
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
	char buf[32];
	sprintf_s(buf,"Cur Vertex Shader: %u", m_currentVertexShaderHash);
	OutputDebugString(buf);
	sprintf_s(buf,"Set Pixel Shader: %u", currentPixelShaderHash);
	OutputDebugString(buf);
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
void DataGatherer::BRASSA_ShaderSubMenu()
{
	UINT menuEntryCount = 6;

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;
	
	UINT entryID;
	BRASSA_NewFrame(entryID, menuEntryCount);

	/**
	* ESCAPE : Set BRASSA inactive and save the configuration.
	***/
	if (controls.Key_Down(VK_ESCAPE))
	{
		BRASSA_mode = BRASSA_Modes::INACTIVE;
	}

	if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// 
		if (entryID == 0)
		{
			// create relevant shader constant table
			GetCurrentShaderRules(true);
			BRASSA_mode = BRASSA_Modes::INACTIVE;
			menuVelocity.x+=2.0f;
			Analyze();
		}
		// 
		if (entryID == 1)
		{
			// create menu names new
			GetCurrentShaderRules(false);
			BRASSA_mode = BRASSA_Modes::CHANGE_RULES_SCREEN;
			menuVelocity.x+=2.0f;
		}
		//// pick rules
		//if (entryID == 2)
		//{
		//	BRASSA_mode = BRASSA_Modes::PICK_RULES_SCREEN;
		//	menuVelocity.x+=2.0f;
		//}
		// show shaders
		if (entryID == 2)
		{
			BRASSA_mode = BRASSA_Modes::SHOW_SHADERS_SCREEN;
			menuVelocity.x+=2.0f;
		}
		// save rules
		if (entryID == 3)
		{
			BRASSA_mode = BRASSA_Modes::INACTIVE;
			menuVelocity.x+=2.0f;
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
		// back to main menu
		if (entryID == 4)
		{
			BRASSA_mode = BRASSA_Modes::MAINMENU;
			menuVelocity.x+=2.0f;
		}
		// back to game
		if (entryID == 5)
		{
			BRASSA_mode = BRASSA_Modes::INACTIVE;
		}
	}

	// output menu
	if (hudFont)
	{
		// adjust border
		float borderDrawingHeight = borderTopHeight;
		if (menuVelocity.y == 0.0f)
			borderTopHeight = menuTop+menuEntryHeight*(float)entryID;

		// draw border - total width due to shift correction
		D3DRECT rect;
		rect.x1 = (int)0; rect.x2 = (int)viewportWidth; rect.y1 = (int)borderTopHeight; rect.y2 = (int)(borderTopHeight+viewportHeight*0.04f);
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		menuHelperRect.left = 550;
		menuHelperRect.top = 300;
		D3DProxyDevice::DrawTextShadowed(hudFont, hudMainMenu, "Brown Reischl and Schneider Settings Analyzer (B.R.A.S.S.A.).\n", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		rect.x1 = 0; rect.x2 = viewportWidth; rect.y1 = (int)(335*fScaleY); rect.y2 = (int)(340*fScaleY);
		Clear(1, &rect, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255,255,128,128), 0, 0);

		menuHelperRect.top += 50;  menuHelperRect.left += 250; float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		DrawTextShadowed(hudFont, hudMainMenu, "Create new Shader Rules", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += 40;
		DrawTextShadowed(hudFont, hudMainMenu, "Change current Shader Rules", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		/*menuHelperRect.top += 40;
		DrawTextShadowed(hudFont, hudMainMenu, "Pick Rules by active Shaders", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));*/
		menuHelperRect.top += 40;
		DrawTextShadowed(hudFont, hudMainMenu, "Show and exclude active Shaders", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += 40;
		DrawTextShadowed(hudFont, hudMainMenu, "Save Shader Rules", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += 40;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to BRASSA Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += 40;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;
		
		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* Change current shader rules.
***/
void DataGatherer::BRASSA_ChangeRules()
{
	menuHelperRect.left = 0;
	menuHelperRect.top = 0;

	UINT menuEntryCount = 2;
	UINT constantIndex = 0;
	std::vector<std::string> menuEntries;
	std::vector<bool> menuColor;
	std::vector<DWORD> menuID;
	// loop through relevant vertex shader constants
	auto itShaderConstants = m_relevantVSConstantNames.begin();
	while (itShaderConstants != m_relevantVSConstantNames.end())
	{
		menuColor.push_back(itShaderConstants->hasRule);
		menuID.push_back(constantIndex);
		menuEntries.push_back(itShaderConstants->name);
		if (itShaderConstants->nodeOpen)
		{
			// output the class
			menuColor.push_back(itShaderConstants->hasRule);
			menuID.push_back(constantIndex+(1<<31));
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
			menuColor.push_back(itShaderConstants->hasRule);
			menuID.push_back(constantIndex+(1<<30));
			if (itShaderConstants->hasRule)
				menuEntries.push_back("  "+itShaderConstants->ruleName);
			else
				menuEntries.push_back("  No Rule assigned");
			menuEntryCount++;

			// output wether transposed or not
			if ((itShaderConstants->hasRule) && (itShaderConstants->desc.Class != D3DXPC_VECTOR))
			{
				menuColor.push_back(itShaderConstants->hasRule);
				menuID.push_back(constantIndex+(1<<29));
				if (itShaderConstants->isTransposed)
					menuEntries.push_back("  Transposed");
				else
					menuEntries.push_back("  Non-Transposed");
				menuEntryCount++;
			}
		}

		constantIndex++;
		menuEntryCount++;
		++itShaderConstants;
	}

	UINT entryID;
	BRASSA_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;

	/**
	* ESCAPE : Set BRASSA inactive and save the configuration.
	***/
	if (controls.Key_Down(VK_ESCAPE))
	{
		BRASSA_mode = BRASSA_Modes::INACTIVE;
	}

	if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// switch shader rule node
		if ((entryID >= 0) && (entryID < menuEntryCount-2) && (menuEntryCount>2))
		{
			// constant node entry ?
			if ((menuID[entryID] & (1<<31)) == (1<<31))
			{
				// no influence on class node entry
			}
			else if ((menuID[entryID] & (1<<30)) == (1<<30)) // add/delete rule
			{
				// no rule present, so add
				if (!m_relevantVSConstantNames[menuID[entryID]].hasRule)
				{
					auto itShaderConstants = m_relevantVSConstants.begin();
					while (itShaderConstants != m_relevantVSConstants.end())
					{
						// constant name in menu entries already present
						if (itShaderConstants->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) == 0)
						{
							// never assign "transposed" to vector
							if (itShaderConstants->desc.Class == D3DXPARAMETER_CLASS::D3DXPC_VECTOR)
								addRule(itShaderConstants->name, true, itShaderConstants->desc.RegisterIndex, itShaderConstants->desc.Class, 1, false);
							else
								addRule(itShaderConstants->name, true, itShaderConstants->desc.RegisterIndex, itShaderConstants->desc.Class, 1, m_bTransposedRules);
							itShaderConstants->hasRule = true;

							// set the menu output accordingly
							auto itShaderConstants1 = m_relevantVSConstantNames.begin();
							while (itShaderConstants1 != m_relevantVSConstantNames.end())
							{
								// set rule bool for all relevant constant names
								if (itShaderConstants1->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) == 0)
								{
									UINT operation;
									itShaderConstants1->hasRule = m_pGameHandler->GetShaderModificationRepository()->ConstantHasRule(itShaderConstants1->name, itShaderConstants1->ruleName, operation, itShaderConstants1->isTransposed);
								}
								++itShaderConstants1;
							}
						}

						++itShaderConstants;
					}
				}
				else // rule present, so delete
				{
					deleteRule(m_relevantVSConstantNames[menuID[entryID]].name);

					// set the menu output accordingly
					auto itShaderConstants1 = m_relevantVSConstantNames.begin();
					while (itShaderConstants1 != m_relevantVSConstantNames.end())
					{
						// set rule bool for all relevant constant names
						if (itShaderConstants1->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) == 0)
						{
							UINT operation;
							itShaderConstants1->hasRule = m_pGameHandler->GetShaderModificationRepository()->ConstantHasRule(itShaderConstants1->name, itShaderConstants1->ruleName, operation, itShaderConstants1->isTransposed);
						}
						++itShaderConstants1;
					}
				}
			}
			else if ((menuID[entryID] & (1<<29)) == (1<<29))
			{
				bool newTrans = !m_relevantVSConstantNames[menuID[entryID]].isTransposed;
				// transposed or non-transposed
				auto itShaderConstants = m_relevantVSConstants.begin();
				while (itShaderConstants != m_relevantVSConstants.end())
				{
					// constant name in menu entries already present
					if (itShaderConstants->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) == 0)
					{
						// get the operation id
						UINT operation;
						m_pGameHandler->GetShaderModificationRepository()->ConstantHasRule(itShaderConstants->name, itShaderConstants->ruleName, operation, itShaderConstants->isTransposed);
						modifyRule(itShaderConstants->name, operation, newTrans);

						// set the menu output accordingly
						auto itShaderConstants1 = m_relevantVSConstantNames.begin();
						while (itShaderConstants1 != m_relevantVSConstantNames.end())
						{
							// set rule bool for all relevant constant names
							if (itShaderConstants1->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) == 0)
							{
								itShaderConstants1->hasRule = m_pGameHandler->GetShaderModificationRepository()->ConstantHasRule(itShaderConstants1->name, itShaderConstants1->ruleName, operation, itShaderConstants1->isTransposed);
							}
							++itShaderConstants1;
						}
					}

					itShaderConstants++;
				}
			}
			else
			{
				// open or close node
				m_relevantVSConstantNames[menuID[entryID]].nodeOpen = !m_relevantVSConstantNames[menuID[entryID]].nodeOpen;

				auto itShaderConstants = m_relevantVSConstants.begin();
				while (itShaderConstants != m_relevantVSConstants.end())
				{
					// constant name in menu entries already present
					if (itShaderConstants->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) == 0)
					{
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
					++itShaderConstants;
				}
			}

			menuVelocity.x+=2.0f;
		}
		// back to main menu
		if (entryID == menuEntryCount-2)
		{
			BRASSA_mode = BRASSA_Modes::MAINMENU;
			menuVelocity.x+=2.0f;
		}
		// back to game
		if (entryID == menuEntryCount-1)
		{
			BRASSA_mode = BRASSA_Modes::INACTIVE;
		}
	}

	if ((controls.Key_Down(VK_LEFT) || controls.Key_Down(0x4A) || (controls.xInputState.Gamepad.sThumbLX<-8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// switch shader rule node
		if ((entryID >= 0) && (entryID < menuEntryCount-2) && (menuEntryCount>2))
		{
			if ((menuID[entryID] & (1<<30)) == (1<<30)) // rule node entry
			{
				// rule present, so modify
				if (m_relevantVSConstantNames[menuID[entryID]].hasRule)
				{
					// get the operation id
					UINT operation;
					m_pGameHandler->GetShaderModificationRepository()->ConstantHasRule(m_relevantVSConstantNames[menuID[entryID]].name, m_relevantVSConstantNames[menuID[entryID]].ruleName, operation, m_relevantVSConstantNames[menuID[entryID]].isTransposed);
					if (operation > 0)
						operation--;

					auto itShaderConstants = m_relevantVSConstants.begin();
					while (itShaderConstants != m_relevantVSConstants.end())
					{
						// constant name in menu entries already present
						if (itShaderConstants->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) == 0)
						{
							modifyRule(itShaderConstants->name, operation, itShaderConstants->isTransposed);

							// set the menu output accordingly
							auto itShaderConstants1 = m_relevantVSConstantNames.begin();
							while (itShaderConstants1 != m_relevantVSConstantNames.end())
							{
								// set rule bool for all relevant constant names
								if (itShaderConstants1->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) == 0)
								{
									itShaderConstants1->hasRule = m_pGameHandler->GetShaderModificationRepository()->ConstantHasRule(itShaderConstants1->name, itShaderConstants1->ruleName, operation, itShaderConstants1->isTransposed);
								}
								++itShaderConstants1;
							}
						}

						itShaderConstants++;
					}
				}
			}
		}
		menuVelocity.x+=2.0f;
	}

	if ((controls.Key_Down(VK_RIGHT) || controls.Key_Down(0x4C) || (controls.xInputState.Gamepad.sThumbLX>8192)) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// switch shader rule node
		if ((entryID >= 0) && (entryID < menuEntryCount-2) && (menuEntryCount>2))
		{
			if ((menuID[entryID] & (1<<30)) == (1<<30)) // rule node entry
			{
				// rule present, so modify
				if (m_relevantVSConstantNames[menuID[entryID]].hasRule)
				{
					// get the operation id
					UINT operation;
					m_pGameHandler->GetShaderModificationRepository()->ConstantHasRule(m_relevantVSConstantNames[menuID[entryID]].name, m_relevantVSConstantNames[menuID[entryID]].ruleName, operation, m_relevantVSConstantNames[menuID[entryID]].isTransposed);
					if (m_relevantVSConstantNames[menuID[entryID]].desc.Class == D3DXPARAMETER_CLASS::D3DXPC_VECTOR)
					{
						if (operation < (UINT)ShaderConstantModificationFactory::Vec4EyeShiftUnity)
							operation++;
					}
					else
					{
						if (operation < (UINT)ShaderConstantModificationFactory::MatConvergenceOffset)
							operation++;
					}

					auto itShaderConstants = m_relevantVSConstants.begin();
					while (itShaderConstants != m_relevantVSConstants.end())
					{
						// constant name in menu entries already present
						if (itShaderConstants->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) == 0)
						{
							modifyRule(itShaderConstants->name, operation, itShaderConstants->isTransposed);

							// set the menu output accordingly
							auto itShaderConstants1 = m_relevantVSConstantNames.begin();
							while (itShaderConstants1 != m_relevantVSConstantNames.end())
							{
								// set rule bool for all relevant constant names
								if (itShaderConstants1->name.compare(m_relevantVSConstantNames[menuID[entryID]].name) == 0)
								{
									itShaderConstants1->hasRule = m_pGameHandler->GetShaderModificationRepository()->ConstantHasRule(itShaderConstants1->name, itShaderConstants1->ruleName, operation, itShaderConstants1->isTransposed);
								}
								++itShaderConstants1;
							}
						}

						itShaderConstants++;
					}
				}
			}
		}
		menuVelocity.x+=2.0f;
	}

	// output menu
	if (hudFont)
	{
		// adjust border & menu due to menu scroll
		float borderDrawingHeight = borderTopHeight;
		if (menuVelocity.y == 0.0f)
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
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		menuHelperRect.left = 800; menuHelperRect.top = 350;
		menuHelperRect.top += (int)(menuTopHeight / fScaleY);
		for (UINT i = 0; i < menuEntryCount-2; i++)
		{
			if (menuColor[i])
				DrawTextShadowed(hudFont, hudMainMenu, menuEntries[i].c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 64, 255, 64));
			else	
				DrawTextShadowed(hudFont, hudMainMenu, menuEntries[i].c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

			menuHelperRect.top += 40;
		}
		DrawTextShadowed(hudFont, hudMainMenu, "Back to BRASSA Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += 40;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;
		
		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* Pick shader rules out of active shaders.
***/
void DataGatherer::BRASSA_PickRules()
{
	// ERROR_CALL_NOT_IMPLEMENTED
}

/**
* Show currently used Shaders (Hash code) and exclude from rendering, if chosen.
***/
void DataGatherer::BRASSA_ShowActiveShaders()
{
	// sort the vertex shader vector
	std::sort (m_activeVShadersLastFrame.begin(), m_activeVShadersLastFrame.end());
	// sort the pixel shader vector
	std::sort (m_activePShadersLastFrame.begin(), m_activePShadersLastFrame.end());

	menuHelperRect.left = 0;
	menuHelperRect.top = 0;
	
	UINT menuEntryCount = 2;
	std::vector<std::string> menuEntries;
	std::vector<bool> menuColor;
	std::vector<uint32_t> menuID;
	std::string menuEntry;
	// loop through relevant vertex shaders
	auto itVShaderHash = m_activeVShadersLastFrame.begin();
	while (itVShaderHash != m_activeVShadersLastFrame.end())
	{
		bool excluded = false;
		// show colored if shader is drawn
		if (std::find(m_excludedVShaders.begin(), m_excludedVShaders.end(), *itVShaderHash) == m_excludedVShaders.end()) {
			excluded = true;
		}
		menuColor.push_back(excluded);
		menuID.push_back(*itVShaderHash);
		char buf[256];
		sprintf_s(buf, 256, "VS : %u", *itVShaderHash);
		menuEntry = std::string(buf);
		menuEntries.push_back(menuEntry);

		menuEntryCount++;
		++itVShaderHash;
	}
	UINT endOfVertexShaderEntries = menuEntryCount-2;
	// loop through relevant pixel shaders
	auto itPShaderHash = m_activePShadersLastFrame.begin();
	while (itPShaderHash != m_activePShadersLastFrame.end())
	{
		bool excluded = false;
		// show colored if shader is drawn
		if (std::find(m_excludedPShaders.begin(), m_excludedPShaders.end(), *itPShaderHash) == m_excludedPShaders.end()) {
			excluded = true;
		}
		menuColor.push_back(excluded);
		menuID.push_back(*itPShaderHash);
		char buf[256];
		sprintf_s(buf, 256, "PS : %u", *itPShaderHash);
		menuEntry = std::string(buf);
		menuEntries.push_back(menuEntry);

		menuEntryCount++;
		++itPShaderHash;
	}

	UINT entryID;
	BRASSA_NewFrame(entryID, menuEntryCount);
	UINT borderSelection = entryID;

	/**
	* ESCAPE : Set BRASSA inactive and save the configuration.
	***/
	if (controls.Key_Down(VK_ESCAPE))
	{
		BRASSA_mode = BRASSA_Modes::INACTIVE;
	}

	if ((controls.Key_Down(VK_RETURN) || controls.Key_Down(VK_RSHIFT) || (controls.xButtonsStatus[0x0c])) && (menuVelocity == D3DXVECTOR2(0.0f, 0.0f)))
	{
		// switch shader node (drawn/not-drawn)
		if ((entryID >= 0) && (entryID < menuEntryCount-2) && (menuEntryCount>2))
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

			menuVelocity.x+=2.0f;
		}
		// back to main menu
		if (entryID == menuEntryCount-2)
		{
			BRASSA_mode = BRASSA_Modes::MAINMENU;
			menuVelocity.x+=2.0f;
		}
		// back to game
		if (entryID == menuEntryCount-1)
		{
			BRASSA_mode = BRASSA_Modes::INACTIVE;
		}
	}



	// output menu
	if (hudFont)
	{
		// adjust border & menu due to menu scroll
		float borderDrawingHeight = borderTopHeight;
		if (menuVelocity.y == 0.0f)
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
		ClearEmptyRect(vireio::RenderPosition::Left, rect, D3DCOLOR_ARGB(255,255,128,128), 2);
		ClearEmptyRect(vireio::RenderPosition::Right, rect, D3DCOLOR_ARGB(255,255,128,128), 2);

		hudMainMenu->Begin(D3DXSPRITE_ALPHABLEND);

		D3DXMATRIX matScale;
		D3DXMatrixScaling(&matScale, fScaleX, fScaleY, 1.0f);
		hudMainMenu->SetTransform(&matScale);

		float guiQSHeight = (float)menuHelperRect.top * fScaleY;
		menuHelperRect.left = 800; menuHelperRect.top = 350;
		menuHelperRect.top += (int)(menuTopHeight / fScaleY);
		for (UINT i = 0; i < menuEntryCount-2; i++)
		{
			if (menuColor[i])
				DrawTextShadowed(hudFont, hudMainMenu, menuEntries[i].c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 64, 255, 64));
			else	
				DrawTextShadowed(hudFont, hudMainMenu, menuEntries[i].c_str(), -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

			menuHelperRect.top += 40;
		}
		DrawTextShadowed(hudFont, hudMainMenu, "Back to BRASSA Menu", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));
		menuHelperRect.top += 40;
		DrawTextShadowed(hudFont, hudMainMenu, "Back to Game", -1, &menuHelperRect, 0, D3DCOLOR_ARGB(255, 255, 255, 255));

		menuHelperRect.left = 0;
		menuHelperRect.top = 0;
		
		D3DXVECTOR3 vPos( 0.0f, 0.0f, 0.0f);
		hudMainMenu->Draw(NULL, &menuHelperRect, NULL, &vPos, D3DCOLOR_ARGB(255, 255, 255, 255));
		hudMainMenu->End();
	}
}

/**
* Analyzes the game and outputs a shader rule xml file.
***/
void DataGatherer::Analyze()
{
	// loop through relevant vertex shader constants
	auto itShaderConstants = m_relevantVSConstantNames.begin();
	while (itShaderConstants != m_relevantVSConstantNames.end())
	{
		// loop through matrix constant name assumptions
		for (int i = 0; i < MATRIX_NAMES; i++)
		{
			// test if assumption is found in constant name
			if (strstr(itShaderConstants->name.c_str(), m_wvpMatrixConstantNames[i].c_str()) != 0)
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
					if (addRule(itShaderConstants->name, true, itShaderConstants->desc.RegisterIndex, itShaderConstants->desc.Class, 2, m_bTransposedRules))
						m_addedVSConstants.push_back(*itShaderConstants);

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
					char buf[32];
					sprintf_s(buf,"Register Index: %d", itShaderConstants->desc.RegisterIndex);
					OutputDebugString(buf);
					sprintf_s(buf,"Shader Hash: %u", itShaderConstants->hash);
					OutputDebugString(buf);
					sprintf_s(buf,"Transposed: %d", m_bTransposedRules);
					OutputDebugString(buf);

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

		++itShaderConstants;
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
	auto itShaderConstants = m_relevantVSConstants.begin();
	while (itShaderConstants != m_relevantVSConstants.end())
	{
		bool namePresent = false;
		bool registerPresent = !allStartRegisters;
		auto itShaderConstants1 = m_relevantVSConstantNames.begin();
		while (itShaderConstants1 != m_relevantVSConstantNames.end())
		{
			// constant name in menu entries already present
			if (itShaderConstants->name.compare(itShaderConstants1->name) == 0)
			{
				namePresent = true;
				if (itShaderConstants->desc.RegisterIndex==itShaderConstants1->desc.RegisterIndex)
					registerPresent = true;
			}
			++itShaderConstants1;
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

		++itShaderConstants;
	}
}