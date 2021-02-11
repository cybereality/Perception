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
#ifndef NOD_FIXEDVALUES
#define NOD_FIXEDVALUES

#ifndef _TRACE
#define TRACE_UINT(a) { wchar_t buf[128]; wsprintf(buf, L"%s:%u", L#a, a); OutputDebugString(buf); }
#define TRACE_HEX(a) { wchar_t buf[128]; wsprintf(buf, L"%s:%x", L#a, a); OutputDebugString(buf); }
#define TRACE_LINE { wchar_t buf[128]; wsprintf(buf, L"LINE : %d", __LINE__); OutputDebugString(buf); }
#endif

/**
* True boolean.
***/
class NOD_True : public NOD_Basic
{
public:	NOD_True(LONG nX, LONG nY) :NOD_Basic(nX, nY, 60, 40) {
	m_paCommanders.clear();
	{ m_szTitle = L"True"; m_eNodeProvokingType = AQU_NodeProvokingType::None; }
			{ static int m_nTrue = TRUE; NOD_Commander * pC = new NOD_Commander();	pC->m_ePlugtype = NOD_Plugtype::AQU_BOOL; pC->m_pOutput = &m_nTrue; pC->m_szTitle = L"bool true"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC);	}
} virtual const char* GetNodeType() { return typeid(this).name(); } virtual UINT32 GetNodeTypeId() { return ELEMENTARY_NODE_TRUE; }
};

/**
* False boolean.
***/
class NOD_False : public NOD_Basic
{
public: NOD_False(LONG nX, LONG nY) :NOD_Basic(nX, nY, 60, 40) {
	m_paCommanders.clear();
	{ m_szTitle = L"False"; m_eNodeProvokingType = AQU_NodeProvokingType::None; }
			{ static int m_nFalse = FALSE; NOD_Commander * pC = new NOD_Commander();	pC->m_ePlugtype = NOD_Plugtype::AQU_BOOL; pC->m_pOutput = &m_nFalse; pC->m_szTitle = L"bool false"; pC->m_paDecommanders.clear(); m_paCommanders.push_back(pC);	}
} virtual const char* GetNodeType() { return typeid(this).name(); } virtual UINT32 GetNodeTypeId() { return ELEMENTARY_NODE_FALSE; }
};

/**
* Fixed float value.
* Takes float value from clipboard as output.
***/
class NOD_FixedFloat : public NOD_Basic
{
public:
	NOD_FixedFloat(LONG nX, LONG nY) :NOD_Basic(nX, nY, 100, 40)
	{
		m_szTitle = L"Fixed float value";

		// get clipboard text and set value
		std::string szClipboard = GetClipboardText();
		std::stringstream szA = std::stringstream(szClipboard);
		szA >> m_fValue;

		// create commander
		NOD_Commander * pC = new NOD_Commander();
		pC->m_ePlugtype = NOD_Plugtype::AQU_FLOAT;
		pC->m_pOutput = &m_fValue;
		pC->m_szTitle = new wchar_t[64];

		// set value as commander title
		ZeroMemory((void*)&pC->m_szTitle[0], 64 * sizeof(wchar_t));
		std::wstringstream sz;
		sz << m_fValue;
		memcpy((void*)&pC->m_szTitle[0], &sz.str().c_str()[0], sz.str().size() * sizeof(wchar_t));
		m_paCommanders.push_back(pC);
	}

	virtual DWORD            GetSaveDataSize() { return sizeof(float); }
	virtual char*            GetSaveData(UINT* pdwSizeOfData) { *pdwSizeOfData = sizeof(float); return (char*)&m_fValue; }
	virtual void             InitNodeData(char* pData, UINT dwSizeOfData)
	{
		if (dwSizeOfData == sizeof(float))
		{
			// set value and value as title
			ZeroMemory((void*)&m_paCommanders[0]->m_szTitle[0], 64 * sizeof(wchar_t));
			memcpy(&m_fValue, (void*)pData, dwSizeOfData);
			std::wstringstream sz;
			sz << m_fValue;
			memcpy((void*)&m_paCommanders[0]->m_szTitle[0], &sz.str().c_str()[0], sz.str().size() * sizeof(wchar_t));
		}
	}

	virtual const char* GetNodeType() { return typeid(this).name(); }
	virtual UINT32 GetNodeTypeId() { return ELEMENTARY_NODE_FIXED_FLOAT; }

	/**
	* The stored float value.
	***/
	float m_fValue;
};

/**
* Fixed int value.
* Takes int value from clipboard as output.
***/
class NOD_FixedInt : public NOD_Basic
{
public:
	NOD_FixedInt(LONG nX, LONG nY) :NOD_Basic(nX, nY, 100, 40)
	{
		m_szTitle = L"Fixed int value";

		// get clipboard text and set value
		std::string szClipboard = GetClipboardText();
		std::stringstream szA = std::stringstream(szClipboard);
		szA >> m_nValue;

		// create commander
		NOD_Commander * pC = new NOD_Commander();
		pC->m_ePlugtype = NOD_Plugtype::AQU_INT;
		pC->m_pOutput = &m_nValue;
		pC->m_szTitle = new wchar_t[64];

		// set value as commander title
		ZeroMemory((void*)&pC->m_szTitle[0], 64 * sizeof(wchar_t));
		std::wstringstream sz;
		sz << m_nValue;
		memcpy((void*)&pC->m_szTitle[0], &sz.str().c_str()[0], sz.str().size() * sizeof(wchar_t));
		m_paCommanders.push_back(pC);

		// create commander for uint
		NOD_Commander* pC_Uint = new NOD_Commander();
		pC_Uint->m_ePlugtype = NOD_Plugtype::AQU_UINT;
		pC_Uint->m_pOutput = &m_nValue;
		pC_Uint->m_szTitle = L"unsigned";
		m_paCommanders.push_back(pC_Uint);

	}

	virtual DWORD            GetSaveDataSize() { return sizeof(INT32); }
	virtual char*            GetSaveData(UINT* pdwSizeOfData) { *pdwSizeOfData = sizeof(INT32); return (char*)&m_nValue; }
	virtual void             InitNodeData(char* pData, UINT dwSizeOfData)
	{
		if (dwSizeOfData == sizeof(float))
		{
			// set value and value as title
			ZeroMemory((void*)&m_paCommanders[0]->m_szTitle[0], 64 * sizeof(wchar_t));
			memcpy(&m_nValue, (void*)pData, dwSizeOfData);
			std::wstringstream sz;
			sz << m_nValue;
			memcpy((void*)&m_paCommanders[0]->m_szTitle[0], &sz.str().c_str()[0], sz.str().size() * sizeof(wchar_t));
		}
	}

	virtual const char* GetNodeType() { return typeid(this).name(); }
	virtual UINT32 GetNodeTypeId() { return ELEMENTARY_NODE_FIXED_INT; }

	/**
	* The stored int value.
	***/
	INT32 m_nValue;
};

/**
* Fixed W string value.
* Takes float value from clipboard as output.
***/
class NOD_FixedWString : public NOD_Basic
{
public:
	NOD_FixedWString(LONG nX, LONG nY) :NOD_Basic(nX, nY, 140, 40)
	{
		m_szTitle = L"Fixed w string";

		// get clipboard text and set value
		std::string szClipboard = GetClipboardText();
		m_szwValue = std::wstring(szClipboard.begin(), szClipboard.end());
		size_t nSize = m_szwValue.length();
		if (nSize >= 64) nSize = 63;
		if (nSize < 0) nSize = 0;
		memcpy(&m_szwValueRaw[0], &m_szwValue[0], sizeof(wchar_t)*nSize);
		m_szwValueRaw[nSize] = 0;

		// create commander
		NOD_Commander * pC = new NOD_Commander();
		pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_WCHAR;
		pC->m_pOutput = &m_szwValue;
		pC->m_szTitle = &m_szwValue[0];
		m_paCommanders.push_back(pC);
	}

	virtual DWORD            GetSaveDataSize() { return sizeof(wchar_t) * 64; }
	virtual char*            GetSaveData(UINT* pdwSizeOfData) { *pdwSizeOfData = sizeof(wchar_t) * 64; return (char*)&m_szwValueRaw[0]; }
	virtual void             InitNodeData(char* pData, UINT dwSizeOfData)
	{
		if (dwSizeOfData == sizeof(wchar_t) * 64)
		{
			// set value and value as title
			memcpy(&m_szwValueRaw[0], (void*)pData, dwSizeOfData);
			m_szwValue = std::wstring(m_szwValueRaw);
			m_paCommanders[0]->m_szTitle = &m_szwValue[0];
		}
	}

	virtual const char* GetNodeType() { return typeid(this).name(); }
	virtual UINT32 GetNodeTypeId() { return ELEMENTARY_NODE_FIXED_WSTRING; }

	/**
	* The stored float value.
	***/
	std::wstring m_szwValue;
	/**
	* The raw data.
	***/
	wchar_t m_szwValueRaw[64];
};

/**
* Fixed pointer.
* Takes pointer address in XML format from clipboard.
***/
class NOD_FixedPointer : public NOD_Basic
{
public:
	NOD_FixedPointer(LONG nX, LONG nY) : NOD_Basic(nX, nY, 100, 100)
	{
		// fields
		static UINT_PTR pAddress = 0;
		static std::string szProcess = std::string();
		static std::string szModule = std::string();
		m_szTitle = new wchar_t[64];
		ZeroMemory((void*)&m_szTitle[0], 64 * sizeof(wchar_t));
		ZeroMemory((void*)&m_sFixedPointerAddress, sizeof(FixedPointerAddress));

		// get text
		std::string szClipboard = GetClipboardText();
		// OutputDebugStringA(szClipboard.c_str());

		// TODO !! test xml header !!

		// get stream
		std::wstringstream szStream;
		szStream << szClipboard.c_str();
		szStream.seekg(0);
		szStream.seekp(0);

		// get tokens
		std::vector<size_t> aOffsets;
		std::wstring szToken;
		while (szStream >> szToken)
		{
			// <Description>"Yaw"</Description>
			if (szToken.compare(0, 13, L"<Description>") == 0)
			{
				// get the description
				size_t nPos = szToken.find(L"</Description>");
				if ((nPos != std::wstring::npos) && (nPos > 13))
				{
					std::wstring szDescription = szToken.substr(13, nPos - 13);

					// copy the description as node title
					memcpy((void*)&m_szTitle[0], &szDescription.c_str()[0], szDescription.length() * sizeof(wchar_t));
					memcpy((void*)&m_sFixedPointerAddress.szTitle[0], &szDescription.c_str()[0], szDescription.length() * sizeof(wchar_t));

				}
				OutputDebugString(m_szTitle);
			}

			// <VariableType>Float</VariableType>
			if (szToken.compare(0, 14, L"<VariableType>") == 0)
			{
				// get the description
				size_t nPos = szToken.find(L"</VariableType>");
				if ((nPos != std::wstring::npos) && (nPos > 14))
				{
					std::wstring szDescription = szToken.substr(13, nPos - 13);

					if (szToken.compare(14, 6, L"Binary") == 0)
						m_sFixedPointerAddress.eType = NOD_FixedPointer::FixedPointerAddress::Binary;
					else if (szToken.compare(14, 5, L"Byte8") == 0)
						m_sFixedPointerAddress.eType = NOD_FixedPointer::FixedPointerAddress::Byte8;
					else if (szToken.compare(14, 5, L"Byte4") == 0)
						m_sFixedPointerAddress.eType = NOD_FixedPointer::FixedPointerAddress::Byte4;
					else if (szToken.compare(14, 5, L"Byte2") == 0)
						m_sFixedPointerAddress.eType = NOD_FixedPointer::FixedPointerAddress::Byte2;
					else if (szToken.compare(14, 4, L"Byte") == 0)
						m_sFixedPointerAddress.eType = NOD_FixedPointer::FixedPointerAddress::Byte;
					else if (szToken.compare(14, 5, L"Float") == 0)
						m_sFixedPointerAddress.eType = NOD_FixedPointer::FixedPointerAddress::Float;
					else if (szToken.compare(14, 6, L"Binary") == 0)
						m_sFixedPointerAddress.eType = NOD_FixedPointer::FixedPointerAddress::Binary;
					else if (szToken.compare(14, 6, L"Double") == 0)
						m_sFixedPointerAddress.eType = NOD_FixedPointer::FixedPointerAddress::Double;
					else if (szToken.compare(14, 5, L"Array") == 0)
						m_sFixedPointerAddress.eType = NOD_FixedPointer::FixedPointerAddress::ArrayOfBytes;
				}
				OutputDebugString(L"Type:");
				TRACE_UINT(m_sFixedPointerAddress.eType);
			}

			// <Address>"ABC.exe" + 03972828< / Address>
			if (szToken.compare(0, 9, L"<Address>") == 0)
			{
				DWORD dwAddress;
				swscanf_s(szToken.substr(szToken.find_last_of(L"+") + 1).c_str(), L"%x", &dwAddress);
				TRACE_HEX(dwAddress);
				OutputDebugString(szToken.substr(szToken.find_last_of(L"+") + 1).c_str());
				m_sFixedPointerAddress.nPointerAddress = (size_t)dwAddress;

				// set process respective address string process
				if (szToken.find('"') != std::string::npos)
				{
					// get the module name
					std::wstring szModule = szToken.substr(10, szToken.find_last_of('"') - 1);
					std::string szModuleA; // = std::string(szModule.begin(), szModule.end());
					for (wchar_t x : szModule) szModuleA += (char)x;
					if (szModuleA.size() > 126) szModuleA = szModuleA.substr(0, 126);
					memcpy(&m_sFixedPointerAddress.szModuleName[0], &szModuleA.c_str()[0], szModuleA.size() * sizeof(char));
					m_sFixedPointerAddress.bStatic = false;

					OutputDebugStringA(m_sFixedPointerAddress.szModuleName);
				}
				else
				{
					// no base address - static pointers
					m_sFixedPointerAddress.bStatic = true;
				}
			}

			// <Offset>A8< / Offset>
			if (szToken.compare(0, 8, L"<Offset>") == 0)
			{
				DWORD dwOffset;
				swscanf_s(szToken.substr(8).c_str(), L"%x", &dwOffset);
				TRACE_HEX(dwOffset);
				aOffsets.push_back((size_t)dwOffset);
			}
		}

		// set the offsets...
		m_sFixedPointerAddress.nOffsetNumber = aOffsets.size();
		if (aOffsets.size() <= 5)
		{
			for (size_t nI = aOffsets.size(); nI > 0; nI--)
			{
				m_sFixedPointerAddress.nOffsets[aOffsets.size() - nI] = aOffsets[nI - 1];
				TRACE_UINT(aOffsets.size() - nI);
				TRACE_HEX(aOffsets[nI - 1]);
			}
		}
		else
			OutputDebugString(L"NOD_FixedValues: Too much pointer offsets !");

		NOD_Commander * pC = new NOD_Commander();
		pC->m_ePlugtype = NOD_Plugtype::AQU_FIXEDPOINTER;
		pC->m_pOutput = &m_sFixedPointerAddress;
		pC->m_szTitle = new wchar_t[64];
		wsprintf((LPWSTR)&pC->m_szTitle[0], L"%x", m_sFixedPointerAddress.nPointerAddress);
		pC->m_paDecommanders.clear();
		m_paCommanders.push_back(pC);
	}

	virtual DWORD            GetSaveDataSize() { return sizeof(FixedPointerAddress); }
	virtual char*            GetSaveData(UINT* pdwSizeOfData) { *pdwSizeOfData = sizeof(FixedPointerAddress); return (char*)&m_sFixedPointerAddress; }
	virtual void             InitNodeData(char* pData, UINT dwSizeOfData) { if (dwSizeOfData == sizeof(FixedPointerAddress)) { memcpy(&m_sFixedPointerAddress, (void*)pData, dwSizeOfData); memcpy((void*)&m_szTitle[0], &m_sFixedPointerAddress.szTitle[0], 64 * sizeof(wchar_t)); wsprintf((LPWSTR)&m_paCommanders[0]->m_szTitle[0], L"%x", m_sFixedPointerAddress.nPointerAddress); } }
	virtual const char*      GetNodeType() { return typeid(this).name(); }
	virtual UINT32           GetNodeTypeId() { return ELEMENTARY_NODE_FIXED_POINTER; }

	/**
	* This node has an own connector : NOD_Plugtype::AQU_FIXEDPOINTER.
	***/
	struct FixedPointerAddress
	{
		size_t nPointerAddress;
		size_t nOffsetNumber;
		size_t nOffsets[4];
		char szModuleName[128];
		wchar_t szTitle[64];
		enum Type
		{
			Binary,
			Byte,
			Byte2,
			Byte4,
			Byte8,
			Float,
			Double,
			Text,
			ArrayOfBytes
		} eType;
		bool bStatic;
	} m_sFixedPointerAddress;
};

/**
* Aquilinus configuration node.
***/
class NOD_AquilinusConfig : public NOD_Basic
{
public:
	NOD_AquilinusConfig(LONG nX, LONG nY) : NOD_Basic(nX, nY, 100, 260)
	{
		ZeroMemory(&m_sConfig, sizeof(AquilinusCfg));

		// get process name
		TCHAR szBuffer[MAX_PATH];
		if (GetModuleFileNameW(NULL, szBuffer, MAX_PATH))
		{
			// create the process string
			m_szProc = std::wstring(szBuffer);
			m_szProc = m_szProc.substr(m_szProc.find_last_of(L"\\/") + 1);
		}
		else
		{
			m_szProc = std::wstring(L"NO PROCESS!");
		}

		// set title (=process)
		m_szTitle = &m_szProc[0];

		// get the config map handle
		HANDLE hConfigMapFile = OpenFileMapping(
			FILE_MAP_ALL_ACCESS,   // read/write access
			FALSE,                 // do not inherit the name
			TEXT("AquilinusCfg"));   // name of the Aquilinus config

		// return if failed
		if (hConfigMapFile == NULL)
		{
			OutputDebugString(TEXT("Aquilinus : Could not create file mapping object.\n"));
			return;
		}

		// create map view
		AquilinusCfg* psAquilinusConfig = (AquilinusCfg*)
			MapViewOfFile(hConfigMapFile,   // handle to map object
			FILE_MAP_ALL_ACCESS,              // read/write permission
			0,
			0,
			sizeof(AquilinusCfg));

		// return if failed
		if (psAquilinusConfig == NULL)
		{
			OutputDebugString(TEXT("Aquilinus : Could not map view of file.\n"));
			CloseHandle(hConfigMapFile);
			return;
		}

		// copy configuration
		memcpy(&m_sConfig, (void*)psAquilinusConfig, sizeof(AquilinusCfg));

		// close handle
		CloseHandle(hConfigMapFile);

		// create commanders
		NOD_Commander * pC = new NOD_Commander();
		pC->m_ePlugtype = NOD_Plugtype::AQU_PNT_WCHAR;
		pC->m_szTitle = &m_szProc[0];
		pC->m_pOutput = (void*)&m_szProc;
		pC->m_paDecommanders.clear();
		m_paCommanders.push_back(pC);
	}
	virtual const char* GetNodeType() { return typeid(this).name(); }
	virtual UINT32 GetNodeTypeId() { return ELEMENTARY_NODE_CONFIGURATION; }

	/**
	* Sets the Aquilinus configuration internally.
	***/
	void SetConfig(AquilinusCfg* psConfig) { if (!psConfig) return; memcpy(&m_sConfig, (void*)psConfig, sizeof(AquilinusCfg)); }

private:
	/**
	* Private copy of the Aquilinus configuration.
	***/
	AquilinusCfg m_sConfig;
	/**
	* The process string.
	***/
	std::wstring m_szProc;
};


#endif