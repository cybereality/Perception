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
		Entry_Float
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
	* Exit menu event.
	**/
	bool bOnExit;
	/**
	* Selection events.
	* Up, Down, Left, Right, Accept.
	***/
	bool bOnUp, bOnDown, bOnLeft, bOnRight, bOnAccept;
};