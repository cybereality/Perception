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

/**
* Constructor, opens the dump file.
* @param pDevice Imbed actual device.
* @param pCreatedBy Pointer to the object that created the device.
***/
DataGatherer::DataGatherer(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy):D3DProxyDevice(pDevice, pCreatedBy),
	m_recordedShaders()
{
	m_shaderDumpFile.open("vertexShaderDump.csv", std::ios::out);

	m_shaderDumpFile << "Shader Hash,Constant Name,ConstantType,Start Register,Register Count" << std::endl;
}

/**
* Destructor, closes the dump file.
***/
DataGatherer::~DataGatherer()
{
	m_shaderDumpFile.close();
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
		if (m_recordedShaders.insert(pActualShader).second && m_shaderDumpFile.is_open()) {

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
							m_shaderDumpFile << "," << pConstantDesc[j].RegisterCount << std::endl;
					}
				}
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
* Calls the super init-method.
* Implemented here for possible future use.
* @param cfg The game configuration to init this class.
***/
void DataGatherer::Init(ProxyHelper::ProxyConfig& cfg)
{
	OutputDebugString("Special Proxy: Shader data gatherer created.\n");

	D3DProxyDevice::Init(cfg);
}