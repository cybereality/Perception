/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <GameHandler.cpp> and
Class <GameHandler> :
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

#include "GameHandler.h"

#define IS_RENDER_TARGET(d3dusage) ((d3dusage & D3DUSAGE_RENDERTARGET) > 0 ? true : false)
#define IS_POOL_DEFAULT(d3dpool) ((d3dpool & D3DPOOL_DEFAULT) > 0 ? true : false)

/**
* Almost empty constructor.
***/
GameHandler::GameHandler() :
	m_ShaderModificationRepository(nullptr),
	m_gameType(0)
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

	// set the internal game type
	m_gameType = cfg.game_type;

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
	switch(m_gameType)
	{
	case D3DProxyDevice::ProxyTypes::SOURCE:
		if (isSwapChainBackBuffer) {
			return true;
		}
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::SOURCE_L4D:
		if (isSwapChainBackBuffer) {
			return true;
		}
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::UNREAL:
		if (isSwapChainBackBuffer) {
			return true;
		}
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::UNREAL_MIRROR:
		if (isSwapChainBackBuffer) {
			return true;
		}
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::UNREAL_UT3:
		if (isSwapChainBackBuffer) {
			return true;
		}
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::UNREAL_BIOSHOCK:
		if (isSwapChainBackBuffer) {
			return true;
		}
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::UNREAL_BORDERLANDS:
		if (isSwapChainBackBuffer) {
			return true;
		}
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::EGO:
		return true;  

	case D3DProxyDevice::ProxyTypes::EGO_DIRT:
		return true; 

	case D3DProxyDevice::ProxyTypes::REALV:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::REALV_ARMA:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::UNITY:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::UNITY_SLENDER:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::GAMEBRYO:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::GAMEBRYO_SKYRIM:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::LFS:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::CDC:
		// NOT TESTED NOW !
		return true;

	default:
		return true;

	}
}

/**
* True if the by parameters described depth stencil surface is to be duplicated for the handled game.
* Currently, always returns true.
* @see D3DProxyDevice::CreateDepthStencilSurface()
***/
bool GameHandler::ShouldDuplicateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard)
{
	switch(m_gameType)
	{
	case D3DProxyDevice::ProxyTypes::SOURCE:
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::SOURCE_L4D:
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::UNREAL:
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::UNREAL_MIRROR:
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::UNREAL_UT3:
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::UNREAL_BIOSHOCK:
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::UNREAL_BORDERLANDS:
		return Width != Height;

	case D3DProxyDevice::ProxyTypes::EGO:
		return true;

	case D3DProxyDevice::ProxyTypes::EGO_DIRT:
		return true;

	case D3DProxyDevice::ProxyTypes::REALV:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::REALV_ARMA:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::UNITY:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::UNITY_SLENDER:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::GAMEBRYO:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::GAMEBRYO_SKYRIM:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::LFS:
		// NOT TESTED NOW !
		return true;

	case D3DProxyDevice::ProxyTypes::CDC:
		// NOT TESTED NOW !
		return true;

	default:
		return true;

	}
}

/**
* True if the by parameters described texture is to be duplicated for the handled game.
* Currently, returns true if texture is a render target.
* @see D3DProxyDevice::CreateTexture()
***/
bool GameHandler::ShouldDuplicateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage, D3DFORMAT Format,D3DPOOL Pool)
{
	switch(m_gameType)
	{
	case D3DProxyDevice::ProxyTypes::SOURCE:
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage) && (Width != Height);

	case D3DProxyDevice::ProxyTypes::SOURCE_L4D:
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage) && (Width != Height);

	case D3DProxyDevice::ProxyTypes::UNREAL:
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage) && (Width != Height);

	case D3DProxyDevice::ProxyTypes::UNREAL_MIRROR:
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage) && (Width != Height);

	case D3DProxyDevice::ProxyTypes::UNREAL_UT3:
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage) && (Width != Height);

	case D3DProxyDevice::ProxyTypes::UNREAL_BIOSHOCK:
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage) && (Width != Height);

	case D3DProxyDevice::ProxyTypes::UNREAL_BORDERLANDS:
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage) && (Width != Height);

	case D3DProxyDevice::ProxyTypes::EGO:
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::EGO_DIRT:
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::REALV:
		// NOT TESTED NOW !
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::REALV_ARMA:
		// NOT TESTED NOW !
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::UNITY:
		// NOT TESTED NOW !
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::UNITY_SLENDER:
		// NOT TESTED NOW !
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::GAMEBRYO:
		// NOT TESTED NOW !
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::GAMEBRYO_SKYRIM:
		// NOT TESTED NOW !
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::LFS:
		// NOT TESTED NOW !
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::CDC:
		// NOT TESTED NOW !
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage);

	default:
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage);

	}
}

/**
* True if the by parameters described cube texture is to be duplicated for the handled game.
* Currently, returns true if cube texture is a render target.
* @see D3DProxyDevice::CreateCubeTexture()
***/
bool GameHandler::ShouldDuplicateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool)
{
	switch(m_gameType)
	{
	case D3DProxyDevice::ProxyTypes::SOURCE:
		return false;

	case D3DProxyDevice::ProxyTypes::SOURCE_L4D:
		return false;

	case D3DProxyDevice::ProxyTypes::UNREAL:
		return false;

	case D3DProxyDevice::ProxyTypes::UNREAL_MIRROR:
		return false;

	case D3DProxyDevice::ProxyTypes::UNREAL_UT3:
		return false;

	case D3DProxyDevice::ProxyTypes::UNREAL_BIOSHOCK:
		return false;

	case D3DProxyDevice::ProxyTypes::UNREAL_BORDERLANDS:
		return false;

	case D3DProxyDevice::ProxyTypes::EGO:
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::EGO_DIRT:
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::REALV:
		// NOT TESTED NOW !
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::REALV_ARMA:
		// NOT TESTED NOW !
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::UNITY:
		// NOT TESTED NOW !
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::UNITY_SLENDER:
		// NOT TESTED NOW !
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::GAMEBRYO:
		// NOT TESTED NOW !
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::GAMEBRYO_SKYRIM:
		// NOT TESTED NOW !
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::LFS:
		// NOT TESTED NOW !
		return IS_RENDER_TARGET(Usage);

	case D3DProxyDevice::ProxyTypes::CDC:
		// NOT TESTED NOW !
		return IS_RENDER_TARGET(Usage);

	default:
		return IS_RENDER_TARGET(Usage);

	}
}

/**
* Add a default rule to the existing shader modification repository or create a new and add.
* @return True if rule was added, false if rule already present.
***/
bool GameHandler::AddRule(std::shared_ptr<ViewAdjustment> spShaderViewAdjustments, std::string constantName, bool allowPartialNameMatch, UINT startRegIndex, D3DXPARAMETER_CLASS constantType, UINT operationToApply, bool transpose)
{
	// repository present ?
	if (!m_ShaderModificationRepository)
		m_ShaderModificationRepository = new ShaderModificationRepository(spShaderViewAdjustments);

	// get unique id and add rule
	UINT uniqueID = m_ShaderModificationRepository->GetUniqueRuleID();
	return m_ShaderModificationRepository->AddRule(constantName, allowPartialNameMatch, startRegIndex, constantType, operationToApply, uniqueID, transpose);
}

/**
* Modifies a default rule in a shader modification repository.
* @return True if rule was modified, false if rule not present.
***/
bool GameHandler::ModifyRule(std::shared_ptr<ViewAdjustment> spShaderViewAdjustments, std::string constantName, UINT operationToApply, bool transpose)
{
	// repository present ?
	if (!m_ShaderModificationRepository)
		m_ShaderModificationRepository = new ShaderModificationRepository(spShaderViewAdjustments);

	// get unique id and add rule
	return m_ShaderModificationRepository->ModifyRule(constantName, operationToApply, transpose);
}

/**
* Delete a default rule in a shader modification repository.
* @return True if rule was deleted, false if rule not present.
***/
bool GameHandler::DeleteRule(std::shared_ptr<ViewAdjustment> spShaderViewAdjustments, std::string constantName)
{
	// repository present ?
	if (!m_ShaderModificationRepository)
		m_ShaderModificationRepository = new ShaderModificationRepository(spShaderViewAdjustments);

	// get unique id and add rule
	return m_ShaderModificationRepository->DeleteRule(constantName);
}

/**
* Returns the shader modification repository for the handled game.
***/
ShaderModificationRepository* GameHandler::GetShaderModificationRepository()
{
	return m_ShaderModificationRepository;
}