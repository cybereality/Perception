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
#include <memory>
#include <unordered_map>
#include "d3d9.h"
#include "ShaderModificationRepository.h"

class ShaderModificationRepository;

class GameHandler
{
public:
	GameHandler(); 
	virtual ~GameHandler();

	void Load(std::string gameId, std::shared_ptr<ViewAdjustment> adjustmentMatricies); //ifstream/xml_doc?	


	bool ShouldDuplicateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,BOOL Lockable, bool isSwapChainBackBuffer);
	bool ShouldDuplicateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard);
	bool ShouldDuplicateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool);
	bool ShouldDuplicateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool);

	float ToWorldUnits(float millimeters);

	ShaderModificationRepository* GetShaderModificationRepository();


	

private:

	ShaderModificationRepository* m_ShaderModificationRepository;

	//std::string m_gameName;
	float m_fWorldScaleFactor;

	bool rollEnabled;
};

#endif