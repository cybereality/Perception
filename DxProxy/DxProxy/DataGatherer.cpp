/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2013 Chris Drain

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

#define KEY_DOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 1 : 0)
#define KEY_UP(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? 0 : 1)

#define MATRIX_NAMES 14
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
	m_startAnalyzingTool(false),
	m_analyzingFrameCounter(0)
{
	m_shaderDumpFile.open("shaderDump.csv", std::ios::out);

	m_shaderDumpFile << "Shader Hash,Constant Name,ConstantType,Start Register,Register Count,Vertex/Pixel Shader" << std::endl;

	// create matrix name array 
	static std::string names[] = { "ViewProj", "viewproj", "viewProj", "wvp", "mvp", "WVP", "MVP", "wvP", "mvP", "matFinal", "matrixFinal", "MatrixFinal", "FinalMatrix", "finalMatrix" };
	m_wvpMatrixConstantNames = names;
	static std::string avoid[] = { "Inv", "inv" };
	m_wvpMatrixAvoidedSubstrings = avoid;
}

/**
* Destructor, closes the dump file.
***/
DataGatherer::~DataGatherer()
{
	m_shaderDumpFile.close();
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
* Increases the vertex shader call counter and draws.
***/
HRESULT WINAPI DataGatherer::DrawPrimitive(D3DPRIMITIVETYPE PrimitiveType,UINT StartVertex,UINT PrimitiveCount)
{
	// increase shader count
	auto itRelevantShaders = m_vertexShaderCallCount.begin();
	while (itRelevantShaders != m_vertexShaderCallCount.end())
	{
		if (itRelevantShaders->first == m_currentVertexShaderHash)
			itRelevantShaders->second++;
		++itRelevantShaders;
	}
	return D3DProxyDevice::DrawPrimitive(PrimitiveType, StartVertex, PrimitiveCount);
}

/**
* Increases the vertex shader call counter and draws. 
***/
HRESULT WINAPI DataGatherer::DrawIndexedPrimitive(D3DPRIMITIVETYPE PrimitiveType,INT BaseVertexIndex,UINT MinVertexIndex,UINT NumVertices,UINT startIndex,UINT primCount)
{
	// increase shader count
	auto itRelevantShaders = m_vertexShaderCallCount.begin();
	while (itRelevantShaders != m_vertexShaderCallCount.end())
	{
		if (itRelevantShaders->first == m_currentVertexShaderHash)
			itRelevantShaders->second++;
		++itRelevantShaders;
	}

	return D3DProxyDevice::DrawIndexedPrimitive(PrimitiveType, BaseVertexIndex, MinVertexIndex, NumVertices, startIndex, primCount);
}

/**
* Increases the vertex shader call counter and draws.
***/
HRESULT WINAPI DataGatherer::DrawPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT PrimitiveCount,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	// increase shader count
	auto itRelevantShaders = m_vertexShaderCallCount.begin();
	while (itRelevantShaders != m_vertexShaderCallCount.end())
	{
		if (itRelevantShaders->first == m_currentVertexShaderHash)
			itRelevantShaders->second++;
		++itRelevantShaders;
	}

	return D3DProxyDevice::DrawPrimitiveUP(PrimitiveType, PrimitiveCount, pVertexStreamZeroData, VertexStreamZeroStride);
}

/**
* Increases the vertex shader call counter and draws.
***/
HRESULT WINAPI DataGatherer::DrawIndexedPrimitiveUP(D3DPRIMITIVETYPE PrimitiveType,UINT MinVertexIndex,UINT NumVertices,UINT PrimitiveCount,CONST void* pIndexData,D3DFORMAT IndexDataFormat,CONST void* pVertexStreamZeroData,UINT VertexStreamZeroStride)
{
	// increase shader count
	auto itRelevantShaders = m_vertexShaderCallCount.begin();
	while (itRelevantShaders != m_vertexShaderCallCount.end())
	{
		if (itRelevantShaders->first == m_currentVertexShaderHash)
			itRelevantShaders->second++;
		++itRelevantShaders;
	}

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

				// if data found, add to vertex shader call count map
				bool dataFound = false;

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

							dataFound = true;

							// add constant to relevant constant vector
							ShaderConstant sc;
							sc.hash = hash;
							sc.desc = D3DXCONSTANT_DESC(pConstantDesc[j]);
							sc.name = std::string(pConstantDesc[j].Name);
							sc.transposed = false;
							m_relevantVSConstants.push_back(sc);
					}
				}

				// shader contains relevant data, so add to call count map
				if (dataFound)
					m_vertexShaderCallCount.insert(std::pair<UINT, UINT>(hash, 0));
			}

#ifdef _DEBUG
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
#endif

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
	m_currentVertexShaderHash = ShaderHash(pShader);

	char buf[32];
	sprintf_s(buf,"Set Vertex Shader: %u", m_currentVertexShaderHash);
	OutputDebugString(buf);

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
	// loop through relevant vertex shader constants
	auto itShaderConstants = m_relevantVSConstants.begin();
	while (itShaderConstants != m_relevantVSConstants.end())
	{
		// is a constant of current shader ?
		// start register ? 
		if ((itShaderConstants->hash == m_currentVertexShaderHash) &&
			(itShaderConstants->desc.RegisterIndex < (StartRegister+(Vector4fCount*4))) &&
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
				if ((vireio::AlmostSame(matrix[14], 1.0f, 0.00001f)) || (vireio::AlmostSame(matrix[14], -1.0f, 0.00001f)))
					itShaderConstants->transposed = false;
				else 
					itShaderConstants->transposed = true;

			}
			else if (itShaderConstants->desc.Class == D3DXPARAMETER_CLASS::D3DXPC_MATRIX_COLUMNS)
			{
				// Perspective projection matrices have in the last column (0,0,1,0) if left-handed and 
				// * (0,0,-1,0) if right-handed.
				// Note that we DO NOT TEST here wether this is actually a projection matrix
				// (we do that in the analyze() method)
				D3DXMATRIX matrix = D3DXMATRIX(pConstantData+((itShaderConstants->desc.RegisterIndex-StartRegister)*4*sizeof(float)));

				// [12] for column matrix ??
				if ((vireio::AlmostSame(matrix[12], 1.0f, 0.00001f)) || (vireio::AlmostSame(matrix[12], -1.0f, 0.00001f)))
					itShaderConstants->transposed = false;
				else 
					itShaderConstants->transposed = true;
			}
		}
		++itShaderConstants;
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

				// if data found, add to vertex shader call count map
				bool dataFound = false;

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

							dataFound = true;

							// add constant to relevant constant vector
							ShaderConstant sc;
							sc.hash = hash;
							sc.desc = D3DXCONSTANT_DESC(pConstantDesc[j]);
							sc.name = std::string(pConstantDesc[j].Name);
							sc.transposed = false;
							m_relevantVSConstants.push_back(sc);
					}
				}

				// shader contains relevant data, so add to call count map
				if (dataFound)
					m_vertexShaderCallCount.insert(std::pair<UINT, UINT>(hash, 0));
			}

#ifdef _DEBUG
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
#endif

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
	// set the current vertex shader hash code for the call counter
	uint32_t currentPixelShaderHash = ShaderHash(pShader);

	char buf[32];
	sprintf_s(buf,"Cur Vertex Shader: %u", m_currentVertexShaderHash);
	OutputDebugString(buf);
	sprintf_s(buf,"Set Pixel Shader: %u", currentPixelShaderHash);
	OutputDebugString(buf);

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
* Adds Hotkey Button F7.
***/
void DataGatherer::HandleControls()
{
	static int keyWaitCount = 0;
	keyWaitCount--;

	if (keyWaitCount<=0)
	{
		/**
		* F7 : Activate analyzing tool.
		***/
		if(KEY_DOWN(VK_F7))
		{
			m_startAnalyzingTool = true;
			keyWaitCount = 50;
		}
	}
	D3DProxyDevice::HandleControls();
}

/**
* Analyzes the game and outputs a shader rule xml file.
***/
void DataGatherer::Analyze()
{
	UINT mostFrequentedShaderHash = 0;
	UINT calls = 0;

	// loop through relevant vertex shader constants
	auto itShaderConstants = m_relevantVSConstants.begin();
	while (itShaderConstants != m_relevantVSConstants.end())
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
					auto itRelevantShaders = m_vertexShaderCallCount.begin();
					while (itRelevantShaders != m_vertexShaderCallCount.end())
					{
						// was the shader used last frame ?
						if ((itRelevantShaders->first == itShaderConstants->hash) && (itRelevantShaders->second > 0))
						{
							// add this rule !!!!
							if (addRule(itShaderConstants->name, true, itShaderConstants->desc.RegisterIndex, itShaderConstants->desc.Class, 2, itShaderConstants->transposed))
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
							sprintf_s(buf,"Transposed: %d", itShaderConstants->transposed);
							OutputDebugString(buf);
						}

						++itRelevantShaders;
					}

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

	// get most frequented shader
	// TODO ... if still no rules present take the first matrix of the most frequented shader
	// maybe we need that for very weird shader constant names
	auto itRelevantShaders = m_vertexShaderCallCount.begin();
	while (itRelevantShaders != m_vertexShaderCallCount.end())
	{
		if (calls < itRelevantShaders->second)
		{
			mostFrequentedShaderHash = itRelevantShaders->first;
			calls = itRelevantShaders->second;
		}
		++itRelevantShaders;
	}

	// output most frequented shader
	OutputDebugString("Most frequented shader :");
	char buf[32];
	sprintf_s(buf,"Shader Hash: %u", mostFrequentedShaderHash);
	OutputDebugString(buf);
	sprintf_s(buf,"Shader Calls: %d", calls);
	OutputDebugString(buf);

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