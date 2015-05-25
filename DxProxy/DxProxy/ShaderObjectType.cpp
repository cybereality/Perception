/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <D3D9ProxyVertexShader.h> and
Class <D3D9ProxyVertexShader> :
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

#include "ShaderObjectType.h"

std::string GetShaderObjectTypeStrng(ShaderObjectType objectType)
{
	std::string type;
	if (objectType == ShaderObjectTypeDoNotDraw)
		type = "DoNotDraw";
	else if (objectType == ShaderObjectTypeReticule)
		type = "Reticule";
	else if (objectType == ShaderObjectTypePlayer)
		type = "Player";
	else if (objectType == ShaderObjectTypeSky)
		type = "Sky";
	else if (objectType == ShaderObjectTypeShadows)
		type = "Shadows";
	else if (objectType == ShaderObjectTypeFog)
		type = "Fog";
	else if (objectType == ShaderObjectTypeClothes)
		type = "Clothes";

	return type;
}

ShaderObjectType GetShaderObjectTypeEnum(std::string objectType)
{
	ShaderObjectType type = ShaderObjectTypeUnknown;
	if (objectType == "DoNotDraw")
		type = ShaderObjectTypeDoNotDraw;
	else if (objectType == "Reticule")
		type = ShaderObjectTypeReticule;
	else if (objectType == "Player")
		type = ShaderObjectTypePlayer;
	else if (objectType == "Sky")
		type = ShaderObjectTypeSky;
	else if (objectType == "Shadows")
		type = ShaderObjectTypeShadows;
	else if (objectType == "Fog")
		type = ShaderObjectTypeFog;
	else if (objectType == "Clothes")
		type = ShaderObjectTypeClothes;
	return type;
}
