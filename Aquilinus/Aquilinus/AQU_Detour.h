/********************************************************************
Vireio Perception : Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Aquilinus : Vireio Perception 3D Modification Studio 
Copyright © 2014 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 to v3.0.x 2014-2015 by Grant Bagwell, Simon Brown and Neil Schneider
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
#ifndef AQU_DETOUR_METHODS
#define AQU_DETOUR_METHODS

/**
* Little helper to override the VTable.
***/
inline void OverrideVTable(LPVOID lpAddress, UINT_PTR dwDetour)
{
	DWORD dwVirtualProtectBackup;
	if (VirtualProtect(lpAddress, 5, /*PAGE_READWRITE*/PAGE_EXECUTE_READWRITE, &dwVirtualProtectBackup) != NULL)
	{
		*(PUINT_PTR)lpAddress = dwDetour;
		VirtualProtect(lpAddress, 5, dwVirtualProtectBackup, &dwVirtualProtectBackup);
	}
#ifdef _DEBUG
	else OutputDebugString(L"Failed to override VTable ! ");
#endif
}

/**
* Little helper to override the VTable.
***/
inline void OverrideFullVTable(LPVOID lpAddress, LPVOID lpTable, DWORD dwSize)
{
	DWORD dwVirtualProtectBackup;
	if (VirtualProtect(lpAddress, dwSize, /*PAGE_READWRITE*/PAGE_EXECUTE_READWRITE, &dwVirtualProtectBackup) != NULL)
	{
		memcpy(lpAddress, lpTable, dwSize);
		VirtualProtect(lpAddress, dwSize, dwVirtualProtectBackup, &dwVirtualProtectBackup); 
	}
}

/**
* Main detour function. 
* @param src [in, out] Source function to be detoured.
* @param dst [in] New detour function.
* @returns Pointer to the old function.
***/
void *DetourFunc(BYTE *src, const BYTE *dst, const int len)
{
	DWORD dwback;

	// allocate a place in memory for the bytes we are going to overwrite in the original, plus the size of a jump 
	BYTE *jmp = (BYTE*)malloc(len+JMP32_SZ);

	// allow read & write access to the memory at the original function
	VirtualProtect(src, len, PAGE_READWRITE, &dwback);

	// copy the bytes of original + length to the allocated memory place
	memcpy(jmp, src, len);	

	// increment to the end of the copied bytes
	jmp += len;

	// insert a jump back to the original + length at the end of those intructions we just copied over
	jmp[0] = JMP;
	*(DWORD*)(jmp+1) = (DWORD)(src+len - jmp) - JMP32_SZ;

	// NOP out all the bytes at the original that we have saved to the memory allocated place
	memset(src, NOP, len);

	// write a jump at the original to the hooked function
	src[0] = JMP;
	*(DWORD*)(src+1) = (DWORD)(dst-src) - JMP32_SZ;

	// put back the old protection flags
	VirtualProtect(src, len, dwback, &dwback);

	// allow execute, read & write access to the allocated memory place
	VirtualProtect((void*)(jmp-len), 10, PAGE_EXECUTE_READWRITE, &dwback);

	// return a pointer to the start of the memory allocated place
	return (jmp-len);
}

#endif