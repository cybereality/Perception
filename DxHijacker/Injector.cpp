#define _WIN32_WINNT 0x0601

#include <windows.h>
#include <psapi.h>
#include "Injector.h"
#include "easyhook.h"
#include <sstream>

using namespace Injector;

namespace
{
	typedef NTSTATUS (__stdcall *RhInjectLibrary)(ULONG InTargetPID, ULONG InWakeUpTID, ULONG InInjectionOptions, WCHAR* InLibraryPath_x86, WCHAR* InLibraryPath_x64, PVOID InPassThruBuffer, ULONG InPassThruSize);

	const int BUFFER_SIZE = 1024;

	HHOOK hookHandle = NULL;

	HINSTANCE thisDll = NULL;

	const char ITEM_SEPARATOR = '^';

	const char PAIR_SEPARATOR = '|';

	const String MAPPED_MEM_PATH = "Global\\InjectionData";

	const unsigned int MAX_DLL_NAME_LENGTH = 60;

	std::vector<String> processNamesToInject;

	std::map<String, String> depsAndPathsForInjection;

	HANDLE mappedFileHandle;

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

	bool InitializeHookData()
	{
		OutputDebugString("Initializing hook data");
		if (mappedFileHandle == NULL)
		{
			OutputDebugString("Opening File");
			mappedFileHandle = OpenFileMapping(FILE_MAP_READ, false, MAPPED_MEM_PATH.c_str());
		}
		LPCTSTR pBuf;
		
		if (mappedFileHandle == NULL)
		{
			OutputDebugString("File did not open");
			return false;
		}
		pBuf = (LPCTSTR) MapViewOfFile(mappedFileHandle, FILE_MAP_READ, 0, 0, BUFFER_SIZE);
		if (pBuf == NULL)
		{
			OutputDebugString("Pbuf was null");
			return false;
		}
		// process name ITEM SEPERATOR process name \n
		// dependency PAIR SEPARATOR path ITEM SEPARATOR dependency...
		String mainElem, secondaryElem;
		bool collectingPair;
		for (int i = 0; i < sizeof(*pBuf) / sizeof(TCHAR); ++i)
		{
			if (pBuf[i] == PAIR_SEPARATOR)
			{
					collectingPair = true;
			}
			else if (pBuf[i] == ITEM_SEPARATOR) 
			{
				if (secondaryElem.empty())
				{
					processNamesToInject.emplace_back(mainElem); 
				}
				else
				{
					depsAndPathsForInjection.emplace(mainElem, secondaryElem);
				}
				collectingPair = false;
				mainElem = "";
				secondaryElem = "";
			}
			else if (collectingPair)
			{
				secondaryElem += pBuf[i];
			}
			else
			{
				mainElem += pBuf[i];
			}
		}
		std::ostringstream outputstr;
		outputstr << "buffer contents: " << pBuf;
		OutputDebugString(outputstr.str().c_str());
		UnmapViewOfFile(pBuf);
		return true;
	}

	extern "C" __declspec(dllexport) BOOL APIENTRY DllMain( HINSTANCE hModule, DWORD fdwReason, LPVOID lpReserved )
	{
		thisDll = hModule;
		OutputDebugString("IN dll main");
		if (processNamesToInject.empty() || depsAndPathsForInjection.empty())
		{
			InitializeHookData();
			std::ostringstream errStr;
			errStr << "ERROR: ";
			errStr << GetLastError() << "  ";
			for (String elem : processNamesToInject) errStr << elem << ", ";
			for (auto elem : depsAndPathsForInjection) errStr << elem.first << " : " << elem.second << ", ";
			OutputDebugString(errStr.str().c_str());
		}
		if (fdwReason == DLL_PROCESS_ATTACH)
		{
			OutputDebugString("Attaching to process");
			if (processNamesToInject.empty() || depsAndPathsForInjection.empty())
			{
				OutputDebugString("ERROR: no pids or deps");
				return true;
			}
			HMODULE easyHook = NULL;
			DWORD pid;
			for(String elem : processNamesToInject)
			{
				OutputDebugString("Getting Process ID");
				pid = GetProcessIDByBaseName(elem);
				if (pid)
					for (auto pairElem : depsAndPathsForInjection)
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

	void SaveInjectionData(String* mappedFileView)
	{
		OutputDebugString("Saving Injection Dat");
		if (processNamesToInject.empty() || depsAndPathsForInjection.empty())
		{
			OutputDebugString("List or map is empty");
		}
		String serializedData;
		for (String elem : processNamesToInject)
		{
			OutputDebugString("Inf foreach for pids");
			serializedData.append(elem + ITEM_SEPARATOR);
		}
		for (auto mapElem : depsAndPathsForInjection)
		{
			OutputDebugString("In for for map");
			serializedData.append(mapElem.first + PAIR_SEPARATOR + mapElem.second + ITEM_SEPARATOR);
		}
		std::ostringstream blah;
		blah << "blah = " << serializedData;
		OutputDebugString(blah.str().c_str());
		CopyMemory((PVOID)mappedFileView, serializedData.c_str(), sizeof(serializedData.c_str()));
	}
}
// I don't actually think this needs to be exported
__declspec(dllexport) LRESULT CALLBACK HookProc(int nCode, WPARAM wParam, LPARAM lParam) 
{
	OutputDebugString("hookproc");
	return CallNextHookEx(NULL, nCode, wParam, lParam); 
}

__declspec(dllexport) bool Injector::InstallHook(std::vector<String>& pprocessNamesToInject, std::map<String, String>& pdepsAndPathsForInjection)
{
	processNamesToInject = pprocessNamesToInject;
	depsAndPathsForInjection = pdepsAndPathsForInjection;
	OutputDebugString("Hook Installing");
	String* pBuf;
	mappedFileHandle = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, BUFFER_SIZE, MAPPED_MEM_PATH.c_str() );
	if (mappedFileHandle == NULL)
	{
		OutputDebugString("Mapped File Handle Null");
		return false;
	}
	
	pBuf = (String*) MapViewOfFile(mappedFileHandle, FILE_MAP_ALL_ACCESS, 0, 0, BUFFER_SIZE);
	if (pBuf == NULL)
	{
		OutputDebugString("pBuf Null");
		return false;
	}
	SaveInjectionData(pBuf);
	UnmapViewOfFile(pBuf);
	OutputDebugString("Saved and unmapped");
	std::ostringstream blah;
	blah << "ERROR STATUS: " << GetLastError();
	OutputDebugString(blah.str().c_str());


	if (thisDll == NULL)
		GetModuleHandleEx(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS,
							(LPCTSTR)"DxInjector.dll",
							&thisDll);
	hookHandle = SetWindowsHookEx(WH_CBT, HookProc, thisDll, 0);
	return hookHandle != NULL;
}

__declspec(dllexport) bool Injector::RemoveHook()
{
	CloseHandle(mappedFileHandle);
	if (hookHandle == NULL)
	{
		SetLastError(0x00000015);	
		return false;
	}

	OutputDebugString("Hook Removed");
	return (UnhookWindowsHookEx(hookHandle) != 0);
}

