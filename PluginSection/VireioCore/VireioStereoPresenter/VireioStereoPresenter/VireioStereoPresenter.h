/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Presenter - Vireio Perception Stereo Configuration Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoPresenter.h> and
Class <VireioStereoPresenter> :
Copyright (C) 2015 Denis Reischl



Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 by Denis Reischl, Grant Bagwell, Simon Brown and Neil Schneider

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

#include<stdio.h>
#include<memory>
#include<sstream>

#include"..//..//..//..//Aquilinus//Aquilinus//AQU_Nodus.h"
#include"Resources.h"

#include<Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

#include<XInput.h>
#pragma comment(lib, "Xinput9_1_0.lib")

#include <DXGI.h>
#pragma comment(lib, "DXGI.lib")

#include <d3d11_1.h>
#pragma comment(lib, "d3d11.lib")

#include <d3d11.h>
#pragma comment(lib, "d3d11.lib")

#include <D3DX11tex.h>
#pragma comment(lib, "D3DX11.lib")

#include <d3d10_1.h>
#pragma comment(lib, "d3d10_1.lib")

#include <d3d10.h>
#pragma comment(lib, "d3d10.lib")

#include <d3dx10.h>
#pragma comment(lib, "d3dx10.lib")

#include <d3d9.h>
#pragma comment(lib, "d3d9.lib")

#include <d3dx9.h>
#pragma comment(lib, "d3dx9.lib")

#include"..\..\..\Include\Vireio_GUIDs.h"
#include"..\..\..\Include\Vireio_DX11Basics.h"
#include"..\..\..\Include\Vireio_Node_Plugtypes.h"

#define NUMBER_OF_COMMANDERS                           0
#define NUMBER_OF_DECOMMANDERS                         1

#define DEBUG_F(fmt, ...) { va_list args; va_start(args, fmt); char buf[8192]; vsnprintf_s(buf, 8192, fmt, args); va_end(args); OutputDebugStringA(buf); }

/// <summary>Node decommander enumeration</summary>
enum STP_Decommanders
{
	Cinema
};

/// <summary>Available stereo output modes (only monitor modes here) </summary>
enum VireioMonitorStereoModes
{
	Vireio_Mono = 0,
	Vireio_SideBySide = 1,
};

/// <summary>Possible menu events</summary>
enum VireioMenuEvent
{
	OnLeft,
	OnRight,
	OnUp,
	OnDown,
	OnAccept,
	OnExit,
	NumberOfEvents = 6
};

/// <summary>
/// Vireio Stereo Presenter Node Plugin(Direct3D 9).
/// Vireio Perception Stereo Drawing Handler.
/// </summary>
class StereoPresenter : public AQU_Nodus
{
public:
	StereoPresenter(ImGuiContext* sCtx);
	virtual ~StereoPresenter();

	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual ImVec2          GetNodeSize() { return ImVec2((float)g_uGlobalNodeWidth, (float)128); }
	virtual DWORD           GetCommandersNumber() { return NUMBER_OF_COMMANDERS; }
	virtual DWORD           GetDecommandersNumber() { return NUMBER_OF_DECOMMANDERS; }
	virtual LPWSTR          GetCommanderName(DWORD dwCommanderIndex);
	virtual LPWSTR          GetDecommanderName(DWORD dwDecommanderIndex);
	virtual DWORD           GetCommanderType(DWORD dwCommanderIndex);
	virtual DWORD           GetDecommanderType(DWORD dwDecommanderIndex);
	virtual void*           GetOutputPointer(DWORD dwCommanderIndex);
	virtual void            SetInputPointer(DWORD dwDecommanderIndex, void* pData);
	virtual bool            SupportsD3DMethod(int nD3DVersion, int nD3DInterface, int nD3DMethod);
	virtual void*           Provoke(void* pThis, int eD3D, int eD3DInterface, int eD3DMethod, DWORD dwNumberConnected, int& nProvokerIndex);

private:
	/*** Stereo Presenter private methods ***/
	void                    RenderMenu(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext);
	void                    RenderSubMenu(ID3D11Device* pcDevice, ID3D11DeviceContext* pcContext, VireioSubMenu* psSubMenu);
	void                    UpdateMenu(float fGlobalTime);
	void                    UpdateSubMenu(VireioSubMenu* psSubMenu, float fGlobalTime);

	/// <summary>
	/// Stereo data input pointer
	/// </summary>
	StereoData* m_psStereoData;
	/// <summary>
	/// True if a stereo mode is selected.
	/// </summary>
	VireioMonitorStereoModes m_eStereoMode;
	/// <summary>
	/// Hotkey switch.
	/// </summary>
	bool m_bHotkeySwitch;
	/// <summary>
	/// Menu hotkey switch.
	/// </summary>
	bool m_bMenuHotkeySwitch;
	/// <summary>
	/// True if Menu is shown.
	/// </summary>
	bool m_bMenu;
	/// <summary>
	/// The back buffer render target view (DX11).
	/// </summary>
	ID3D11RenderTargetView* m_pcBackBufferView;
	/// <summary>
	/// The 2D vertex shader.
	/// </summary>
	union
	{
		ID3D10VertexShader* m_pcVertexShader10;
		ID3D11VertexShader* m_pcVertexShader11;
	};
	/// <summary>
	/// The 2D pixel shader.
	/// </summary>
	union
	{
		ID3D10PixelShader* m_pcPixelShader10;
		ID3D11PixelShader* m_pcPixelShader11;
	};
	/// <summary>
	/// The 2D vertex layout.
	/// </summary>
	union
	{
		ID3D10InputLayout* m_pcVertexLayout10;
		ID3D11InputLayout* m_pcVertexLayout11;
	};
	/// <summary>
	/// The 2D vertex buffer.
	/// </summary>
	union
	{
		ID3D10Buffer* m_pcVertexBuffer10;
		ID3D11Buffer* m_pcVertexBuffer11;
	};
	/// <summary>
	/// The d3d11 sampler.
	/// </summary>
	ID3D11SamplerState* m_pcSampler11;
	/// <summary>
	/// The constant buffer.
	/// </summary>
	union
	{
		ID3D10Buffer* m_pcConstantBufferDirect10;
		ID3D11Buffer* m_pcConstantBufferDirect11;
	};
	/// <summary>
	/// The depth stencil DX11.
	/// </summary>
	union
	{
		ID3D10Texture2D* m_pcDSGeometry10;
		ID3D11Texture2D* m_pcDSGeometry11;
	};
	/// <summary>
	/// The depth stencil view DX11.
	/// </summary>
	union
	{
		ID3D10DepthStencilView* m_pcDSVGeometry10;
		ID3D11DepthStencilView* m_pcDSVGeometry11;
	};
	/// <summary>
	/// Constant buffer data structure.
	/// </summary>
	GeometryConstantBuffer m_sGeometryConstants;
	/// <summary>
	/// Cinema mode pointer bool.
	/// </summary>
	bool *m_pbCinemaMode;
	/// <summary>
	/// Menu basic font.
	/// </summary>
	VireioFont* m_pcFontSegeo128;
	/// <summary>
	/// Font file name.
	/// File must have extension ".spritefont", string must NOT have the extension attached.
	/// </summary>
	std::string m_strFontName;
	/// <summary>
	/// The sub menu for the main menu.
	/// </summary>
	VireioSubMenu m_sMainMenu;
	/// <summary>
	/// The sub menu for the presenter node.
	/// </summary>
	VireioSubMenu m_sSubMenu;
	/// <summary>
	/// The sub menu pointer vector.
	/// Will be resized to 32 (max sub menues)
	/// </summary>
	std::vector<VireioSubMenu*> m_asSubMenu;
	/// <summary>
	/// Font selection value.
	/// </summary>
	UINT m_unFontSelection;
	/// <summary>
	/// Menu control structure.
	/// </summary>
	struct MenuControl
	{
		INT nMenuIx;            /**< Current sub menu index, -1 for main menu. ***/
		UINT unSelection;       /**< Current selection index. **/
		UINT unSelectionFormer; /**< Former selection. **/
		enum SelectionMovement
		{
			Standing = 0,
			MovesUp = 1,
			MovesDown = 2,
			Accepted = 3,
			Exit = 4,
			TriggerLeft = 5,
			TriggerRight = 6
		} eSelectionMovement;   /**< The movement of the selection. **/
		float fActionTime;      /**< The (idle) time for the current action. **/
		float fActionStartTime; /**< The start time of the action. **/
		float fYOrigin;         /**< The y origin for the menu to be drawn. **/
	} m_sMenuControl;
	/// <summary>
	/// Possible menu events.
	/// </summary>
	BOOL m_abMenuEvents[VireioMenuEvent::NumberOfEvents];
};

/// <summary>
/// Exported Constructor Method.
/// </summary>
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create(ImGuiContext * sCtx)
{
	StereoPresenter* pStereoPresenter = new StereoPresenter(sCtx);
	return static_cast<AQU_Nodus*>(pStereoPresenter);
}
