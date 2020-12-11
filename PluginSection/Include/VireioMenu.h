/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <VireioMenu.h> :
Copyright (C) 2016 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
v4.0.x 2015 onwards
by Denis Reischl, Grant Bagwell, Simon Brown, Samuel Austin and Neil Schneider

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

#include<string>
#include<vector>
#include"..\..\Aquilinus\Aquilinus\AQU_GlobalTypes.h"

/**
* Vireio menu entry structure.
***/
struct VireioMenuEntry
{
	/**
	* True if this menu entry got changed.
	***/
	bool bOnChanged;
	/**
	* True if this menu entry is active.
	***/
	bool bIsActive;
	/**
	* True if a second value is available in cinema mode.
	***/
	bool bSecondCinemaValue;
	/**
	* True if this is a switch value cinema/fullscreen mode.
	***/
	bool bIsFullscreenswitch;
	/**
	* Type of this entry.
	***/
	enum EntryType
	{
		Entry_Bool,
		Entry_Int,
		Entry_UInt,
		Entry_Float,
		Entry
	} eType;
	/**
	* Minimum value.
	***/
	union
	{
		bool bMinimum;
		int nMinimum;
		unsigned int unMinimum;
		float fMinimum;
	};
	/**
	* Maximum value.
	***/
	union
	{
		bool bMaximum;
		int nMaximum;
		unsigned int unMaximum;
		float fMaximum;
	};
	/**
	* Actual value pointer.
	* The destination value is either the actual or the secondary value.
	***/
	union
	{
		bool *pbValue;
		int *pnValue;
		unsigned int *punValue;
		float *pfValue;
	};
	/**
	* Actual value.
	***/
	union
	{
		bool bValue;
		int nValue;
		unsigned int unValue;
		float fValue;
	};
	/**
	* Secondary value.
	***/
	union
	{
		bool bValueSec;
		int nValueSec;
		unsigned int unValueSec;
		float fValueSec;
	};
	/**
	* Menu change size.
	* +/- Amount changed in the menu.
	***/
	union
	{
		bool bChangeSize;
		int nChangeSize;
		unsigned int unChangeSize;
		float fChangeSize;
	};
	/**
	* True if value enumeration strings are present.
	***/
	bool bValueEnumeration;
	/**
	* Value enumeration.
	***/
	std::vector<std::string> astrValueEnumeration;
	/**
	* Entry string.
	***/
	std::string strEntry;
};

/**
* A Vireio sub menu.
* Each node has its own sub menu.
***/
struct VireioSubMenu
{
	/**
	* True if menu is active.
	***/
	bool bIsActive;
	/**
	* True if any menu entry got changed.
	***/
	bool bOnChanged;
	/**
	* Sub menu string.
	***/
	std::string strSubMenu;
	/**
	* All entries.
	***/
	std::vector<VireioMenuEntry> asEntries;
	/**
	* Number of active entries.
	* Gets automatically updated each frame by stereo presenter node.
	***/
	UINT unActiveEntries;
	/**
	* Exit menu event.
	**/
	bool bOnExit;
	/**
	* Selection events.
	* Up, Down, Left, Right, Accept, Back.
	***/
	bool bOnUp, bOnDown, bOnLeft, bOnRight, bOnAccept, bOnBack;
	/**
	* Full Immersive Mode event.
	* True if cinema mode is bound to switch either to Full Immersive Mode or back to Cinema Mode.
	***/
	bool bOnFullImmersive;
	/**
	* True if hand poses are present.
	***/
	bool bHandPosesPresent;
	/**
	* True if the node requests hand poses.
	***/
	bool bHandPosesRequest;
	/*
	* The pose matrices for left/right eye (if bHandPosesPresent is true).
	**/
	D3DXMATRIX sPoseMatrix[2];
	/*
	* The position vectors for left / right eye (if bHandPosesPresent is true).
	**/
	D3DXVECTOR3 sPosition[2];
};

#pragma region helper
/// <summary>
/// Small helper to filter out inactive menu entries.
/// </summary>
/// <param name="psSubMenu">The menu pointer</param>
/// <param name="unPrimalSelection">Selection index to be found</param>
/// <returns>The actual index of the sub menu entry</returns>
inline UINT GetSelection(VireioSubMenu* psSubMenu, UINT unPrimalSelection)
{
	UINT unIxPrimal = unPrimalSelection;
	UINT unIx = 0;
	for (UINT unI = 0; unI < psSubMenu->asEntries.size(); unI++)
	{
		// menu error ?
		if (unIx >= (UINT)psSubMenu->asEntries.size())
		{
			OutputDebugString(L"[STP] Fatal menu structure error ! Empty menu ??");
			// m_sMenuControl.unSelection = 0;
			unIx = 0;
			break;
		}

		// set back index runner if inactive entry
		if (!psSubMenu->asEntries[unIx].bIsActive) { unIxPrimal++; }

		// caught selection ?
		if (unIx == unIxPrimal) break;

		// increment index
		unIx++;
	}
	return unIx;
}

/// <summary>
/// Provides the Vireio base directory through the Aquilinus configuration.
/// </summary>
/// <returns>Vireio Perception base directory</returns>
inline std::wstring GetBaseDir()
{
	// Aquilinus fields, cfg imported from AQU_GlobalTypes.h
	const TCHAR szMemoryPageName[] = TEXT("AquilinusCfg");
	HANDLE hConfigMapFile;
	AquilinusCfg* pAquilinusConfig;
	std::wstring acRet = std::wstring();

	// get the config map handle
	hConfigMapFile = OpenFileMapping(
		FILE_MAP_ALL_ACCESS,   // read/write access
		FALSE,                 // do not inherit the name
		szMemoryPageName);   // name of the Aquilinus config

	// return if failed
	if (hConfigMapFile == NULL)
	{
		OutputDebugString(TEXT("Aquilinus : Could not create file mapping object.\n"));
		return acRet;
	}

	// create map view
	pAquilinusConfig = (AquilinusCfg*)MapViewOfFile(hConfigMapFile, FILE_MAP_ALL_ACCESS, NULL, NULL, sizeof(AquilinusCfg));

	// return if failed
	if (pAquilinusConfig == NULL)
	{
		OutputDebugString(TEXT("Aquilinus : Could not map view of file.\n"));
		CloseHandle(hConfigMapFile);
		return acRet;
	}

	// get path and close memory file
	acRet = std::wstring(pAquilinusConfig->szAquilinusPath);
	UnmapViewOfFile((LPCVOID)pAquilinusConfig);
	CloseHandle(hConfigMapFile);

	return acRet;
}
#pragma endregion
