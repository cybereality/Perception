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

#include "GameHandler.h"

#define IS_RENDER_TARGET(d3dusage) ((d3dusage & D3DUSAGE_RENDERTARGET) > 0 ? true : false)
#define IS_POOL_DEFAULT(d3dpool) ((d3dpool & D3DPOOL_DEFAULT) > 0 ? true : false)


GameHandler::GameHandler() :
	m_ShaderModificationRepository(nullptr),
	m_spShaderViewAdjustment(nullptr)
{
	HMDisplayInfo defaultInfo; // rift info
	m_spShaderViewAdjustment = std::make_shared<ViewAdjustment>(defaultInfo, 1.0f, false);
}

GameHandler::~GameHandler()
{
	if (m_ShaderModificationRepository)
		delete m_ShaderModificationRepository;

	if (m_spShaderViewAdjustment)
		m_spShaderViewAdjustment.reset();
}


bool GameHandler::Load(ProxyHelper::ProxyConfig& cfg)
{
	// Get rid of existing modification repository if there is one (shouldn't be, load should only need to be called once)
	if (m_ShaderModificationRepository) {
		delete m_ShaderModificationRepository;
		m_ShaderModificationRepository = nullptr;
	}

	m_spShaderViewAdjustment->EnableRoll(cfg.rollEnabled);
	m_spShaderViewAdjustment->SetWorldScaleFactor(cfg.worldScaleFactor);

	bool loadSuccess = true;

	//if (game profile has shader rules)
	if (!cfg.shaderRulePath.empty()) {
		m_ShaderModificationRepository = new ShaderModificationRepository(m_spShaderViewAdjustment);
		
		if (!m_ShaderModificationRepository->LoadRules(cfg.shaderRulePath)) {
			OutputDebugString("Rules failed to load.");
			loadSuccess = false;
		}
	}
	else {
		OutputDebugString("No shader rule path found.");
		// We call this success as we have successfully loaded nothing. We assume no rules is intentional
	}
	


	return true;
}

//TODO implementation - For now use one set of rules for everything, at some point this is probably going to need to be reworked to allow modifications per game.
// TODO - externalise these as rules? It might be good to have default values for various rules like
// "Don't duplicate targets smaller than X pixels", where that rule could be enabled and the value changed in the cfg file for the game
// Do something similar to shader modifications?
// Extending class and overriding these methods would end up with a similar problem with duplicate code to original shader handling. Try and avoid (or at least find approach that avoids the duplication)
bool GameHandler::ShouldDuplicateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,BOOL Lockable, bool isSwapChainBackBuffer)
{
	if (isSwapChainBackBuffer) {
		return true;
	}

	//!((Width == Height) || (Width <= 1024))) // Trying some random things out - this one fixes guy on screens in hl2 (but makes him left shifted - his shaders would need a non-stereo value or a modification that returns unmodified in place of left)
	//TODO implementation
	return true;

}

bool GameHandler::ShouldDuplicateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard)
{
	//TODO implementation
	return true;
}

bool GameHandler::ShouldDuplicateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage, D3DFORMAT Format,D3DPOOL Pool)
{
	//TODO implementation
	// IF render target then check render target rules?
	return IS_RENDER_TARGET(Usage);
}

bool GameHandler::ShouldDuplicateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool)
{
	//TODO implementation
	// IF render target then check render target rules?
	return IS_RENDER_TARGET(Usage);
}


ShaderModificationRepository* GameHandler::GetShaderModificationRepository()
{
	return m_ShaderModificationRepository;
}