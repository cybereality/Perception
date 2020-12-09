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
#ifndef AQU_FILEMANAGER_CLASS
#define AQU_FILEMANAGER_CLASS

/**
* The joliet specification only allows filenames to be up to 64 Unicode characters in length.
* We use that as limit for the process list.
***/
#define MAX_JOLIET_FILENAME 64
/** 
* The entry size is MAX_JOLIET_FILENAME*sizeof(wchar_t).
***/
#define ENTRY_SIZE MAX_JOLIET_FILENAME*sizeof(wchar_t)
/**
* The size of one process entry is 3*MAX_JOLIET_FILENAME*sizeof(wchar_t).
* (name, window name, process name)
***/
#define PROCESS_ENTRY_SIZE 3*MAX_JOLIET_FILENAME*sizeof(wchar_t)
/**
* Size of reserved options (*sizeof(DWORD))
***/
#define OPTIONS_RESERVED 256


#include <Windows.h>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <vector>
#include "NOD_Basic.h"
#include "NOD_Plugin.h"

/**
* Aquilinus encryption modes.
* All supported encryption modes.
***/
enum AQU_EncryptionMode
{
	Caesar,
	MixUpPosition,
};

/**
* Aquilinus File Manager.
* Inherits all input and output operations including file or memory encryptions.
***/
class AQU_FileManager
{
public:
	AQU_FileManager(AquilinusCfg* psConfig);
	~AQU_FileManager();

	HRESULT LoadWorkingAreaBasics(LPWSTR szWorkspacePath, DWORD &dwProcessIndex, DWORD &dwSupportedInterfacesNumber, __int32* pnInterfaceInjectionTechnique, LPWSTR szPicturePath, BOOL &bPicture, unsigned __int32 &dwDetourTimeDelay, __int32 &nInjectionRepetition, bool bKeepProcessName);
	HRESULT LoadProfileBasics(LPCWSTR szProfilePath, AquilinusCfg* psConfig, DWORD &dwSupportedInterfacesNumber, BYTE* &paPictureData, DWORD &dwPictureSize);
	HRESULT LoadWorkingArea(LPWSTR szWorkspacePath, std::stringstream &sstrDataStream);
	HRESULT LoadProfile(LPWSTR szProfilePath, std::stringstream &sstrDataStream);
	HRESULT SaveWorkingArea(AquilinusCfg* psConfig, std::vector<NOD_Basic*>* ppaNodes, DWORD dwSupportedInterfacesNumber);
	HRESULT CompileProfile(AquilinusCfg* psConfig, std::vector<NOD_Basic*>* ppaNodes, DWORD dwSupportedInterfacesNumber);
	LPCWSTR GetAquilinusPath();
	HRESULT SetPluginPath(LPCWSTR szPath);
	LPCWSTR GetPluginPath();
	DWORD   GetProcessNumber();
	LPWSTR  GetName(DWORD dwIndex);
	LPWSTR  GetWindowName(DWORD dwIndex);
	LPWSTR  GetProcessName(DWORD dwIndex);
	unsigned __int32 GetHash(BYTE* pcData, unsigned __int32 dwSize);
	HRESULT AddProcess(LPCWSTR szName, LPCWSTR szWindow, LPCWSTR szProcess);
	HRESULT LoadProcessListCSV(AquilinusCfg* psConfig);
	HRESULT SaveProcessListCSV();
	HRESULT SaveGameListTXT();

private:
	/**
	* The Aquilinus node directory path.
	***/
	std::string m_szPluginPath;
	/**
	* The Aquilinus node directory path.
	***/
	wchar_t m_szPluginPathW[MAX_PATH]; 
	/**
	* The encrpyted process list.
	* Contains Name, Window Name, Process. Each string is a MAX_PATH wchar_t entry.
	* So, each game process is 3*MAX_JOLIET_FILENAME*sizeof(wchar_t).
	***/
	BYTE* m_pcProcesses;
	/**
	* The current size of the process list, in bytes.
	***/
	DWORD m_dwProcessListSize;
	/**
	* The handle to the save file map.
	* Stores the workspace/compiled file data saved by Inicio later.
	***/
	HANDLE m_hSaveMapFile;
};



#endif