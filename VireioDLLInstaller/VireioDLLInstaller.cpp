
#include "stdafx.h"
#include "windows.h"

#pragma comment(lib, "Shlwapi.lib")

#include <string>
#include <Shlobj.h>
#include <Shlwapi.h>
#include <vector>
#include <algorithm>

#include "ProxyHelper.h"

void SearchForFiles( const std::string& strSearchFileName, const std::string& strFilePath, const bool& bRecursive, 
	std::vector<std::string> &shortFileNames, std::vector<std::string> &paths)
{
    std::string strFoundFilePath;
    WIN32_FIND_DATA file;

    std::string strPathToSearch = strFilePath + "\\";

    HANDLE hFile = FindFirstFile((strPathToSearch + "*").c_str(), &file);
    if ( hFile != INVALID_HANDLE_VALUE )
    {
        do
        {
            std::string strTheNameOfTheFile = file.cFileName;

			if (strTheNameOfTheFile != "." && strTheNameOfTheFile != "..")
			{
				if ( file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY )
				{
					if( bRecursive )
					{
						SearchForFiles( strSearchFileName, strPathToSearch + strTheNameOfTheFile, bRecursive, shortFileNames, paths);
					}
				}
				else
				{
					if (strTheNameOfTheFile.find(strSearchFileName) != std::string::npos)
					{
						std::transform(strTheNameOfTheFile.begin(), strTheNameOfTheFile.end(), strTheNameOfTheFile.begin(), ::tolower);		
						shortFileNames.push_back(strTheNameOfTheFile);
						paths.push_back(strFilePath);
					}
				}
			}
        }
        while ( FindNextFile(hFile, &file) );

        FindClose( hFile );
    }
}

static int CALLBACK BrowseFolderCallback(
                  HWND hwnd, UINT uMsg, LPARAM lParam, LPARAM lpData)
{
    if (uMsg == BFFM_INITIALIZED) {
        LPCTSTR path = reinterpret_cast<LPCTSTR>(lpData);
        ::SendMessage(hwnd, BFFM_SETSELECTION, true, (LPARAM) path);
        ::SendMessage(hwnd, BFFM_SETEXPANDED, true, (LPARAM) path);
    }
    return 0;
}

std::string GetLastPath()
{
	HKEY hKey;
	LPCTSTR sk = TEXT("SOFTWARE\\Vireio\\Perception");

	LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_QUERY_VALUE , &hKey);
	if (openRes!=ERROR_SUCCESS) 
	{
		return "";
	}

	char *lastPath = NULL;
	HRESULT hr = ProxyHelper::RegGetString(hKey, TEXT("VireioDllInstallerPath"), &lastPath);
	if (FAILED(hr)) 
	{
		return "";
	}

	std::string path = lastPath;
	free(lastPath);

	RegCloseKey(hKey);

	return path;
}

void SaveLastPath(std::string path)
{
	HKEY hKey;
	LPCTSTR sk = TEXT("SOFTWARE\\Vireio\\Perception");

	LONG openRes = RegOpenKeyEx(HKEY_CURRENT_USER, sk, 0, KEY_ALL_ACCESS , &hKey);

	if (openRes==ERROR_SUCCESS) {
		OutputDebugString("Hx // Success opening key.\n");
	} else {
		OutputDebugString("Hx // Error opening key.\n");
	}

	LPCTSTR value = TEXT("VireioDllInstallerPath");

	LONG setRes = RegSetValueEx(hKey, value, 0, REG_SZ, (LPBYTE)path.c_str(), path.length()+1);

	if (setRes == ERROR_SUCCESS) {
		OutputDebugString("Hx // Success writing to Registry.\n");
	} else {
		OutputDebugString("Hx // Error writing to Registry.\n");
	}

	LONG closeOut = RegCloseKey(hKey);

	if (closeOut == ERROR_SUCCESS) {
		OutputDebugString("Hx // Success closing key.\n");
	} else {
		OutputDebugString("Hx // Error closing key.\n");
	}
}

int _tmain(int argc, _TCHAR* argv[])
{
	_tprintf ( _T(" Vireio DLL link Install/Uninstall Utility\r\n"));
	_tprintf ( _T(" -------------------------------------\r\n"));
	_tprintf ( _T("\r\n"));
	_tprintf ( _T("NOTE: THIS TOOL IS ONLY REQUIRED IF VIREIO DOES NOT INJECT WITH THE\r\n"));
	_tprintf ( _T("PERCEPTION APP RUNNING WHEN YOU START THE GAME\r\n"));
	_tprintf ( _T("\r\n"));
	std::string instructions;
	instructions += "This will create symbolic links to the Vireio DLLs in the target\r\n";
	instructions += "folder rather than physically copying them.\r\n";
	instructions += "To uninstall the symbolic links just run this application again\r\n";
	instructions += "and select the game folder and the symbolic links will be removed\r\n";
	instructions += "This means if you upgrade Vireio and put it in the same location as the\r\n";
	instructions += "previous version there is no need to re-run this installation tool\r\n";
	instructions += "This application needs to run as Administrator to create symlinks\r\n";
	instructions += "\r\n";
	instructions += "** Please select target game root folder in selection dialog **\r\n";
	_tprintf ( instructions.c_str());

	std::string rootFolder = GetLastPath();
	if (rootFolder.length() == 0)
	{
		std::string localDriveList;
		DWORD dw = GetLogicalDrives();
		char d[] = {'A', 0};
		while (dw)
		{
			if (dw & 1)
			{
				std::string drive = std::string(d) + ":\\";
				if (GetDriveType(drive.c_str()) == DRIVE_FIXED)
					localDriveList += d;
			}
			dw = dw >> 1;
			d[0]++;
		}

		std::vector<std::string> steamLocations;
		steamLocations.push_back(":\\Program Files (x86)\\Steam\\SteamApps\\common");
		steamLocations.push_back(":\\Program Files\\Steam\\SteamApps\\common");
		steamLocations.push_back(":\\Steam\\SteamApps\\common");

		bool foundPath = false;
		std::string steamLocation;
		for (int i = 0; (i < localDriveList.length() && !foundPath); i++)
		{
			//Attempt to find the right root folder
			int location = 0;
			while (location < (int)steamLocations.size())
			{
				std::string path = localDriveList[i] + steamLocations[location];
				if (PathFileExists(path.c_str()))
				{
					foundPath = true;
					steamLocation = path;
					break;
				}
				location++;
			}
		}

		if (!foundPath)
		{
			//Fail, just go to user's documents folder
			PWSTR pszPath = NULL;
			SHGetKnownFolderPath(FOLDERID_Documents, 0, NULL, &pszPath);
			char path[1024];
			size_t c  = 0;
			wcstombs_s(&c, path, pszPath, 1024);
			rootFolder = path;
		}
		else
			rootFolder =  steamLocation;
	}


	BROWSEINFO binf = { 0 };

	//Set this to the default location
	char folder[1024];
	memset(folder, 0, 1024);
	memcpy(folder, rootFolder.c_str(), rootFolder.length() + 1);
	binf.lParam = reinterpret_cast<LPARAM>(folder);
    binf.lpszTitle = _T("Pick a target game folder:");
	binf.lpfn = BrowseFolderCallback;
	binf.ulFlags = BIF_RETURNONLYFSDIRS;

    LPITEMIDLIST pidl = SHBrowseForFolder ( &binf );
    if ( pidl != 0 )
    {
        // get the name of the folder
        TCHAR path[MAX_PATH];
        if ( SHGetPathFromIDList ( pidl, path ) )
        {
            _tprintf ( _T("Selected Folder: %s\n"), path );
        }

        // free memory used
        CoTaskMemFree(pidl);

		//Now save the selected folder for the next time we open
		SaveLastPath(path);

		std::string msg = std::string("Scanning: ") + path + "  for supported game executables...\r\n";
		_tprintf ( msg.c_str() );

		std::vector<std::string> exefiles;
		std::vector<std::string> exepaths;
		SearchForFiles(".exe", path, true, exefiles, exepaths);

		//Build compatible list from xml
		std::vector<std::string> g_list;
		ProxyHelper helper;
		helper.GetProfileGameExes(g_list);

		std::string gamePath;
		std::string gameExe;
		for (int i = 0; i < (int)exefiles.size(); ++i)
		{
			std::vector<std::string>::iterator iter = std::find(g_list.begin(), g_list.end(), exefiles[i]);
			if (iter != g_list.end())
			{
				//Found one in our collection
				gameExe = exefiles[i];
				gamePath = exepaths[i];
				break;
			}
		}

		if (gameExe.length() == 0)
		{
			//Invalid folder, no supported games contained within
			MessageBox(NULL, ("No supported games found in folder:\r\n\r\n"+std::string(path)).c_str(), "", MB_OK|MB_ICONINFORMATION);
		}
		else
		{
			_tprintf ( ("Found: " + gameExe + "\r\n").c_str() );
			_tprintf ( ("In folder: " + gamePath + "\r\n").c_str() );

			//Now check to see if the symlinks exist in that folder
			std::string filename = gamePath;
			filename += "\\d3d9.dll";

			OFSTRUCT of = {0};
			of.cBytes = sizeof(OFSTRUCT);
			int file = (int)OpenFile(filename.c_str(), &of, OF_EXIST);
			if (file == 1)
			{
				int result = MessageBox(NULL, ("Are you sure you wish to remove Vireio DLLs from the following location:\r\n\r\n"+std::string(gamePath)).c_str(), "Uninstall DLLs", MB_YESNO|MB_ICONEXCLAMATION);
				if (result == IDYES)
				{
					std::string command = helper.is64bit(gameExe.c_str()) ? "call RevokeVireioSymlinks64bit.cmd \"" : "call RevokeVireioSymlinks.cmd \"";
					command += gamePath;
					command += "\"";
					system(command.c_str());

					//Now check for lack of presence of DLL
					int file = (int)OpenFile(filename.c_str(), &of, OF_EXIST);
					if (file == -1)
						MessageBox(NULL, "Uninstall Complete", "", MB_OK|MB_ICONINFORMATION);
					else
						MessageBox(NULL, "Uninstall Failed - Please try again and run this application as Administrator", "", MB_OK|MB_ICONINFORMATION);
				}
			}
			else if (file == -1)
			{
				int result = MessageBox(NULL, ("Are you sure you wish to Install Vireio DLLs to the following location:\r\n\r\n"+std::string(gamePath)).c_str(), "Install DLLs", MB_YESNO|MB_ICONEXCLAMATION);
				if (result == IDYES)
				{
					//handling for 32 or 64 bit
					std::string command = helper.is64bit(gameExe.c_str()) ? "call DeployVireioSymlinks64bit.cmd \"" : "call DeployVireioSymlinks.cmd \"";
					command += gamePath;
					command += "\"";
					system(command.c_str());

					//Now check for presence of DLL
					int file = (int)OpenFile(filename.c_str(), &of, OF_EXIST);
					if (file == 1)
						MessageBox(NULL, "Install Complete", "", MB_OK|MB_ICONINFORMATION);
					else
						MessageBox(NULL, "Install Failed - Please try again and run this application as Administrator", "", MB_OK|MB_ICONINFORMATION);
				}
			}
			else
			{
				MessageBox(NULL, "Error: Could not open folder, please try again", "ERROR", MB_OK|MB_ICONEXCLAMATION);
				return -1;
			}
		}
    }

	return 0;
}

