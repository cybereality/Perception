#define _WIN32_WINNT 0x0601

#include <windows.h>
#include <psapi.h>
#include "Injector.h"
#include "easyhook.h"

using namespace Injector;

namespace
{
	typedef NTSTATUS (__stdcall *RhInjectLibrary)(ULONG InTargetPID, ULONG InWakeUpTID, ULONG InInjectionOptions, WCHAR* InLibraryPath_x86, WCHAR* InLibraryPath_x64, PVOID InPassThruBuffer, ULONG InPassThruSize);

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
		return NULL;
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

	extern "C" __declspec(dllexport) BOOL APIENTRY DllMain( HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved )
	{
		thisDll = hModule;
		OutputDebugString("IN dll main");
		if (fdwReason == DLL_PROCESS_ATTACH)
		{
			OutputDebugString("Attaching to process");
			if (ProcessNamesToInject.empty() || DepsAndPathsForInjection.empty())
			{
				OutputDebugString("ERROR: no pids or deps");
				return true;
			}
			HMODULE easyHook = NULL;
			DWORD pid;
			for(std::string elem : ProcessNamesToInject)
			{
				OutputDebugString("Getting Process ID");
				pid = GetProcessIDByBaseName(elem);
				if (pid)
					for (auto pairElem : DepsAndPathsForInjection)
					{	
						OutputDebugString("Going through dependencies");
						if (DoesProcessDependOnModule(pid, pairElem.first) && !pairElem.first.empty())
						{
							OutputDebugString("Process depends on module");
							if (easyHook == NULL)
								easyHook = LoadLibrary(TEXT("EasyHook.dll"));
							if (easyHook)
							{
								OutputDebugString("Loading library");
								RhInjectLibrary func = (RhInjectLibrary)GetProcAddress(easyHook, TEXT("RhInjectLibrary"));
								if (func)
								{
									OutputDebugString("injecting");
									std::wstring wideString = std::wstring(pairElem.second.begin(), pairElem.second.end());
									(func)(pid, 0, EASYHOOK_INJECT_DEFAULT, &wideString[0], NULL, NULL, NULL);
								}
							}
						}
					}
			}
			if (easyHook)
				FreeLibrary(easyHook);
		}
		
	}
}
// I don't actually think this needs to be exported
__declspec(dllexport) LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
	OutputDebugString("hookproc");
	return CallNextHookEx(NULL, nCode, wParam, lParam); 
}

__declspec(dllexport) bool Injector::InstallHook()
{
	OutputDebugString("Hook Installed");
	if (thisDll == NULL)
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
							(LPCTSTR)"DxInjector.dll",
							&thisDll);
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

	OutputDebugString("Hook Removed");
	return (UnhookWindowsHookEx(hookHandle) != 0);
}

