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
bool GameHandler::Load(ProxyConfig& cfg, std::shared_ptr<ViewAdjustment> spShaderViewAdjustments)
{
	// Get rid of existing modification repository if there is one
	if (m_ShaderModificationRepository) {
		delete m_ShaderModificationRepository;
		m_ShaderModificationRepository = nullptr;
	}

	bool loadSuccess = true;
	intDuplicateCubeTexture = 0;
	intDuplicateTexture = 0;

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
bool GameHandler::Save(ProxyConfig& cfg, std::shared_ptr<ViewAdjustment> spShaderViewAdjustments)
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
	//Get duplicate render target entry from the game type
	if (m_gameType.length() == 8 && m_gameType[3] >= '0' && m_gameType[3] <= '2')
	{
		switch(m_gameType[3] - '0')
		{
		case 0:
			return false;
		case 1: 
			return true;
		case 2:
			if (isSwapChainBackBuffer) {
				return true;
			}
			return Width != Height;
		}
	}

	return true;
}

/**
* True if the by parameters described depth stencil surface is to be duplicated for the handled game.
* Currently, always returns true.
* @see D3DProxyDevice::CreateDepthStencilSurface()
***/
bool GameHandler::ShouldDuplicateDepthStencilSurface(UINT Width,UINT Height,D3DFORMAT Format,D3DMULTISAMPLE_TYPE MultiSample,DWORD MultisampleQuality,BOOL Discard)
{
	if (m_gameType.length() == 8 && m_gameType[3] >= '0' && m_gameType[3] <= '2')
	{
		switch(m_gameType[3] - '0')
		{
		case 0:
			return false;
		case 1: 
			return true;
		case 2:
			return Width != Height;
		}
	}

	return true;
}

/**
* True if the by parameters described texture is to be duplicated for the handled game.
* Currently, returns true if texture is a render target.
* @see D3DProxyDevice::CreateTexture()
***/
bool GameHandler::ShouldDuplicateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage, D3DFORMAT Format,D3DPOOL Pool)
{
	//Default behaviour
	int duplicateTexture = 2;
	if(intDuplicateTexture > 0)
	{
		duplicateTexture = intDuplicateTexture - 1;
	}
	else
	{
		if (m_gameType.length() != 8 || m_gameType[4] < '0' || m_gameType[4] > '3')
		{
			//Drop out to default
		}
		else
		{
			duplicateTexture = m_gameType[4] - '0';
		}
	}

	switch (duplicateTexture)
	{
	case 0:
		return false;
	case 1:
		return true;
	case 2:
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage);
	case 3:
		if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
			return true;
		return IS_RENDER_TARGET(Usage) && (Width != Height);
	}

	//Default - Won't ever get here
	if ((Usage & D3DUSAGE_DEPTHSTENCIL) == D3DUSAGE_DEPTHSTENCIL)
		return true;
	return IS_RENDER_TARGET(Usage);
}

/**
* True if the by parameters described cube texture is to be duplicated for the handled game.
* Currently, returns true if cube texture is a render target.
* @see D3DProxyDevice::CreateCubeTexture()
***/
bool GameHandler::ShouldDuplicateCubeTexture(UINT EdgeLength, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool)
{
	//Default behaviour
	int duplicateCubeTexture = 2;

	if(intDuplicateCubeTexture > 0)
	{
		duplicateCubeTexture = intDuplicateCubeTexture - 1;
	}
	else
	{
		if (m_gameType.length() != 8 || m_gameType[5] < '0' || m_gameType[5] > '2')
		{
			//Drop out to default
		}
		else
		{
			duplicateCubeTexture = m_gameType[5] - '0';
		}
	}

	switch(duplicateCubeTexture)
	{
	case 0:
		return false;
	case 1:
		return true;
	case 2:
		return IS_RENDER_TARGET(Usage);
	}

	return IS_RENDER_TARGET(Usage);
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