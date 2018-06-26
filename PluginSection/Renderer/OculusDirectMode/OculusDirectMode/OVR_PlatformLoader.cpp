/************************************************************************************

Filename    :   OVR_PlatformLoader.cpp
Content     :   Platform DLL user library
Created     :   February 18, 2016
Copyright   :   Copyright 2016 Oculus VR, LLC All Rights reserved.

Licensed under the Oculus VR Rift SDK License Version 3.2 (the "License");
you may not use the Oculus VR Rift SDK except in compliance with the License,
which is provided at the time of installation or download, or which
otherwise accompanies this software in either electronic or hard copy form.

You may obtain a copy of the License at

http://www.oculusvr.com/licenses/LICENSE-3.2

Unless required by applicable law or agreed to in writing, the Oculus VR SDK
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.

************************************************************************************/


// Drop-in shim to load LibOVRPlatform
// This shim will perform signature verification on the platform library.
// If the library is properly signed and loaded, it will pass through
// the initialization call, and also pass through attempts to call
// a few other critical functions.

// The majority of this is code derived from LibOVR/Src/OVR_CAPIShim.c

#ifndef OVRPL_DISABLED

#if defined(_MSC_VER)
#pragma warning(push, 0)
#endif
#include <Windows.h>
#if defined(_MSC_VER)
#pragma warning(pop)
#endif

#include <stdio.h>
#include <stdlib.h>

#include "OVR_Platform.h"
#include "OVR_PlatformVersion.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4996) // 'getenv': This function or variable may be unsafe.
#endif


#if defined(_DEBUG)
// OVR_INTERNAL_CODE allows internal builds to ignore signature failure
// It should never be defined in any build that actually expects to load a signed dll
#define OVR_INTERNAL_CODE sizeof(NoCompile) == BuiltForInternalOnly
#endif



//-----------------------------------------------------------------------------------
// ***** FilePathCharType, ModuleHandleType, ModuleFunctionType
//
#define FilePathCharType       wchar_t                // #define instead of typedef because debuggers (VC++, XCode) don't recognize typedef'd types as a string type.
typedef HMODULE                ModuleHandleType;
typedef FARPROC                ModuleFunctionType;

#define ModuleHandleTypeNull   ((ModuleHandleType)NULL)
#define ModuleFunctionTypeNull ((ModuleFunctionType)NULL)


//-----------------------------------------------------------------------------------
// ***** OVRPL_DECLARE_IMPORT
//
// Creates typedef and pointer declaration for a function of a given signature.
// The typedef is <FunctionName>Type, and the pointer is <FunctionName>Ptr.
//
// Example usage:
//     int MultiplyValues(float x, float y);  // Assume this function exists in an external shared library. We don't actually need to redeclare it.
//     OVRPL_DECLARE_IMPORT(int, MultiplyValues, (float x, float y)) // This creates a local typedef and pointer for it.

#define OVRPL_DECLARE_IMPORT(ReturnValue, FunctionName, Arguments)  \
    typedef ReturnValue (OVRP_CDECL *FunctionName##Type)Arguments; \
    static FunctionName##Type FunctionName##PLPtr = NULL;

//-----------------------------------------------------------------------------------
// ***** OVRPL_GETFUNCTION
//
// Loads <FunctionName>Ptr from <library> if not already loaded.
// Assumes a variable named <FunctionName>Ptr of type <FunctionName>Type exists which is called <FunctionName> in LibOVRPlatform.
//
// Example usage:
//     OVRPL_GETFUNCTION(module, MultiplyValues)
//     int result = MultiplyValuesPtr(3.f, 4.f);

#define OVRPL_GETFUNCTION(l, f)        \
    if(!f##PLPtr)                      \
    {                                  \
        union                          \
        {                              \
            f##Type p1;                \
            ModuleFunctionType p2;     \
        } u;                           \
        u.p2 = GetProcAddress(l, #f);  \
        f##PLPtr = u.p1;               \
    }




//-----------------------------------------------------------------------------------
// ***** OVR_MAX_PATH
//
#if !defined(OVR_MAX_PATH)
#define OVR_MAX_PATH  _MAX_PATH
#endif



static size_t OVR_strlcpy(char* dest, const char* src, size_t destsize)
{
	const char* s = src;
	size_t      n = destsize;

	if (n && --n)
	{
		do{
			if ((*dest++ = *s++) == 0)
				break;
		} while (--n);
	}

	if (!n)
	{
		if (destsize)
			*dest = 0;
		while (*s++)
		{
		}
	}

	return (size_t)((s - src) - 1);
}


static size_t OVR_strlcat(char* dest, const char* src, size_t destsize)
{
	const size_t d = destsize ? strlen(dest) : 0;
	const size_t s = strlen(src);
	const size_t t = s + d;

	if (t < destsize)
		memcpy(dest + d, src, (s + 1) * sizeof(*src));
	else
	{
		if (destsize)
		{
			memcpy(dest + d, src, ((destsize - d) - 1) * sizeof(*src));
			dest[destsize - 1] = 0;
		}
	}

	return t;
}


#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4201)
#endif

#include <Softpub.h>
#include <Wincrypt.h>

#ifdef _MSC_VER
#pragma warning(pop)
#endif

// Expected certificates:
#define ExpectedNumCertificates 3
typedef struct CertificateEntry_t {
	const wchar_t* Issuer;
	const wchar_t* Subject;
} CertificateEntry;

static CertificateEntry NewCertificateChainPL[ExpectedNumCertificates] = {
	{ L"DigiCert SHA2 Assured ID Code Signing CA", L"Oculus VR, LLC" },
	{ L"DigiCert Assured ID Root CA", L"DigiCert SHA2 Assured ID Code Signing CA" },
	{ L"DigiCert Assured ID Root CA", L"DigiCert Assured ID Root CA" },
};

#define CertificateChainCount 1
static CertificateEntry* AllowedCertificateChainsPL[CertificateChainCount] = {
	NewCertificateChainPL
};

typedef WINCRYPT32API
DWORD
(WINAPI *PtrCertGetNameStringW)(
	PCCERT_CONTEXT pCertContext,
	DWORD dwType,
	DWORD dwFlags,
	void *pvTypePara,
	LPWSTR pszNameString,
	DWORD cchNameString
	);
typedef LONG(WINAPI *PtrWinVerifyTrust)(HWND hwnd, GUID *pgActionID,
	LPVOID pWVTData);
typedef CRYPT_PROVIDER_DATA * (WINAPI *PtrWTHelperProvDataFromStateData)(HANDLE hStateData);
typedef CRYPT_PROVIDER_SGNR * (WINAPI *PtrWTHelperGetProvSignerFromChain)(
	CRYPT_PROVIDER_DATA *pProvData, DWORD idxSigner, BOOL fCounterSigner, DWORD idxCounterSigner);

static PtrCertGetNameStringW m_PtrCertGetNameStringWPL = 0;
static PtrWinVerifyTrust m_PtrWinVerifyTrustPL = 0;
static PtrWTHelperProvDataFromStateData m_PtrWTHelperProvDataFromStateDataPL = 0;
static PtrWTHelperGetProvSignerFromChain m_PtrWTHelperGetProvSignerFromChainPL = 0;

static int ValidateCertificateContents(CertificateEntry* chain, CRYPT_PROVIDER_SGNR* cps)
{
	int certIndex;

	if (!cps ||
		!cps->pasCertChain ||
		cps->csCertChain != ExpectedNumCertificates)
	{
		return -1;
	}

	for (certIndex = 0; certIndex < ExpectedNumCertificates; ++certIndex)
	{
		CRYPT_PROVIDER_CERT* pCertData = &cps->pasCertChain[certIndex];
		wchar_t subjectStr[400] = { 0 };
		wchar_t issuerStr[400] = { 0 };

		if ((pCertData->fSelfSigned && !pCertData->fTrustedRoot) ||
			pCertData->fTestCert)
		{
			return -2;
		}

		m_PtrCertGetNameStringWPL(
			pCertData->pCert,
			CERT_NAME_ATTR_TYPE,
			0,
			szOID_COMMON_NAME,
			subjectStr,
			ARRAYSIZE(subjectStr));

		m_PtrCertGetNameStringWPL(
			pCertData->pCert,
			CERT_NAME_ATTR_TYPE,
			CERT_NAME_ISSUER_FLAG,
			0,
			issuerStr,
			ARRAYSIZE(issuerStr));

		if (wcscmp(subjectStr, chain[certIndex].Subject) != 0 ||
			wcscmp(issuerStr, chain[certIndex].Issuer) != 0)
		{
			return -3;
		}
	}

	return 0;
}

#define OVR_SIGNING_CONVERT_PTR(ftype, fptr, procaddr) { \
        union { ftype p1; ModuleFunctionType p2; } u; \
        u.p2 = procaddr; \
        fptr = u.p1; }

static HANDLE OVR_Win32_SignCheck(FilePathCharType* fullPath, ovrPlatformInitializeResult* outResult)
{
	HANDLE hFile = INVALID_HANDLE_VALUE;
	WINTRUST_FILE_INFO fileData;
	WINTRUST_DATA wintrustData;
	GUID actionGUID = WINTRUST_ACTION_GENERIC_VERIFY_V2;
	LONG resultStatus;
	int verified = 0;
	HMODULE libWinTrust = LoadLibraryW(L"wintrust");
	HMODULE libCrypt32 = LoadLibraryW(L"crypt32");
	if (libWinTrust == NULL || libCrypt32 == NULL)
	{
		*outResult = ovrPlatformInitialize_UnableToVerify;
		return INVALID_HANDLE_VALUE;
	}

	OVR_SIGNING_CONVERT_PTR(PtrCertGetNameStringW, m_PtrCertGetNameStringWPL, GetProcAddress(libCrypt32, "CertGetNameStringW"));
	OVR_SIGNING_CONVERT_PTR(PtrWinVerifyTrust, m_PtrWinVerifyTrustPL, GetProcAddress(libWinTrust, "WinVerifyTrust"));
	OVR_SIGNING_CONVERT_PTR(PtrWTHelperProvDataFromStateData, m_PtrWTHelperProvDataFromStateDataPL, GetProcAddress(libWinTrust, "WTHelperProvDataFromStateData"));
	OVR_SIGNING_CONVERT_PTR(PtrWTHelperGetProvSignerFromChain, m_PtrWTHelperGetProvSignerFromChainPL, GetProcAddress(libWinTrust, "WTHelperGetProvSignerFromChain"));

	if (m_PtrCertGetNameStringWPL == NULL || m_PtrWinVerifyTrustPL == NULL ||
		m_PtrWTHelperProvDataFromStateDataPL == NULL || m_PtrWTHelperGetProvSignerFromChainPL == NULL)
	{
		*outResult = ovrPlatformInitialize_UnableToVerify;
		return INVALID_HANDLE_VALUE;
	}

	if (!fullPath)
	{
		*outResult = ovrPlatformInitialize_FileInvalid;
		return INVALID_HANDLE_VALUE;
	}

	hFile = CreateFileW(fullPath, GENERIC_READ, FILE_SHARE_READ,
		0, OPEN_EXISTING, FILE_ATTRIBUTE_READONLY, 0);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		*outResult = ovrPlatformInitialize_FileInvalid;
		return INVALID_HANDLE_VALUE;
	}

	ZeroMemory(&fileData, sizeof(fileData));
	fileData.cbStruct = sizeof(fileData);
	fileData.pcwszFilePath = fullPath;
	fileData.hFile = hFile;

	ZeroMemory(&wintrustData, sizeof(wintrustData));
	wintrustData.cbStruct = sizeof(wintrustData);
	wintrustData.pFile = &fileData;
	wintrustData.dwUnionChoice = WTD_CHOICE_FILE; // Specify WINTRUST_FILE_INFO.
	wintrustData.dwUIChoice = WTD_UI_NONE; // Do not display any UI.
	wintrustData.dwUIContext = WTD_UICONTEXT_EXECUTE; // Hint that this is about app execution.
	wintrustData.fdwRevocationChecks = WTD_REVOKE_NONE;
	wintrustData.dwProvFlags = WTD_REVOCATION_CHECK_NONE;
	wintrustData.dwStateAction = WTD_STATEACTION_VERIFY;
	wintrustData.hWVTStateData = 0;

	resultStatus = m_PtrWinVerifyTrustPL(
		(HWND)INVALID_HANDLE_VALUE, // Do not display any UI.
		&actionGUID, // V2 verification
		&wintrustData);

	if (resultStatus == ERROR_SUCCESS &&
		wintrustData.hWVTStateData != 0 &&
		wintrustData.hWVTStateData != INVALID_HANDLE_VALUE)
	{
		CRYPT_PROVIDER_DATA* cpd = m_PtrWTHelperProvDataFromStateDataPL(wintrustData.hWVTStateData);
		if (cpd && cpd->csSigners == 1)
		{
			CRYPT_PROVIDER_SGNR* cps = m_PtrWTHelperGetProvSignerFromChainPL(cpd, 0, FALSE, 0);
			int chainIndex;
			for (chainIndex = 0; chainIndex < CertificateChainCount; ++chainIndex)
			{
				CertificateEntry* chain = AllowedCertificateChainsPL[chainIndex];
				if (0 == ValidateCertificateContents(chain, cps))
				{
					verified = 1;
					break;
				}
			}
		}
	}


	wintrustData.dwStateAction = WTD_STATEACTION_CLOSE;

	m_PtrWinVerifyTrustPL(
		(HWND)INVALID_HANDLE_VALUE, // Do not display any UI.
		&actionGUID, // V2 verification
		&wintrustData);

	if (verified != 1)
	{
		CloseHandle(hFile);
		*outResult = ovrPlatformInitialize_SignatureInvalid;
		return INVALID_HANDLE_VALUE;
	}

	*outResult = ovrPlatformInitialize_Success;
	return hFile;
}

static ModuleHandleType OVR_OpenLibrary(const FilePathCharType* libraryPath, ovrPlatformInitializeResult* outResult)
{
	DWORD fullPathNameLen = 0;
	FilePathCharType fullPath[MAX_PATH] = { 0 };
	HANDLE hFilePinned = INVALID_HANDLE_VALUE;
	ModuleHandleType hModule = 0;
	fullPathNameLen = GetFullPathNameW(libraryPath, MAX_PATH, fullPath, 0);
	if (fullPathNameLen <= 0 || fullPathNameLen >= MAX_PATH)
	{
		*outResult = ovrPlatformInitialize_FileInvalid;
		return ModuleHandleTypeNull;
	}
	fullPath[MAX_PATH - 1] = 0;

	auto signResult = ovrPlatformInitialize_Uninitialized;
	hFilePinned = OVR_Win32_SignCheck(fullPath, &signResult);
#ifdef OVR_INTERNAL_CODE
	// Ignore signature check errors in internal builds, since in-development builds are not signed
	if (signResult == ovrPlatformInitialize_SignatureInvalid)
	{
		signResult = ovrPlatformInitialize_Success;
	}
#else // OVR_INTERNAL_CODE
	if (hFilePinned == INVALID_HANDLE_VALUE)
	{
		// Some sort of error verifying the file
		// Set that as the out result, but try to LoadLibrary anyway and send that as the return value. If
		// the developer ignores the error they can try to call into the library, it'll be loaded automatically,
		// and then their call will hit an assert because we didn't call the platform initialization function.
		*outResult = signResult;
		return LoadLibraryW(fullPath);
	}
#endif // OVR_INTERNAL_CODE

	// In this case we thought we found a valid library at the location, so we'll load it, and if that
	// fails for some reason we'll mark it as an invalid file error.
	hModule = LoadLibraryW(fullPath);

	if (hFilePinned != INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFilePinned);
	}

	if (hModule == ModuleHandleTypeNull)
	{
		*outResult = ovrPlatformInitialize_FileInvalid;
	}
	else
	{
		*outResult = signResult;
	}
	return hModule;
}


static void OVR_CloseLibrary(ModuleHandleType hLibrary)
{
	if (hLibrary)
	{
		// We may need to consider what to do in the case that the library is in an exception state.
		// In a Windows C++ DLL, all global objects (including static members of classes) will be constructed just
		// before the calling of the DllMain with DLL_PROCESS_ATTACH and they will be destroyed just after
		// the call of the DllMain with DLL_PROCESS_DETACH. We may need to intercept DLL_PROCESS_DETACH and
		// have special handling for the case that the DLL is broken.
		FreeLibrary(hLibrary);
	}
}


// Returns a valid ModuleHandleType (e.g. Windows HMODULE) or returns ModuleHandleTypeNull (e.g. NULL).
// The caller is required to eventually call OVR_CloseLibrary on a valid return handle.
//
static ModuleHandleType OVR_FindLibraryPath(int requestedProductVersion, int requestedMajorVersion,
	ovrPlatformInitializeResult *outResult,
	FilePathCharType* libraryPath, size_t libraryPathCapacity)
{
	ModuleHandleType moduleHandle;
	int printfResult;
	FilePathCharType developerDir[OVR_MAX_PATH] = { '\0' };

#if defined(_WIN64)
	const char* pBitDepth = "64";
#else
	const char* pBitDepth = "32";
#endif

	(void)requestedProductVersion;

	moduleHandle = ModuleHandleTypeNull;
	if (libraryPathCapacity)
		libraryPath[0] = '\0';

#define OVR_FILE_PATH_SEPARATOR "\\"

	{
		const char* pLibOvrDllDir = getenv("LIBOVR_DLL_DIR"); // Example value: ..\\LibOVRPlatform\\msvc-out\\x64\\Debug

		if (pLibOvrDllDir)
		{
			char developerDir8[OVR_MAX_PATH];
			size_t length = OVR_strlcpy(developerDir8, pLibOvrDllDir, sizeof(developerDir8)); // If missing a trailing path separator then append one.

			if ((length > 0) && (length < sizeof(developerDir8)) && (developerDir8[length - 1] != OVR_FILE_PATH_SEPARATOR[0]))
			{
				length = OVR_strlcat(developerDir8, OVR_FILE_PATH_SEPARATOR, sizeof(developerDir8));

				if (length < sizeof(developerDir8))
				{
					size_t i;
					for (i = 0; i <= length; ++i) // ASCII conversion of 8 to 16 bit text.
						developerDir[i] = (FilePathCharType)(uint8_t)developerDir8[i];
				}
			}
		}
	}

	auto lastOpenResult = ovrPlatformInitialize_Uninitialized;

	{
		size_t i;

		// On Windows, only search the developer directory and the usual path
		const FilePathCharType* directoryArray[2];
		directoryArray[0] = developerDir; // Developer directory.
		directoryArray[1] = L""; // No directory, which causes Windows to use the standard search strategy to find the DLL.

		for (i = 0; i < sizeof(directoryArray) / sizeof(directoryArray[0]); ++i)
		{
			printfResult = swprintf(libraryPath, libraryPathCapacity, L"%lsLibOVRPlatform%hs_%d.dll", directoryArray[i], pBitDepth, requestedMajorVersion);

			if (*directoryArray[i] == 0)
			{
				int k;
				FilePathCharType foundPath[MAX_PATH] = { 0 };
				DWORD searchResult = SearchPathW(NULL, libraryPath, NULL, MAX_PATH, foundPath, NULL);
				if (searchResult <= 0 || searchResult >= libraryPathCapacity)
				{
					continue;
				}
				foundPath[MAX_PATH - 1] = 0;
				for (k = 0; k < MAX_PATH; ++k)
				{
					libraryPath[k] = foundPath[k];
				}
			}

			if ((printfResult >= 0) && (printfResult < (int)libraryPathCapacity))
			{
				auto openResult = ovrPlatformInitialize_Uninitialized;
				moduleHandle = OVR_OpenLibrary(libraryPath, &openResult);
				if (moduleHandle != ModuleHandleTypeNull)
				{
					*outResult = openResult;
					return moduleHandle;
				}
				else
				{
					lastOpenResult = openResult;
				}
			}
		}
	}

	*outResult = lastOpenResult != ovrPlatformInitialize_Uninitialized ? lastOpenResult : ovrPlatformInitialize_UnableToVerify;
	return moduleHandle;
}


OVRPL_DECLARE_IMPORT(ovrPlatformInitializeResult, ovr_PlatformInitializeWindows, (const char *appId));
OVRPL_DECLARE_IMPORT(ovrMessage*, ovr_PopMessage, ());
OVRPL_DECLARE_IMPORT(bool, ovr_IsEntitled, ());
OVRPL_DECLARE_IMPORT(void, ovr_PlatformInitializeStandaloneAccessToken, (const char *accessToken));

static void LoadFunctions(ModuleHandleType hModule) {
	OVRPL_GETFUNCTION(hModule, ovr_PlatformInitializeWindows);
	OVRPL_GETFUNCTION(hModule, ovr_PopMessage);
	OVRPL_GETFUNCTION(hModule, ovr_IsEntitled);
	OVRPL_GETFUNCTION(hModule, ovr_PlatformInitializeStandaloneAccessToken);
}

static ovrPlatformInitializeResult InitializeResult = ovrPlatformInitialize_Uninitialized;

OVRPL_PUBLIC_FUNCTION(ovrPlatformInitializeResult) ovr_PlatformInitializeWindowsEx(const char *appId, int productVersion, int majorVersion)
{
	if (InitializeResult != ovrPlatformInitialize_Uninitialized) {
		return InitializeResult;
	}

	// Check to make sure the headers they're using (which automatically pass the version to this function) match the version
	// that this loader library was compiled under.
	if (productVersion != PLATFORM_PRODUCT_VERSION || majorVersion != PLATFORM_MAJOR_VERSION) {
		InitializeResult = ovrPlatformInitialize_VersionMismatch;
		return InitializeResult;
	}

	// See if the one we want has already been loaded by mirroring the OVR_FindLibraryPath name generation but without the path
	FilePathCharType preLoadLibName[OVR_MAX_PATH];
	FilePathCharType preLoadFilePath[OVR_MAX_PATH];

#if defined(_WIN64)
	const char* pBitDepth = "64";
#else
	const char* pBitDepth = "32";
#endif

	swprintf(preLoadLibName, sizeof(preLoadLibName) / sizeof(preLoadLibName[0]), L"LibOVRPlatform%hs_%d.dll", pBitDepth, PLATFORM_MAJOR_VERSION);
	auto hLibPreLoad = GetModuleHandleW(preLoadLibName);
	if (hLibPreLoad != NULL) {
		// Someone already loaded the module. Might be fine, just copy the path out so we can check it later.
		GetModuleFileNameW(hLibPreLoad, preLoadFilePath, sizeof(preLoadFilePath) / sizeof(preLoadFilePath[0]));
	}

	FilePathCharType filePath[OVR_MAX_PATH];

	auto hLib = OVR_FindLibraryPath(PLATFORM_PRODUCT_VERSION, PLATFORM_MAJOR_VERSION, &InitializeResult, filePath, sizeof(filePath) / sizeof(filePath[0]));

	if (InitializeResult == ovrPlatformInitialize_Success) {
		if (hLibPreLoad != NULL && wcsicmp(filePath, preLoadFilePath) != 0) {
			// The pre-loaded module was on a different path than the validated library. Not a particularly likely case, but if it happens we should fail, since
			// the non-shimmed functions could call into the wrong library.
			InitializeResult = ovrPlatformInitialize_PreLoaded;
			return InitializeResult;
		}

		LoadFunctions(hLib);
		ovr_PlatformInitializeWindowsPLPtr(appId);
	}

	return InitializeResult;
}


OVRPL_PUBLIC_FUNCTION(ovrMessage*) ovr_PopMessage() {
	if (InitializeResult == ovrPlatformInitialize_Success) {
		return ovr_PopMessagePLPtr();
	}
	return nullptr;
}

OVRPL_PUBLIC_FUNCTION(bool) ovr_IsEntitled() {
	if (InitializeResult == ovrPlatformInitialize_Success) {
		return ovr_IsEntitledPLPtr();
	}
	return false;
}

// We also shim the standalone initializer just for consistency in debugging, basically a shorter version of the windows init since we don't care about safety
OVRPL_PUBLIC_FUNCTION(void) ovr_PlatformInitializeStandaloneAccessToken(const char* accessToken) {
	if (InitializeResult != ovrPlatformInitialize_Uninitialized) {
		return;
	}

	InitializeResult = ovrPlatformInitialize_Success;

	FilePathCharType preLoadLibName[OVR_MAX_PATH];

#if defined(_WIN64)
	const char* pBitDepth = "64";
#else
	const char* pBitDepth = "32";
#endif

	swprintf(preLoadLibName, sizeof(preLoadLibName) / sizeof(preLoadLibName[0]), L"LibOVRPlatform%hs_%d.dll", pBitDepth, PLATFORM_MAJOR_VERSION);
	auto hLibPreLoad = GetModuleHandleW(preLoadLibName);

	LoadFunctions(hLibPreLoad);
	ovr_PlatformInitializeStandaloneAccessTokenPLPtr(accessToken);
}

// Resetting to pre-init state without unloading completely
// Useful for development purposes such as running it in the Unity or UE4 editor
OVRP_PUBLIC_FUNCTION(void) ovr_UnityResetTestPlatform();
OVRPL_PUBLIC_FUNCTION(void) ovr_ResetInitAndContext()
{
	ovr_UnityResetTestPlatform();
	InitializeResult = ovrPlatformInitialize_Uninitialized;
}

#endif
