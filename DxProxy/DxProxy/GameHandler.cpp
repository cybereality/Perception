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

/**
* Almost empty constructor.
***/
GameHandler::GameHandler() :
	m_ShaderModificationRepository(nullptr)
{
	
}

/**
* Almost empty destructor.
***/
GameHandler::~GameHandler()
{
	if (m_ShaderModificationRepository)
		delete m_ShaderModificationRepository;
}

/**
* Loads the shader view adjustments for the handled game.
* @param cfg The game configuration.
* @param spShaderViewAdjustments The view adjustments pointer.
***/
bool GameHandler::Load(ProxyHelper::ProxyConfig& cfg, std::shared_ptr<ViewAdjustment> spShaderViewAdjustments)
{
	// Get rid of existing modification repository if there is one
	if (m_ShaderModificationRepository) {
		delete m_ShaderModificationRepository;
		m_ShaderModificationRepository = nullptr;
	}

	bool loadSuccess = true;

	//if (game profile has shader rules)
	if (!cfg.shaderRulePath.empty()) {
		m_ShaderModificationRepository = new ShaderModificationRepository(spShaderViewAdjustments);
	
		if (!m_ShaderModificationRepository->LoadRules(cfg.shaderRulePath)) {
			OutputDebugString("Rules failed to load.");
			loadSuccess = false;
		}
	}
	else {
		OutputDebugString("No shader rule path found. No rules to apply");
		// We call this success as we have successfully loaded nothing. We assume 'no rules' is intentional
	}	

	return true;
}

/**
* Saves the shader view adjustments for the handled game.
* @param cfg The game configuration.
* @param spShaderViewAdjustments The view adjustments pointer.
***/
bool GameHandler::Save(ProxyHelper::ProxyConfig& cfg, std::shared_ptr<ViewAdjustment> spShaderViewAdjustments)
{
	OutputDebugString("Save shader rules...");

	// if (game profile has shader rules path) and (modification repository present)
	if ((!cfg.shaderRulePath.empty()) && (m_ShaderModificationRepository)) {
		if (!m_ShaderModificationRepository->SaveRules(cfg.shaderRulePath)) {
			OutputDebugString("Rules failed to save.");
		}
	}
	else {
		OutputDebugString("No shader rule path found. No rules to save.");
	}	

	return true;
}

/**
* True if the by parameters described render target is to be duplicated for the handled game.
* Currently, always returns true.
* @see D3DProxyDevice::CreateRenderTarget()
***/
bool GameHandler::ShouldDuplicateRenderTarget(UINT Width, UINT Height, D3DFORMAT Format, D3DMULTISAMPLE_TYPE MultiSample, DWORD MultisampleQuality,BOOL Lockable, bool isSwapChainBackBuffer)
{
	if (isSwapChainBackBuffer) {
		return true;
	}

	//return !((Width == Height) || (Width <= 1024)); // Trying some random things out - this one fixes guy on screens in hl2 (but makes him left shifted - his shaders would need a non-stereo value or a modification that returns unmodified in place of left)
	// enabling the line above breaks reflections in f1 2010
	//TODO implementation
	return true;

}

/**
* True if the by parameters described depth stencil surface is to be duplicated for the handled game.
* Currently, always returns true.
* @see D3DProxyDevice::CreateDepthStencilSurface()
***/
bool GameHandler::ShouldDuplicateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard)
{
	//TODO implementation
	return true;
}

/**
* True if the by parameters described texture is to be duplicated for the handled game.
* Currently, returns true if texture is a render target.
* @see D3DProxyDevice::CreateTexture()
***/
bool GameHandler::ShouldDuplicateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage, D3DFORMAT Format,D3DPOOL Pool)
{
	//TODO implementation
	// IF render target then check render target rules?
	return IS_RENDER_TARGET(Usage);
}

/**
* True if the by parameters described cube texture is to be duplicated for the handled game.
* Currently, returns true if cube texture is a render target.
* @see D3DProxyDevice::CreateCubeTexture()
***/
bool GameHandler::ShouldDuplicateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool)
{
	//TODO implementation
	// IF render target then check render target rules?
	return IS_RENDER_TARGET(Usage);
}

/**
* Add a default rule to the existing shader modification repository or create a new and add.
***/
void GameHandler::AddRule(std::shared_ptr<ViewAdjustment> spShaderViewAdjustments, std::string constantName, bool allowPartialNameMatch, UINT startRegIndex, D3DXPARAMETER_CLASS constantType, UINT operationToApply, bool transpose)
{
	// repository present ?
	if (!m_ShaderModificationRepository)
		m_ShaderModificationRepository = new ShaderModificationRepository(spShaderViewAdjustments);

	// get unique id and add rule
	UINT uniqueID = m_ShaderModificationRepository->GetUniqueRuleID();
	m_ShaderModificationRepository->AddRule(constantName, allowPartialNameMatch, startRegIndex, constantType, operationToApply, uniqueID, transpose);
}

/**
* Returns the shader modification repository for the handled game.
***/
ShaderModificationRepository* GameHandler::GetShaderModificationRepository()
{
	return m_ShaderModificationRepository;
}