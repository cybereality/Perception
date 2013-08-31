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

#ifndef DATAGATHERER_H_INCLUDED
#define DATAGATHERER_H_INCLUDED

#include "Direct3DDevice9.h"
#include "D3DProxyDevice.h"
#include "ProxyHelper.h"
#include "MurmurHash3.h"
#include "Direct3DVertexShader9.h"

/**
* Data gatherer class, outputs relevant shader data to dump file (.csv format) .
* Outputs Shader Hash,Constant Name,ConstantType,Start Register,Register Count to "vertexShaderDump.csv".
* Used ".csv" file format to easily open and sort using OpenOffice (for example). These informations let 
* you create new shader rules.
* (if compiled to debug, it outputs shader code to "VS(hash).txt")
*/
class DataGatherer : public D3DProxyDevice
{
public:
	DataGatherer(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy);
	virtual ~DataGatherer();
		
	/*** IDirect3DDevice9 methods ***/
	virtual HRESULT WINAPI CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader);	

	/*** DataGatherer methods ***/
	virtual void Init(ProxyHelper::ProxyConfig& cfg);

private:
	/**
	* Set of recorded shaders, to avoid double output.
	***/
	std::unordered_set<IDirect3DVertexShader9*> m_recordedShaders;
	/**
	* The shader dump file (.csv format).
	***/
	std::ofstream m_shaderDumpFile;
};

#endif