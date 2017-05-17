/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Stereo Presenter - Vireio Perception Stereo Configuration Handler
Copyright (C) 2015 Denis Reischl

File <VireioStereoPresenter.h> and
Class <VireioStereoPresenter> :
Copyright (C) 2015 Denis Reischl

The stub class <AQU_Nodus> is the only public class from the Aquilinus
repository and permitted to be used for open source plugins of any kind.
Read the Aquilinus documentation for further information.

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider

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

#include"AQU_Nodus.h"
#include"Resources.h"
#include"..\..\..\..\DxProxy\DxProxy\ViewAdjustment.h"

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
#include"..\..\..\Include\VireioMenu.h"

#define NUMBER_OF_COMMANDERS                           0
#define NUMBER_OF_DECOMMANDERS                         35

#pragma region registry helpers
/**
* Small debug helper quickly imported from Vireio v3.
***/
void debugf(const char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	char buf[8192];
	vsnprintf_s(buf, 8192, fmt, args);
	va_end(args);
	OutputDebugStringA(buf);
}

/**
* Reads a string from registry.
* @param hKey Registry key handle.
* @param szValueName Name of the value to be read.
* @param lpszResult The string result.
***/
HRESULT RegGetString(HKEY hKey, LPCSTR szValueName, LPSTR * lpszResult) {

	// Given a HKEY and value name returns a string from the registry.
	// Upon successful return the string should be freed using free()
	// eg. RegGetString(hKey, TEXT("my value"), &szString);

	DWORD dwType = 0, dwDataSize = 0, dwBufSize = 0;
	LONG lResult;

	// Incase we fail set the return string to null...
	if (lpszResult != NULL) *lpszResult = NULL;

	// Check input parameters...
	if (hKey == NULL || lpszResult == NULL) return E_INVALIDARG;

	// Get the length of the string in bytes (placed in dwDataSize)...
	lResult = RegQueryValueExA(hKey, szValueName, 0, &dwType, NULL, &dwDataSize);

	// Check result and make sure the registry value is a string(REG_SZ)...
	if (lResult != ERROR_SUCCESS) return HRESULT_FROM_WIN32(lResult);
	else if (dwType != REG_SZ)    return DISP_E_TYPEMISMATCH;

	// Allocate memory for string - We add space for a null terminating character...
	dwBufSize = dwDataSize + (1 * sizeof(CHAR));
	*lpszResult = (LPSTR)malloc(dwBufSize);

	if (*lpszResult == NULL) return E_OUTOFMEMORY;

	// Now get the actual string from the registry...
	lResult = RegQueryValueExA(hKey, szValueName, 0, &dwType, (LPBYTE)*lpszResult, &dwDataSize);

	// Check result and type again.
	// If we fail here we must free the memory we allocated...
	if (lResult != ERROR_SUCCESS) { free(*lpszResult); return HRESULT_FROM_WIN32(lResult); }
	else if (dwType != REG_SZ) { free(*lpszResult); return DISP_E_TYPEMISMATCH; }

	// We are not guaranteed a null terminated string from RegQueryValueEx.
	// Explicitly null terminate the returned string...
	(*lpszResult)[(dwBufSize / sizeof(CHAR)) - 1] = TEXT('\0');

	return NOERROR;
}

/**
* Reads a string from registry.
* @param hKey Registry key handle.
* @param szValueName Name of the value to be read.
* @param resultStr The string result.
***/
HRESULT RegGetString(HKEY hKey, LPCSTR szValueName, std::string &resultStr)
{
	char *cstr = NULL;
	HRESULT success = RegGetString(hKey, szValueName, &cstr);
	if (cstr != NULL)
	{
		resultStr = cstr;
		free(cstr);
	}
	else
	{
		resultStr = "";
	}
	return success;
}
#pragma endregion

/**
* Returns the name of the Vireio Perception base directory read from registry.
* Saved to registry by InitConfig() in Main.cpp.
* Original method found in Vireio v3 class ProxyHelper.
***/
std::string GetBaseDir()
{
	static char* baseDirCache = nullptr;

	if (baseDirCache != nullptr)
	{
		return baseDirCache;
	}

	HKEY hKey;
	LPCTSTR sk = TEXT("SOFTWARE\\Vireio\\Perception");

	LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_QUERY_VALUE, &hKey);

	if (openRes != ERROR_SUCCESS)
	{
		OutputDebugString(L"PxHelp: Error opening key.\n");
		return "";
	}

	static std::string baseDir = "";
	HRESULT hr = RegGetString(hKey, "BasePath", baseDir);
	if (FAILED(hr))
	{
		OutputDebugString(L"Error getting BasePath from registry.\n");
		return "";
	}
	else
	{
		debugf("%s\n", baseDir.c_str());
		baseDirCache = _strdup(baseDir.c_str());
	}

	return baseDir;
}

/**
* Node Commander Enumeration.
***/
enum STP_Decommanders
{
	LeftTexture11,
	RightTexture11,
	MenuTexture,
	VireioSubMenu00,
	VireioSubMenu01,
	VireioSubMenu02,
	VireioSubMenu03,
	VireioSubMenu04,
	VireioSubMenu05,
	VireioSubMenu06,
	VireioSubMenu07,
	VireioSubMenu08,
	VireioSubMenu09,
	VireioSubMenu10,
	VireioSubMenu11,
	VireioSubMenu12,
	VireioSubMenu13,
	VireioSubMenu14,
	VireioSubMenu15,
	VireioSubMenu16,
	VireioSubMenu17,
	VireioSubMenu18,
	VireioSubMenu19,
	VireioSubMenu20,
	VireioSubMenu21,
	VireioSubMenu22,
	VireioSubMenu23,
	VireioSubMenu24,
	VireioSubMenu25,
	VireioSubMenu26,
	VireioSubMenu27,
	VireioSubMenu28,
	VireioSubMenu29,
	VireioSubMenu30,
	VireioSubMenu31,
};

/**
* Available stereo output modes (only monitor modes here).
***/
enum VireioMonitorStereoModes
{
	Vireio_Mono = 0,
	Vireio_SideBySide = 1,
};

/**
* Possible menu events.
***/
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

/**
* Vireio Stereo Presenter Node Plugin (Direct3D 9).
* Vireio Perception Stereo Drawing Handler.
***/
class StereoPresenter : public AQU_Nodus
{
public:
	StereoPresenter();
	virtual ~StereoPresenter();

	/*** AQU_Nodus public methods ***/
	virtual const char*     GetNodeType();
	virtual UINT            GetNodeTypeId();
	virtual LPWSTR          GetCategory();
	virtual HBITMAP         GetLogo();
	virtual HBITMAP         GetControl();
	virtual DWORD           GetNodeWidth() { return 4 + 256 + 4; }
	virtual DWORD           GetNodeHeight() { return 128; }
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

	/**
	* Stereo Textures input. (DX11)
	***/
	ID3D11ShaderResourceView** m_ppcTexView11[2];
	/**
	* Menu texture view pointer
	***/
	ID3D11RenderTargetView** m_ppcTexViewMenu;
	/**
	* True if a stereo mode is selected.
	***/
	VireioMonitorStereoModes m_eStereoMode;
	/**
	* Hotkey switch.
	***/
	bool m_bHotkeySwitch;
	/**
	* Menu hotkey switch.
	***/
	bool m_bMenuHotkeySwitch;
	/**
	* True if Menu is shown.
	***/
	bool m_bMenu;
	/**
	* The back buffer render target view (DX11).
	***/
	ID3D11RenderTargetView* m_pcBackBufferView;
	/**
	* The 2D vertex shader.
	***/
	union
	{
		ID3D10VertexShader* m_pcVertexShader10;
		ID3D11VertexShader* m_pcVertexShader11;
	};
	/**
	* The 2D pixel shader.
	***/
	union
	{
		ID3D10PixelShader* m_pcPixelShader10;
		ID3D11PixelShader* m_pcPixelShader11;
	};
	/**
	* The 2D vertex layout.
	***/
	union
	{
		ID3D10InputLayout* m_pcVertexLayout10;
		ID3D11InputLayout* m_pcVertexLayout11;
	};
	/**
	* The 2D vertex buffer.
	***/
	union
	{
		ID3D10Buffer* m_pcVertexBuffer10;
		ID3D11Buffer* m_pcVertexBuffer11;
	};
	/**
	* The d3d11 sampler.
	***/
	ID3D11SamplerState* m_pcSampler11;
	/**
	* The constant buffer.
	***/
	union
	{
		ID3D10Buffer* m_pcConstantBufferDirect10;
		ID3D11Buffer* m_pcConstantBufferDirect11;
	};
	/**
	* The depth stencil DX11.
	***/
	union
	{
		ID3D10Texture2D* m_pcDSGeometry10;
		ID3D11Texture2D* m_pcDSGeometry11;
	};
	/**
	* The depth stencil view DX11.
	***/
	union
	{
		ID3D10DepthStencilView* m_pcDSVGeometry10;
		ID3D11DepthStencilView* m_pcDSVGeometry11;
	};
	/**
	* Constant buffer data structure.
	***/
	GeometryConstantBuffer m_sGeometryConstants;
	/**
	* Cinema mode pointer bool.
	***/
	bool *m_pbCinemaMode;
	/**
	* Menu basic font.
	***/
	VireioFont* m_pcFontSegeo128;
	/**
	* Font file name.
	* File must have extension ".spritefont", string must NOT have the extension attached.
	***/
	std::string m_strFontName;
	/**
	* The sub menu for the main menu.
	***/
	VireioSubMenu m_sMainMenu;
	/**
	* The sub menu for the presenter node.
	***/
	VireioSubMenu m_sSubMenu;
	/**
	* The sub menu pointers.
	***/
	VireioSubMenu* m_apsSubMenues[32];
	/**
	* Font selection value.
	***/
	UINT m_unFontSelection;
	/**
	* Menu control structure.
	***/
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
	/**
	* Possible menu events.
	***/
	BOOL m_abMenuEvents[VireioMenuEvent::NumberOfEvents];
};

/**
* Exported Constructor Method.
***/
extern "C" __declspec(dllexport) AQU_Nodus* AQU_Nodus_Create()
{
	StereoPresenter* pStereoPresenter = new StereoPresenter();
	return static_cast<AQU_Nodus*>(pStereoPresenter);
}
