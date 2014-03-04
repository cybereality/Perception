#include "Injector.h"
#include <windows.h>
#include <psapi.h>

#ifndef UNICODE  
typedef std::string String; 
#else
typedef std::wstring String; 
#endif


namespace
{
	HHOOK hookHandle = NULL;

	HINSTANCE thisDll = NULL;

	const unsigned int MAX_DLL_NAME_LENGTH = 60;
	String GetProcessBaseName(DWORD pid, HANDLE hProcess = NULL)
	{
		if (NULL == hProcess)
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

		if (NULL == hProcess)
			return TEXT("");

		TCHAR ProcessName[MAX_PATH];
		DWORD bufferSize = MAX_PATH;
		QueryFullProcessImageName(hProcess, 0, ProcessName, &bufferSize);

		String FinalName = String(ProcessName);
		size_t index = FinalName.find_last_of('\\');
		if (std::string::npos != index)
			FinalName = FinalName.substr(index + 1);
		
		return FinalName;
	}

	DWORD GetProcessIDByBaseName(std::string processName)
	{
		DWORD pids[1024];
		DWORD pidsListSize;

		if (!EnumProcesses(pids, sizeof(pids), &pidsListSize))
			return -1;

		DWORD numOfProcesses = pidsListSize / sizeof(DWORD);
		for (unsigned int i = 0; i < numOfProcesses; ++i)
		{
			if (processName == GetProcessBaseName(pids[i]))
				return pids[i];
		}
		return -1;
	}


	bool DoesProcessDependOnModule(DWORD pid, const String moduleName, HANDLE hProcess = NULL)
	{
		if (NULL == hProcess)
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pid);

		if (NULL == hProcess)
			return false;

		HMODULE hMods[1024];
		DWORD bytesNeeded;
		bool doesNotDependOnModule = true;

		if (EnumProcessModules(hProcess, hMods, sizeof(hMods), &bytesNeeded))
		{	
			for (unsigned int i = 0; doesNotDependOnModule && i < (bytesNeeded / sizeof(HMODULE)); ++i)
			{
				TCHAR curModName[MAX_DLL_NAME_LENGTH]; 
				if (GetModuleBaseName(hProcess, hMods[i], curModName, MAX_DLL_NAME_LENGTH))
				{
					doesNotDependOnModule &= 
						(String::npos == String(curModName).find(moduleName));
				}
			}
		}
		CloseHandle(hProcess);

		return doesNotDependOnModule;
	}
	
	LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) 
	{
		return CallNextHookEx(NULL, nCode, wParam, lParam); 
	}

	BOOL APIENTRY DllMain( HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved )
	{
		return TRUE;
	}
}

__declspec(dllexport) bool Injector::InstallHook()
{
	hookHandle = SetWindowsHookEx(WH_CBT, HookProc, thisDll, 0);
	return hookHandle != NULL;
}

__declspec(dllexport) bool Injector::RemoveHook()
{
	if (hookHandle == NULL)
	{
		SetLastError(0x00000015);	
		return false;
	}
	return (UnhookWindowsHookEx(hookHandle) != 0);
}

