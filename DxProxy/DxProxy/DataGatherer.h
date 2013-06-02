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
#include "Direct3DVertexShader9.h"

class DataGatherer : public D3DProxyDevice
{
public:
	DataGatherer(IDirect3DDevice9* pDevice, BaseDirect3D9* pCreatedBy);
	virtual ~DataGatherer();
	
	virtual HRESULT WINAPI CreateVertexShader(CONST DWORD* pFunction,IDirect3DVertexShader9** ppShader);

	virtual void Init(ProxyHelper::ProxyConfig& cfg);
	

private:
	std::unordered_set<IDirect3DVertexShader9*> m_recordedShaders;
	std::ofstream m_shaderDumpFile;
};

#endif