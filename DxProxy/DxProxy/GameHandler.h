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
#include "pugixml.hpp"
#include "ShaderModificationRepository.h"
#include "ProxyHelper.h"
#include "ViewAdjustment.h"

class ShaderModificationRepository;

/**
* Game-specific shader modification handling class.
* TODO implementation - For now use one set of rules for everything, at some point this is probably 
* going to need to be reworked to allow modifications per game.
* TODO - externalise these as rules? It might be good to have default values for various rules like
* "Don't duplicate targets smaller than X pixels", where that rule could be enabled and the value 
* changed in the cfg file for the game.
* Do something similar to shader modifications?
* Extending class and overriding these methods would end up with a similar problem with duplicate code
* to original shader handling. Try and avoid (or at least find approach that avoids the duplication)
* 
*/
class GameHandler
{
public:
	GameHandler(); 
	virtual ~GameHandler();

	/*** GameHandler public methods ***/
	bool                          Load(ProxyHelper::ProxyConfig& cfg, std::shared_ptr<ViewAdjustment> spShaderViewAdjustments);
	bool                          Save(ProxyHelper::ProxyConfig& cfg, std::shared_ptr<ViewAdjustment> spShaderViewAdjustments);
	bool                          ShouldDuplicateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,BOOL Lockable, bool isSwapChainBackBuffer);
	bool                          ShouldDuplicateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard);
	bool                          ShouldDuplicateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool);
	bool                          ShouldDuplicateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool);
	void                          AddRule(std::shared_ptr<ViewAdjustment> spShaderViewAdjustments, std::string constantName, bool allowPartialNameMatch, UINT startRegIndex, D3DXPARAMETER_CLASS constantType, UINT operationToApply, bool transpose);
	ShaderModificationRepository* GetShaderModificationRepository();
private:
	/**
	* The shader modification repository for the handled game.
	***/
	ShaderModificationRepository* m_ShaderModificationRepository;
};
#endif