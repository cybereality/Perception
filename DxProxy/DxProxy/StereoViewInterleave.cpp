/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

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

#include "StereoViewInterleave.h"

/**
* Empty constructor.
***/ 
StereoViewInterleave::StereoViewInterleave(ProxyHelper::ProxyConfig& config):StereoView(config)
{
	OutputDebugString("Created SteroView Interleave\n");
}

/**
* Empty constructor.
***/ 
StereoViewInterleave::~StereoViewInterleave()
{
}

/**
* Empty render function, to be new written for the new architecture.
***/ 
void StereoViewInterleave::Draw(D3D9ProxySurface* stereoCapableSurface)
{
	// TODO modify to match new drawing method
	// old code:
	/*SaveState();
	SetState();

	m_pActualDevice->SetFVF(D3DFVF_TEXVERTEX);

	m_pActualDevice->SetTexture(0, leftTexture);
	m_pActualDevice->SetTexture(1, rightTexture);
	m_pActualDevice->SetStreamSource(0, screenVertexBuffer, 0, sizeof(TEXVERTEX));

	UINT iPass, cPasses;

	D3DXHANDLE viewWidth = viewEffect->GetParameterByName(0, "viewWidth");
	D3DXHANDLE viewHeight = viewEffect->GetParameterByName(0, "viewHeight");
	
	viewEffect->SetInt(viewWidth, viewport.Width);
	viewEffect->SetInt(viewHeight, viewport.Height);

	viewEffect->SetTechnique("ViewShader");
	viewEffect->Begin(&cPasses, 0);

	for(iPass = 0; iPass < cPasses; iPass++)
	{
		viewEffect->BeginPass(iPass);
		m_pActualDevice->DrawPrimitive(D3DPT_TRIANGLEFAN, 0, 2);
		viewEffect->EndPass();
	}
	viewEffect->End();

	RestoreState();*/
}