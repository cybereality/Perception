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

#ifndef GAMEHANDLER_H_INCLUDED
#define GAMEHANDLER_H_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>
#include "d3d9.h"
#include "MurmurHash3.h"
#include "ShaderConfig.h"

class GameHandler
{
public:
	GameHandler();
	virtual ~GameHandler();

	static GameHandler* CreateFor(/* game (std::string gameId)*/);

	bool ShouldDuplicateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample,
										DWORD MultisampleQuality,BOOL Lockable,IDirect3DSurface9** ppSurface,HANDLE* pSharedHandle, bool isSwapChainBackBuffer);
	bool ShouldDuplicateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture9** ppTexture,HANDLE* pSharedHandle);
	bool ShouldDuplicateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DCubeTexture9** ppCubeTexture, HANDLE* pSharedHandle);

	float ToWorldUnits(float millimeters);

	vector<ShaderConstantConfig> ConstantConfigsForShader(Hash128Bit shaderHash);
	bool ConstantConfigsForShaderExists(Hash128Bit shaderHash);

private:

	std::unordered_map<Hash128Bit, vector<ShaderConstantConfig>> m_constantModifications;

	std::string m_gameId;
};

#endif