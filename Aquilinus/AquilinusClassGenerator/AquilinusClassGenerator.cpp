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
#include "AquilinusClassGenerator.h"

/**
* Simple tool to generate Aquilinus Code files.
* @param argv[0] 
* @param argv[1]
***/
int _tmain(int argc, _TCHAR* argv[])
{
	if ( argc != 2 ) 
	{
		OutputDebugString(L"AquilinusClassGenerator <filename>");
		OutputDebugString(L"<filename> needs to be a text file containing the class definition.");
	}
	else
	{

#pragma region Read Input Data

		// get file name
		std::wstring szFileName = std::wstring(argv[1]);
		OutputDebugString(szFileName.c_str());

		// open file
		std::wifstream ioFile;
		ioFile.open(szFileName.c_str());

		// all necessary class data
		enum ReadInSteps
		{
			DetectD3DVersion,
			DetectClass,
			DetectFirstMethod,
			ReadInMethods,
			Finished
		} eReadInSteps = ReadInSteps::DetectD3DVersion;
		/**
		* The global class name.
		***/
		std::wstring szClassName;
		/**
		* The global class name (uppercase).
		***/
		std::wstring szClassNameUpper;
		/**
		* A vector of all class type names. (f.i. "iUnknown", "iDXGISwapChain", "...")
		***/
		std::vector<std::wstring> aszClassTypes;
		/**
		* The method index of the first method corresponding aszClassTypes[<thisIndex>] class name.
		* (f.i. method "QueryInterface" is index "0" at aszClassTypes[0]....)
		***/
		std::vector<UINT> adwClassTypeIndices;
		/**
		* The type of the method correspondig aszMethodNames[<thisIndex>].
		* (f.i. method type "HRESULT" for aszMethodTypes[x] corresponds "Present" for aszMethodNames[x])
		***/
		std::vector<std::wstring> aszMethodTypes;
		/**
		* The convention of the method correspondig aszMethodNames[<thisIndex>].
		* (f.i. method convention "WINAPI" for aszMethodConvention[x] corresponds "Present" for aszMethodNames[x])
		***/
		std::vector<std::wstring> aszMethodConvention;
		/**
		* The name of the method correspondig aszMethodTypes[<thisIndex>].
		* (f.i. method type "HRESULT" for aszMethodTypes[x] corresponds "Present" for aszMethodNames[x])
		***/
		std::vector<std::wstring> aszMethodNames;
		/**
		* The type names of the parameters for the method corresponding aszMethodNames[<thisIndex>].
		***/
		std::vector<std::vector<std::wstring>> aaszMethodParameterTypes;
		/**
		* The names of the parameters for the method corresponding aszMethodNames[<thisIndex>].
		***/
		std::vector<std::vector<std::wstring>> aaszMethodParameterNames;
		/**
		* The chosen Direct3D version.
		* (in the class file, set "D3D 10" for D3D 10 and so on.
		* do that BEFORE the "class" line)
		***/
		UINT dwD3DVersion;

		// read the file and fill in all information about the class
		while (!ioFile.eof()) 
		{
			std::wstring szOutput;
			std::getline(ioFile, szOutput);

			switch (eReadInSteps)
			{
			case DetectD3DVersion:
				{
					// get line stream
					std::wstringstream szStream(szOutput); 

					// parse first word
					std::wstring szWord;
					szStream >> szWord;

					// is class ?
					if (szWord.compare(L"D3D") == 0)
					{
						// get class name
						szStream >> dwD3DVersion;
						eReadInSteps = ReadInSteps::DetectClass;
					}
				}
				break;
			case DetectClass:
				{
					// get line stream
					std::wstringstream szStream(szOutput); 

					// parse first word
					std::wstring szWord;
					szStream >> szWord;

					// is class ?
					if (szWord.compare(L"class") == 0)
					{
						// get class name
						szStream >> szClassName;
						while (szClassName[0] == ' ') szClassName.erase(0,1);

						// get class name uppercase
						szClassNameUpper = std::wstring(szClassName);
						for (auto & c: szClassNameUpper) c = toupper(c);

						// output debug and step forward
						/*OutputDebugString(L"Class name : ");
						OutputDebugString(szClassName.c_str());
						OutputDebugString(szClassNameUpper.c_str());*/
						eReadInSteps = ReadInSteps::DetectFirstMethod;
					}
				}
				break;
			case DetectFirstMethod:
				{
					// get line stream
					std::wstringstream szStream(szOutput); 

					// parse first word
					std::wstring szWord;
					szStream >> szWord;

					// class methods ? (always declared as followed : /*** <class name> public methods ***/
					if (szWord.compare(L"/***") == 0)
					{
						// set new class type
						std::wstring szType;
						szStream >> szType;
						while (szType[0] == ' ') szType.erase(0,1);
						UINT dwIndex = (UINT)aszMethodTypes.size();
						aszClassTypes.push_back(szType);
						adwClassTypeIndices.push_back(dwIndex);

						/*OutputDebugString(L"New class type :");
						OutputDebugString(szType.c_str());*/

						// get class name
						eReadInSteps = ReadInSteps::ReadInMethods;
					}
				}
				break;
			case ReadInMethods:
				{
					// get line stream
					std::wstringstream szStream(szOutput); 

					// parse first word
					std::wstring szWord;
					szStream >> szWord;

					// class methods ? (always declared as followed : /*** <class name> public methods ***/
					if (szWord.compare(L"/***") == 0)
					{
						// set new class type
						std::wstring szType;
						szStream >> szType;
						while (szType[0] == ' ') szType.erase(0,1);
						UINT dwIndex = (UINT)aszMethodTypes.size();
						aszClassTypes.push_back(szType);
						adwClassTypeIndices.push_back(dwIndex);

						OutputDebugString(L"New class type :");
						OutputDebugString(szType.c_str());
					}
					else if (szWord.compare(L"}") == 0)
					{
						// finished
						eReadInSteps = ReadInSteps::Finished;
					}
					else if (szWord.size() == 0)
					{
						// do nothing if empty line
					}
					else
					{
						// read in method type
						while (szWord[0] == ' ') szWord.erase(0,1);
						//OutputDebugString(szWord.c_str());

						// read in method convention
						std::wstring szConvention;
						szStream >> szConvention;
						while (szConvention[0] == ' ') szConvention.erase(0,1);
						//OutputDebugString(szConvention.c_str());

						// read in method convention
						std::wstring szName;
						wchar_t ch;
						while (szStream.get(ch) && ch!='(')
							szName+=ch;
						while (szName[0] == ' ') szName.erase(0,1);
						//OutputDebugString(szName.c_str());

						// read in parameters
						std::vector<std::wstring> aszTypes;
						std::vector<std::wstring> aszParams;
						while (!szStream.eof())
						{
							// get the parameter type
							std::wstring szType;
							szStream >> szType;
							while (szType[0] == ' ') szType.erase(0,1);
							while (szType[0] == ',') szType.erase(0,1);

							// ignore 'const'
							while (szType.compare(L"const") == 0)
							{
								szType = std::wstring();
								szStream >> szType;
								while (szType[0] == ' ') szType.erase(0,1);
							}

							// delete end
							if (szType.find(L");") != std::string::npos)
							{
								szType.erase(szType.length()-2, 2);
							}

							// get the parameter name
							std::wstring szParam;
							szStream >> szParam;
							while (szParam[0] == ' ') szParam.erase(0,1);
							while (szParam[0] == ',') szParam.erase(0,1);

							// move all pointers (*) to the type
							while (szParam.find('*') != std::string::npos)
							{
								szType+='*';
								szParam.erase(0,1);
							}

							// delete ',' at the end of the param
							if (szParam.find(',') != std::string::npos)
							{
								szParam.erase(szParam.length()-1, 1);
							}

							// delete end
							if (szParam.find(L");") != std::string::npos)
							{
								szParam.erase(szParam.length()-2, 2);
							}

							//OutputDebugString(szType.c_str());
							//OutputDebugString(szParam.c_str());

							// push back
							aszTypes.push_back(szType);
							aszParams.push_back(szParam);
						}

						// push all back... if there is data
						if ((szWord.size()) && (szConvention.size()))
						{
							aszMethodTypes.push_back(szWord);
							aszMethodConvention.push_back(szConvention);
							aszMethodNames.push_back(szName);
							aaszMethodParameterTypes.push_back(aszTypes);
							aaszMethodParameterNames.push_back(aszParams);
						}

						// draw a line
						//OutputDebugString(L"----------------------------------------");
					}
				}
				break;
			case Finished:
				break;
			default:
				break;
			}
		}

#pragma endregion

#pragma region Detour class header file

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// First file : DCL_<MyClass>.h

		// get stream
		std::wstringstream szStream1;

		// header
		szStream1 << "/***************************************************************\n"
			"Aquilinus - 3D Modification Studio\n"
			"Copyright © 2014 Denis Reischl\n\n"
			"File <DCL_";
		szStream1 << szClassName;
		szStream1 << ".h> and\n"
			"Class <DCL_";
		szStream1 << szClassName;
		szStream1 << "> :\n"
			"Copyright © 2014 Denis Reischl\n\n"
			"This code is private and MUST NOT be set public for any\n"
			"reason. This code is intended to be used, changed, compiled\n"
			"and its build published only by Denis Reischl.\n\n"
			"This code is intended to be used by its author,\n"
			"Denis Reischl,\n"
			"for any commercial and non-commercial purpose.\n\n"
			"Following persons are granted to read the full Aquilinus\n"
			"source code for life time :\n"
			"Neil Schneider, Grant Bagwell and Simon Brown.\n"
			"If the original author of Aquilinus, Denis Reischl,\n"
			"discontinues the work on this software, the named persons\n"
			"automatically inherit all rights to continue this work.\n\n"
			"Except where otherwise noted, this work is licensed under :\n"
			"<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.\n"
			"****************************************************************/\n\n";

		// defines + includes + class header
		szStream1 << "#ifndef DCL_" << szClassNameUpper << "_CLASS\n";
		szStream1 << "#define DCL_" << szClassNameUpper << "_CLASS\n\n";
		szStream1 << "#include <d3d" << dwD3DVersion << ".h>\n"; // choose dx version manually here :-S
		szStream1 << "#include " << '"' << "VMT_" << szClassName << ".h" << '"' << "\n";
		szStream1 << "#include " << '"' << "AQU_TransferSite.h" << '"' << "\n\n";
		szStream1 << "/**\n";
		szStream1 << "* " << szClassName << " detour class.\n";
		szStream1 << "***/\n";
		szStream1 << "class DCL_" << szClassName << "\n";
		szStream1 << "{\n";
		szStream1 << "public:\n";
		szStream1 << "DCL_" << szClassName << "(AQU_TransferSite* pcTransferSite);\n";
		szStream1 << "~DCL_ID3D10Device();\n\n";

		// loop through methods
		UINT dwClassTypeIndex = 0;

		// first, output comment string
		szStream1 << "/*** " << aszClassTypes[dwClassTypeIndex] << " public methods ***/\n";
		for (size_t nMethodIndex = 0; nMethodIndex < aszMethodTypes.size(); nMethodIndex++)
		{
			// set new class type index
			if (dwClassTypeIndex < ((UINT)adwClassTypeIndices.size()-1))
			{
				// next index chosen by method index
				UINT dwNextMethodIndex = adwClassTypeIndices[dwClassTypeIndex+1];
				if (dwNextMethodIndex == (UINT)nMethodIndex)
				{
					// increase index
					dwClassTypeIndex++;

					// output comment string
					szStream1 << "/*** " << aszClassTypes[dwClassTypeIndex] << " public methods ***/\n";
				}
			}

			// HRESULT WINAPI QueryInterface                       (ID3D10Device* pcThis, REFIID riid, void **ppvObject);
			szStream1 << aszMethodTypes[nMethodIndex] << " " << aszMethodConvention[nMethodIndex] << " " << aszMethodNames[nMethodIndex] << "(" << szClassName << "* pcThis";

			// debug output
			/*OutputDebugString(L"_____________________Method");
			OutputDebugString(aszClassTypes[dwClassTypeIndex].c_str());
			OutputDebugString(aszMethodNames[nMethodIndex].c_str());*/
			for (size_t nParamIndex = 0; nParamIndex < aaszMethodParameterTypes[nMethodIndex].size(); nParamIndex++)
			{
				// add a separator (,) 
				if (aaszMethodParameterTypes[nMethodIndex][nParamIndex].size()) szStream1 << ", ";

				// add parameter
				szStream1 << aaszMethodParameterTypes[nMethodIndex][nParamIndex] << " " << aaszMethodParameterNames[nMethodIndex][nParamIndex];

				/*OutputDebugString(L"__Param");
				OutputDebugString(aaszMethodParameterTypes[nMethodIndex][nParamIndex].c_str());
				OutputDebugString(aaszMethodParameterNames[nMethodIndex][nParamIndex].c_str());*/
			}
			szStream1 << ");\n";
		}

		szStream1 << "\n/*** DCL_" << aszClassTypes[dwClassTypeIndex] << " public methods ***/\n";
		for (size_t nClassTypeIndex = 0; nClassTypeIndex < aszClassTypes.size(); nClassTypeIndex++)
		{
			std::wstring szUpper(aszClassTypes[nClassTypeIndex]);
			for (auto & c: szUpper) c = toupper(c);
			szStream1 << "HRESULT SetSuperFunctionPointer(VMT_" << szUpper << "::VMT_" << aszClassTypes[nClassTypeIndex] << " eFunc, UINT_PTR dwFunc);\n";
		}
		szStream1 << "HRESULT SetSuperFunctionPointers(PUINT_PTR pVMTable);\n\n";

		szStream1 << "/**\n* The transfer site.\n***/\nAQU_TransferSite* m_pcTransferSite;\n};\n\n#endif\n";

		// output the string
		// OutputDebugString(szStream1.str().c_str());

		// get the filename
		std::wstring szFileName1;
		szFileName1 = L"DCL_";
		szFileName1 += szClassName.c_str();
		szFileName1 += L".h";

		// write the file
		std::wofstream ioFile1;
		ioFile1.open(szFileName1);
		ioFile1.write( szStream1.str().c_str(), szStream1.str().length() );
		ioFile1.close();

#pragma endregion

#pragma region Detour class source code file

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Second file : DCL_<MyClass>.cpp

		// get stream
		std::wstringstream szStream2;

		// header
		szStream2 << "/***************************************************************\n"
			"Aquilinus - 3D Modification Studio\n"
			"Copyright © 2014 Denis Reischl\n\n"
			"File <DCL_";
		szStream2 << szClassName;
		szStream2 << ".cpp> and\n"
			"Class <DCL_";
		szStream2 << szClassName;
		szStream2 << "> :\n"
			"Copyright © 2014 Denis Reischl\n\n"
			"This code is private and MUST NOT be set public for any\n"
			"reason. This code is intended to be used, changed, compiled\n"
			"and its build published only by Denis Reischl.\n\n"
			"This code is intended to be used by its author,\n"
			"Denis Reischl,\n"
			"for any commercial and non-commercial purpose.\n\n"
			"Following persons are granted to read the full Aquilinus\n"
			"source code for life time :\n"
			"Neil Schneider, Grant Bagwell and Simon Brown.\n"
			"If the original author of Aquilinus, Denis Reischl,\n"
			"discontinues the work on this software, the named persons\n"
			"automatically inherit all rights to continue this work.\n\n"
			"Except where otherwise noted, this work is licensed under :\n"
			"<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.\n"
			"****************************************************************/\n\n";

		// defines, includes
		szStream2 << "#ifdef _DEBUG\n#include <windows.h>\n#include <tchar.h>\n#include <sstream>\ntypedef std::basic_stringstream<TCHAR> tstringstream;\n";
		szStream2 << "template<typename T> tstringstream& operator,(tstringstream& tss, T t) { tss << _T(" ") << t; return tss; }\n";
		szStream2 << "#define OUTPUT_DEBUG_STRING_" << szClassName << "(...) ::OutputDebugString((tstringstream(), _T(" << '"' << "***" << '"' << "), __VA_ARGS__, _T(" << '"' << "\n" << '"' << ")).str().c_str());\n";
		szStream2 << "#else\n#define OUTPUT_DEBUG_STRING_" << szClassName << "(...)\n#endif\n";
		szStream2 << "#define AQU_" << szClassNameUpper << "_HEADER________(a) if ((!m_pcTransferSite->m_bForceD3D) && (m_pcTransferSite->m_ppNOD_" << szClassName << "[a]) && (m_pcTransferSite->m_ppNOD_" << szClassName << "[a]->m_cProvoker.m_paInvokers.size() > 0)) {\n";
		szStream2 << "#define AQU_" << szClassNameUpper << "_SET_DATA______(a,b,c) m_pcTransferSite->m_ppNOD_" << szClassName << "[a]->m_paCommanders[b]->m_pOutput = (void*)&c;\n";
		szStream2 << "#define AQU_" << szClassNameUpper << "_PROVOKE_______(a) m_pcTransferSite->m_bForceD3D = true; void* pvRet = m_pcTransferSite->m_ppNOD_" << szClassName << "[a]->Provoke((void*)pcThis, m_pcTransferSite->m_ppaNodes); m_pcTransferSite->m_bForceD3D = false;\n";
		szStream2 << "#define AQU_" << szClassNameUpper << "_REPLACE_METHOD(a,b) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_" << szClassName << "[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ static b hr = (b)*(b*)pvRet; return hr; } }\n";
		szStream2 << "#define AQU_" << szClassNameUpper << "_REPLACE_VOID__(a) if ((*m_pcTransferSite->m_ppaNodes)[m_pcTransferSite->m_ppNOD_" << szClassName << "[a]->m_cProvoker.m_paInvokers[0]->m_lNodeIndex]->m_bReturn){ return; } }\n";

		szStream2 << "#include" << '"' << "DCL_" << szClassName << ".h" << '"' << "\n";
		szStream2 << "#include" << '"' << "DCL_" << szClassName << "_Super.h" << '"' << "\n\n";

		// constructor, destructor
		szStream2 << "#pragma region DCL_" << szClassName << " constructor/destructor\n";
		szStream2 << "/**\n";
		szStream2 << "* Constructor\n";
		szStream2 << "***/\n";
		szStream2 << "DCL_" << szClassName << "::DCL_" << szClassName << "(AQU_TransferSite* pcTransferSite) : \n";
		szStream2 << "m_pcTransferSite(pcTransferSite)\n";
		szStream2 << "{ }\n";

		szStream2 << "/**\n";
		szStream2 << "* Destructor\n";
		szStream2 << "***/\n";
		szStream2 << "DCL_" << szClassName << "::~DCL_" << szClassName << "(){ } }\n";
		szStream2 << "#pragma endregion\n\n";

		// loop through methods
		dwClassTypeIndex = 0;

		// first, output pragma region
		szStream2 << "#pragma region " << aszClassTypes[dwClassTypeIndex] << " methods\n";
		for (size_t nMethodIndex = 0; nMethodIndex < aszMethodTypes.size(); nMethodIndex++)
		{
			// set new class type index
			if (dwClassTypeIndex < ((UINT)adwClassTypeIndices.size()-1))
			{
				// next index chosen by method index
				UINT dwNextMethodIndex = adwClassTypeIndices[dwClassTypeIndex+1];
				if (dwNextMethodIndex == (UINT)nMethodIndex)
				{
					// increase index
					dwClassTypeIndex++;

					// output pragma region
					szStream2 << "#pragma endregion\n\n";
					szStream2 << "#pragma region " << aszClassTypes[dwClassTypeIndex] << " methods\n";
				}
			}
			// get uppercase
			std::wstring szUpper(aszClassTypes[dwClassTypeIndex]);
			for (auto & c: szUpper) c = toupper(c);

			szStream2 << "/**\n";
			szStream2 << "* " <<aszMethodTypes[nMethodIndex] << " " << aszMethodConvention[nMethodIndex] << " " << aszMethodNames[nMethodIndex];
			szStream2 << "\n***/\n";
			szStream2 << aszMethodTypes[nMethodIndex] << " " << aszMethodConvention[nMethodIndex] << " DCL_" << szClassName << "::" << aszMethodNames[nMethodIndex] << "(" << szClassName << "* pcThis";
			// loop throug params, first time
			for (size_t nParamIndex = 0; nParamIndex < aaszMethodParameterTypes[nMethodIndex].size(); nParamIndex++)
			{
				// add a separator (,) 
				if (aaszMethodParameterTypes[nMethodIndex][nParamIndex].size()) szStream2 << ", ";

				// add parameter
				szStream2 << aaszMethodParameterTypes[nMethodIndex][nParamIndex] << " " << aaszMethodParameterNames[nMethodIndex][nParamIndex];
			}
			szStream2 << ")\n{\nOUTPUT_DEBUG_STRING_" << szClassName << "(L" << '"' << szClassName << "::" << aszMethodNames[nMethodIndex] << '"' << ");\n\n// watch DCL_IDirect3DDevice9.cpp for detailed code (without macros)\n";
			szStream2 << "AQU_" << szClassNameUpper << "_HEADER________(VMT_" << szUpper << "::" << aszMethodNames[nMethodIndex] << ");\n";
			int nParamCount = 0;
			// loop throug params, second time
			for (size_t nParamIndex = 0; nParamIndex < aaszMethodParameterTypes[nMethodIndex].size(); nParamIndex++)
			{
				// output only if param present
				if (aaszMethodParameterTypes[nMethodIndex][nParamIndex].size())
				{
					// write line
					szStream2 << "AQU_" << szClassNameUpper << "_SET_DATA______(VMT_" << szUpper << "::" << aszMethodNames[nMethodIndex] << "," << nParamCount << "," << aaszMethodParameterNames[nMethodIndex][nParamIndex] << ");\n";
					// increase param counter
					nParamCount++;
				}
			}
			szStream2 << "AQU_" << szClassNameUpper << "_PROVOKE_______(VMT_" << szUpper << "::" << aszMethodNames[nMethodIndex] << ");\n";
			if (aszMethodTypes[nMethodIndex].compare(L"void") == 0)
				szStream2 << "AQU_" << szClassNameUpper << "_REPLACE_VOID__(VMT_" << szUpper << "::" << aszMethodNames[nMethodIndex] << ");\n\n";
			else
				szStream2 << "AQU_" << szClassNameUpper << "_REPLACE_METHOD(VMT_" << szUpper << "::" << aszMethodNames[nMethodIndex] << ", " << aszMethodTypes[nMethodIndex] << ");\n\n return ";
			szStream2 << "D3D" << dwD3DVersion << "_" << szClassName << "_" << aszMethodNames[nMethodIndex] << "_Super(pcThis";
			// loop throug params, third time
			for (size_t nParamIndex = 0; nParamIndex < aaszMethodParameterTypes[nMethodIndex].size(); nParamIndex++)
			{
				// add a separator (,) 
				if (aaszMethodParameterTypes[nMethodIndex][nParamIndex].size()) szStream2 << ", ";

				// add parameter
				szStream2 << aaszMethodParameterNames[nMethodIndex][nParamIndex];
			}
			szStream2 << ");\n}\n\n";
		}
		szStream2 << "#pragma endregion\n\n";

		// detour class public methods
		szStream2 << "#pragma region DCL_" << szClassName << " public methods\n";

		// loop through class types
		UINT dwMethodIndex = 0;
		UINT dwNextTypeMethodIndex = 0;
		for (size_t nClassTypeIndex = 0; nClassTypeIndex < aszClassTypes.size(); nClassTypeIndex++)
		{
			std::wstring szUpper(aszClassTypes[nClassTypeIndex]);
			for (auto & c: szUpper) c = toupper(c);

			szStream2 << "/**\n* Sets old function pointer by provided index.\n* @param eFunc The function as listed in VMT_" << szClassName << ".\n";
			szStream2 << "* @param dwFunc The address of the super function as retourned by the detour method.\n***/\n";


			szStream2 << "HRESULT DCL_" << szClassName << "::SetSuperFunctionPointer(VMT_" << szUpper << "::VMT_" << aszClassTypes[nClassTypeIndex] << " eFunc, UINT_PTR dwFunc)\n";
			szStream2 << "{\nswitch(eFunc)\n{\n/*** " << aszClassTypes[nClassTypeIndex] << " super methods ***/\n";

			// get the range of methods for that class type
			if (nClassTypeIndex < (aszClassTypes.size()-1))
				dwNextTypeMethodIndex = adwClassTypeIndices[nClassTypeIndex+1];
			else
				dwNextTypeMethodIndex = (UINT)aszMethodTypes.size();

			while (dwMethodIndex < dwNextTypeMethodIndex)
			{
				szStream2 << "case VMT_" << szUpper << "::" << aszMethodNames[dwMethodIndex] << ":*(PUINT_PTR)&D3D" << dwD3DVersion << "_" << szClassName << "_" << aszMethodNames[dwMethodIndex] << "_Super = (UINT_PTR)dwFunc; break;\n";
				dwMethodIndex++;
			}
			szStream2 << "}\n\nreturn S_OK;\n}\n\n";
		}

		// last method
		szStream2 << "/**\n* Sets old function pointers by provided virtual methods table.\n* @param pVMTable Pointer to the Virtual Methods Table.\n***/\n";
		szStream2 << "HRESULT DCL_" << szClassName << "::SetSuperFunctionPointers(PUINT_PTR pVMTable)\n{\nif (!pVMTable) return E_FAIL;\n\n";

		// loop through class types
		dwMethodIndex = 0;
		dwNextTypeMethodIndex = 0;
		for (size_t nClassTypeIndex = 0; nClassTypeIndex < aszClassTypes.size(); nClassTypeIndex++)
		{
			std::wstring szUpper(aszClassTypes[nClassTypeIndex]);
			for (auto & c: szUpper) c = toupper(c);

			szStream2 << "/*** " << aszClassTypes[nClassTypeIndex] << " super methods ***/\n";


			// get the range of methods for that class type
			if (nClassTypeIndex < (aszClassTypes.size()-1))
				dwNextTypeMethodIndex = adwClassTypeIndices[nClassTypeIndex+1];
			else
				dwNextTypeMethodIndex = (UINT)aszMethodTypes.size();

			while (dwMethodIndex < dwNextTypeMethodIndex)
			{
				szStream2 << "*(PUINT_PTR)&D3D" << dwD3DVersion << "_" << szClassName << "_" << aszMethodNames[dwMethodIndex] << "_Super = (UINT_PTR)pVMTable[VMT_" << szUpper << "::"  << aszMethodNames[dwMethodIndex] << "];\n";
				dwMethodIndex++;
			}
		}
		szStream2 << "\n\nreturn S_OK;\n}\n\n#pragma endregion";

		// output the string
		// OutputDebugString(szStream2.str().c_str());

		// get the filename
		std::wstring szFileName2;
		szFileName2 = L"DCL_";
		szFileName2 += szClassName.c_str();
		szFileName2 += L".cpp";

		// write the file
		std::wofstream ioFile2;
		ioFile2.open(szFileName2);
		ioFile2.write( szStream2.str().c_str(), szStream2.str().length() );
		ioFile2.close();

#pragma endregion

#pragma region Detour class super methods file

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// First file : DCL_<MyClass>_Super.h

		// get stream
		std::wstringstream szStream3;

		// header
		szStream3 << "/***************************************************************\n"
			"Aquilinus - 3D Modification Studio\n"
			"Copyright © 2014 Denis Reischl\n\n"
			"File <DCL_";
		szStream3 << szClassName;
		szStream3 << "_Super.h> :\n"
			"Copyright © 2014 Denis Reischl\n\n"
			"This code is private and MUST NOT be set public for any\n"
			"reason. This code is intended to be used, changed, compiled\n"
			"and its build published only by Denis Reischl.\n\n"
			"This code is intended to be used by its author,\n"
			"Denis Reischl,\n"
			"for any commercial and non-commercial purpose.\n\n"
			"Following persons are granted to read the full Aquilinus\n"
			"source code for life time :\n"
			"Neil Schneider, Grant Bagwell and Simon Brown.\n"
			"If the original author of Aquilinus, Denis Reischl,\n"
			"discontinues the work on this software, the named persons\n"
			"automatically inherit all rights to continue this work.\n\n"
			"Except where otherwise noted, this work is licensed under :\n"
			"<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.\n"
			"****************************************************************/\n\n";

		szStream3 << "#ifndef DCL_" << szClassNameUpper << "_SUPER\n";
		szStream3 << "#define DCL_" << szClassNameUpper << "_SUPER\n\n";

		// loop through methods
		dwClassTypeIndex = 0;

		// first, output comment string
		szStream3 << "/*** " << aszClassTypes[dwClassTypeIndex] << " method prototypes ***/\n";
		for (size_t nMethodIndex = 0; nMethodIndex < aszMethodTypes.size(); nMethodIndex++)
		{
			// set new class type index
			if (dwClassTypeIndex < ((UINT)adwClassTypeIndices.size()-1))
			{
				// next index chosen by method index
				UINT dwNextMethodIndex = adwClassTypeIndices[dwClassTypeIndex+1];
				if (dwNextMethodIndex == (UINT)nMethodIndex)
				{
					// increase index
					dwClassTypeIndex++;

					// output comment string
					szStream3 << "\n/*** " << aszClassTypes[dwClassTypeIndex] << " method prototypes ***/\n";
				}
			}

			// typedef HRESULT (WINAPI* D3D10_QueryInterface)                       (ID3D10Device* pDevice, REFIID riid, void **ppvObject);
			szStream3 << "typedef " << aszMethodTypes[nMethodIndex] << " (" << aszMethodConvention[nMethodIndex] << "* D3D" << dwD3DVersion << "_" << aszMethodNames[nMethodIndex] << ") (" << szClassName << "* pcThis";
			for (size_t nParamIndex = 0; nParamIndex < aaszMethodParameterTypes[nMethodIndex].size(); nParamIndex++)
			{
				// add a separator (,) 
				if (aaszMethodParameterTypes[nMethodIndex][nParamIndex].size()) szStream3 << ", ";

				// add parameter
				szStream3 << aaszMethodParameterTypes[nMethodIndex][nParamIndex] << " " << aaszMethodParameterNames[nMethodIndex][nParamIndex];
			}
			szStream3 << ");\n";
		}

		// loop through methods
		dwClassTypeIndex = 0;

		// first, output comment string
		szStream3 << "\n/*** " << aszClassTypes[dwClassTypeIndex] << " super methods ***/\n";
		for (size_t nMethodIndex = 0; nMethodIndex < aszMethodTypes.size(); nMethodIndex++)
		{
			// set new class type index
			if (dwClassTypeIndex < ((UINT)adwClassTypeIndices.size()-1))
			{
				// next index chosen by method index
				UINT dwNextMethodIndex = adwClassTypeIndices[dwClassTypeIndex+1];
				if (dwNextMethodIndex == (UINT)nMethodIndex)
				{
					// increase index
					dwClassTypeIndex++;

					// output comment string
					szStream3 << "\n/*** " << aszClassTypes[dwClassTypeIndex] << " super methods ***/\n";
				}
			}

			// D3D10_QueryInterface                        D3D10_ID3D10Device_QueryInterface_Super;
			szStream3 << "D3D" << dwD3DVersion << "_" << aszMethodNames[nMethodIndex] << "    D3D" << dwD3DVersion << "_" << szClassName << "_" << aszMethodNames[nMethodIndex] << "_Super;\n";
		}

		szStream3 << "\n#endif\n";

		// output the string
		//OutputDebugString(szStream3.str().c_str());

		// get the filename
		std::wstring szFileName3;
		szFileName3 = L"DCL_";
		szFileName3 += szClassName.c_str();
		szFileName3 += L"_Super.h";

		// write the file
		std::wofstream ioFile3;
		ioFile3.open(szFileName3);
		ioFile3.write( szStream3.str().c_str(), szStream3.str().length() );
		ioFile3.close();

#pragma endregion

#pragma region Detour methods

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// First file : DCL_<MyClass>.cpp

		// get stream
		std::wstringstream szStream4;

		// header
		szStream4 << "/***************************************************************\n"
			"Aquilinus - 3D Modification Studio\n"
			"Copyright © 2014 Denis Reischl\n\n"
			"File <DCL_";
		szStream4 << szClassName;
		szStream4 << ".h> and\n"
			"Class <DCL_";
		szStream4 << szClassName;
		szStream4 << "> :\n"
			"Copyright © 2014 Denis Reischl\n\n"
			"This code is private and MUST NOT be set public for any\n"
			"reason. This code is intended to be used, changed, compiled\n"
			"and its build published only by Denis Reischl.\n\n"
			"This code is intended to be used by its author,\n"
			"Denis Reischl,\n"
			"for any commercial and non-commercial purpose.\n\n"
			"Following persons are granted to read the full Aquilinus\n"
			"source code for life time :\n"
			"Neil Schneider, Grant Bagwell and Simon Brown.\n"
			"If the original author of Aquilinus, Denis Reischl,\n"
			"discontinues the work on this software, the named persons\n"
			"automatically inherit all rights to continue this work.\n\n"
			"Except where otherwise noted, this work is licensed under :\n"
			"<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.\n"
			"****************************************************************/\n\n";

		szStream4 << "#include" << '"' << "AQU_Detour.h" << '"' << "\n";

		szStream4 << "#ifndef DMT_" << szClassNameUpper << "_TABLE\n";
		szStream4 << "#define DMT_" << szClassNameUpper << "_TABLE\n\n";

		// loop through methods
		dwClassTypeIndex = 0;

		// first, output comment string
		szStream4 << "/*** " << aszClassTypes[dwClassTypeIndex] << " methods ***/\n";
		for (size_t nMethodIndex = 0; nMethodIndex < aszMethodTypes.size(); nMethodIndex++)
		{
			// set new class type index
			if (dwClassTypeIndex < ((UINT)adwClassTypeIndices.size()-1))
			{
				// next index chosen by method index
				UINT dwNextMethodIndex = adwClassTypeIndices[dwClassTypeIndex+1];
				if (dwNextMethodIndex == (UINT)nMethodIndex)
				{
					// increase index
					dwClassTypeIndex++;

					// output comment string
					szStream4 << "\n/*** " << aszClassTypes[dwClassTypeIndex] << " methods ***/\n";
				}
			}

			// HRESULT WINAPI D3D9_IDirect3DDevice9_QueryInterface(IDirect3DDevice9* pDevice, REFIID riid, void** ppvObj) { return pDCL_IDirect3DDevice9->QueryInterface(pDevice, riid, ppvObj);}
			szStream4 << aszMethodTypes[nMethodIndex] << " " << aszMethodConvention[nMethodIndex] << " D3D" << dwD3DVersion << "_" << szClassName << "_" << aszMethodNames[nMethodIndex] << "(" << szClassName << "* pcThis";
			for (size_t nParamIndex = 0; nParamIndex < aaszMethodParameterTypes[nMethodIndex].size(); nParamIndex++)
			{
				// add a separator (,) 
				if (aaszMethodParameterTypes[nMethodIndex][nParamIndex].size()) szStream4 << ", ";

				// add parameter
				szStream4 << aaszMethodParameterTypes[nMethodIndex][nParamIndex] << " " << aaszMethodParameterNames[nMethodIndex][nParamIndex];
			}
			szStream4 << ") { return pDCL_" << szClassName << "->" << aszMethodNames[nMethodIndex] << "(pcThis";
			for (size_t nParamIndex = 0; nParamIndex < aaszMethodParameterTypes[nMethodIndex].size(); nParamIndex++)
			{
				// add a separator (,) 
				if (aaszMethodParameterTypes[nMethodIndex][nParamIndex].size()) szStream4 << ", ";

				// add parameter
				szStream4 << aaszMethodParameterNames[nMethodIndex][nParamIndex];
			}
			szStream4 << ");}\n"; 
		}

		szStream4 << "\n\n/**\n* Overrides the Virtual Methods Table of " << szClassName << "\n***/\nvoid Override_D3D" << dwD3DVersion << "_" << szClassName << "_VMTable()\n{\n";

		// loop through methods
		dwClassTypeIndex = 0;

		// first, output comment string
		szStream4 << "/*** " << aszClassTypes[dwClassTypeIndex] << " super methods ***/\n";
		for (size_t nMethodIndex = 0; nMethodIndex < aszMethodTypes.size(); nMethodIndex++)
		{
			// set new class type index
			if (dwClassTypeIndex < ((UINT)adwClassTypeIndices.size()-1))
			{
				// next index chosen by method index
				UINT dwNextMethodIndex = adwClassTypeIndices[dwClassTypeIndex+1];
				if (dwNextMethodIndex == (UINT)nMethodIndex)
				{
					// increase index
					dwClassTypeIndex++;

					// output comment string
					szStream4 << "\n/*** " << aszClassTypes[dwClassTypeIndex] << " super methods ***/\n";
				}
			}

			// get uppercase class type
			std::wstring szUpper(aszClassTypes[dwClassTypeIndex]);
			for (auto & c: szUpper) c = toupper(c);

			//OverrideVTable(&D3D9_IDirect3DDevice9_VMTable[VMT_IUNKNOWN::QueryInterface],                      (UINT_PTR)D3D9_IDirect3DDevice9_QueryInterface);
			szStream4 << "OverrideVTable(&D3D" << dwD3DVersion << "_" << szClassName << "_VMTable[VMT_" << szUpper << "::" << aszMethodNames[nMethodIndex] << "], (UINT_PTR)D3D" << dwD3DVersion << "_" << szClassName << "_" << aszMethodNames[nMethodIndex] << ");\n";
		}
		szStream4 << "}\n";

		szStream4 << "\n/**\n* Creates a Virtual Methods Table array of " << szClassName << "\n***/\nvoid Generate_D3D" << dwD3DVersion << "_" << szClassName << "_VMTable_Array()\n{\n";

		// loop through methods
		dwClassTypeIndex = 0;

		// first, output comment string
		szStream4 << "/*** " << aszClassTypes[dwClassTypeIndex] << " super methods ***/\n";
		for (size_t nMethodIndex = 0; nMethodIndex < aszMethodTypes.size(); nMethodIndex++)
		{
			// set new class type index
			if (dwClassTypeIndex < ((UINT)adwClassTypeIndices.size()-1))
			{
				// next index chosen by method index
				UINT dwNextMethodIndex = adwClassTypeIndices[dwClassTypeIndex+1];
				if (dwNextMethodIndex == (UINT)nMethodIndex)
				{
					// increase index
					dwClassTypeIndex++;

					// output comment string
					szStream4 << "\n/*** " << aszClassTypes[dwClassTypeIndex] << " super methods ***/\n";
				}
			}

			// get uppercase class type
			std::wstring szUpper(aszClassTypes[dwClassTypeIndex]);
			for (auto & c: szUpper) c = toupper(c);

			//anD3D9_IDirect3DDevice9_VMTable[VMT_IUNKNOWN::QueryInterface] =                      (UINT_PTR)D3D9_IDirect3DDevice9_QueryInterface;
			szStream4 << "anD3D" << dwD3DVersion << "_" << szClassName << "_VMTable[VMT_" << szUpper << "::" << aszMethodNames[nMethodIndex] << "] =  (UINT_PTR)D3D" << dwD3DVersion << "_" << szClassName << "_" << aszMethodNames[nMethodIndex] << ";\n";
		}
		szStream4 << "}\n";

		szStream4 << "\n/**\n* Overrides the Virtual Methods Table of " << szClassName << " using a generated array.\n***/\nvoid Override_D3D" << dwD3DVersion << "_" << szClassName << "_VMTable_by_Array()\n{\n";
		szStream4 << "OverrideFullVTable(&D3D" << dwD3DVersion << "_" << szClassName << "_VMTable[0],\n";
		szStream4 << "(void*)&anD3D" << dwD3DVersion << "_" << szClassName << "_VMTable[0],\n";
		szStream4 << "(D3D" << dwD3DVersion << "_" << szClassNameUpper << "_METHODS_NUMBER)*sizeof(DWORD)+1);\n";
		szStream4 << "}\n";

		szStream4 << "\n/**\n* Overrides the Virtual Methods Table of " << szClassName << "\n***/\nvoid Detour_D3D" << dwD3DVersion << "_" << szClassName << "VMTable()\n{\n";

		// loop through methods
		dwClassTypeIndex = 0;

		// first, output comment string
		szStream4 << "/*** " << aszClassTypes[dwClassTypeIndex] << " super methods ***/\n";
		for (size_t nMethodIndex = 0; nMethodIndex < aszMethodTypes.size(); nMethodIndex++)
		{
			// set new class type index
			if (dwClassTypeIndex < ((UINT)adwClassTypeIndices.size()-1))
			{
				// next index chosen by method index
				UINT dwNextMethodIndex = adwClassTypeIndices[dwClassTypeIndex+1];
				if (dwNextMethodIndex == (UINT)nMethodIndex)
				{
					// increase index
					dwClassTypeIndex++;

					// output comment string
					szStream4 << "\n/*** " << aszClassTypes[dwClassTypeIndex] << " super methods ***/\n";
				}
			}

			// get uppercase class type
			std::wstring szUpper(aszClassTypes[dwClassTypeIndex]);
			for (auto & c: szUpper) c = toupper(c);

			//pDCL_IDirect3DDevice9->SetSuperFunctionPointer(VMT_IUNKNOWN::QueryInterface,                      (UINT_PTR)DetourFunc((PBYTE)D3D9_IDirect3DDevice9_VMTable[VMT_IUNKNOWN::QueryInterface],                      (PBYTE)D3D9_IDirect3DDevice9_QueryInterface,              JMP32_SZ));
			szStream4 << "pDCL_" << szClassName << "->SetSuperFunctionPointer(VMT_" << szUpper << "::" << aszMethodNames[nMethodIndex] << ",  (UINT_PTR)DetourFunc((PBYTE)D3D"<< dwD3DVersion << "_" << szClassName << "_VMTable[VMT_" << szUpper << "::" << aszMethodNames[nMethodIndex] << "],  (PBYTE)D3D"<< dwD3DVersion << "_" << szClassName << "_" << aszMethodNames[nMethodIndex] << ", JMP32_SZ));\n";
		}
		szStream4 << "}\n\n";

		// get the filename
		std::wstring szFileName4;
		szFileName4 = L"DMT_";
		szFileName4 += szClassName.c_str();
		szFileName4 += L".h";

		// write the file
		std::wofstream ioFile4;
		ioFile4.open(szFileName4);
		ioFile4.write( szStream4.str().c_str(), szStream4.str().length() );
		ioFile4.close();

#pragma endregion

#pragma region Nodes

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// Second file : DCL_<MyClass>.cpp

		// get stream
		std::wstringstream szStream5;

		// header
		szStream5 << "/***************************************************************\n"
			"Aquilinus - 3D Modification Studio\n"
			"Copyright © 2014 Denis Reischl\n\n"
			"File <NOD_";
		szStream5 << szClassName;
		szStream5 << ".h> :\n"
			"Copyright © 2014 Denis Reischl\n\n"
			"This code is private and MUST NOT be set public for any\n"
			"reason. This code is intended to be used, changed, compiled\n"
			"and its build published only by Denis Reischl.\n\n"
			"This code is intended to be used by its author,\n"
			"Denis Reischl,\n"
			"for any commercial and non-commercial purpose.\n\n"
			"Following persons are granted to read the full Aquilinus\n"
			"source code for life time :\n"
			"Neil Schneider, Grant Bagwell and Simon Brown.\n"
			"If the original author of Aquilinus, Denis Reischl,\n"
			"discontinues the work on this software, the named persons\n"
			"automatically inherit all rights to continue this work.\n\n"
			"Except where otherwise noted, this work is licensed under :\n"
			"<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.\n"
			"****************************************************************/\n\n";

		// defines, includes
		szStream5 << "#ifndef NOD_" << szClassNameUpper << "\n";
		szStream5 << "#define NOD_" << szClassNameUpper << "\n\n";

		szStream5 << "#include" << '"' << "VMT_" << szClassName << ".h" << '"' << "\n\n";

		// loop through methods
		dwClassTypeIndex = 0;

		// first, output pragma region
		szStream5 << "#pragma region " << aszClassTypes[dwClassTypeIndex] << " methods\n";
		for (size_t nMethodIndex = 0; nMethodIndex < aszMethodTypes.size(); nMethodIndex++)
		{
			// set new class type index
			if (dwClassTypeIndex < ((UINT)adwClassTypeIndices.size()-1))
			{
				// next index chosen by method index
				UINT dwNextMethodIndex = adwClassTypeIndices[dwClassTypeIndex+1];
				if (dwNextMethodIndex == (UINT)nMethodIndex)
				{
					// increase index
					dwClassTypeIndex++;

					// output pragma region
					szStream5 << "#pragma endregion\n\n";
					szStream5 << "#pragma region " << aszClassTypes[dwClassTypeIndex] << " methods\n";
				}
			}
			// get uppercase
			std::wstring szUpper(aszClassTypes[dwClassTypeIndex]);
			for (auto & c: szUpper) c = toupper(c);

			szStream5 << "/**\n";
			szStream5 << "* " <<aszMethodTypes[nMethodIndex] << " " << aszMethodConvention[nMethodIndex] << " " << aszMethodNames[nMethodIndex];
			szStream5 << "\n***/\n";
			//AQU_D3D_NODE_HEADER___(NOD_IDirect3DDevice9_QueryInterface, 280, 100);
			//AQU_D3D_NODE_TITLE____(L"IDirect3DDevice9::QueryInterface");
			//AQU_D3D_NODE_COMMANDER(AQU_REFIID, L"riid");
			//AQU_D3D_NODE_COMMANDER(AQU_PPNT_VOID, L"ppvObj");
			//AQU_D3D_NODE_METHOD___(DirectX_9_0, ITA_D3D9INTERFACES::ITA_D3D9Interfaces::IDirect3DDevice9, VMT_IUNKNOWN::QueryInterface);
			//AQU_D3D_NODE_FOOTER___;
			szStream5 << "AQU_D3D_NODE_HEADER___(NOD_" << szClassName << "_" << aszMethodNames[nMethodIndex] << ", 320, 100);\n";
			szStream5 << "AQU_D3D_NODE_TITLE____(L" << '"' << szClassName << "::" << aszMethodNames[nMethodIndex] << '"' << ");\n";
			int nParamCount = 0;
			// loop throug params, second time
			for (size_t nParamIndex = 0; nParamIndex < aaszMethodParameterTypes[nMethodIndex].size(); nParamIndex++)
			{
				// output only if param present
				if (aaszMethodParameterTypes[nMethodIndex][nParamIndex].size())
				{
					// get uppercase class type
					std::wstring szUpperType(aaszMethodParameterTypes[nMethodIndex][nParamIndex]);
					for (auto & c: szUpperType) c = toupper(c);

					// get Aquilinus pointer string out of *
					std::wstring szPointerString;
					if (szUpperType.find(L"**") != std::string::npos)
						szPointerString = L"PPNT_";
					else if (szUpperType.find(L"*") != std::string::npos)
						szPointerString = L"PNT_";

					// now, erase all * from the string
					while (szUpperType[szUpperType.length()-1] == '*') szUpperType.erase(szUpperType.length()-1,1);

					// write line
					szStream5 << "AQU_D3D_NODE_COMMANDER(AQU_" << szPointerString << szUpperType << ", L" << '"' <<  aaszMethodParameterNames[nMethodIndex][nParamIndex] << '"' << ");\n";

					// increase param counter
					nParamCount++;
				}
			}
			szStream5 << "AQU_D3D_NODE_METHOD___(DirectX_" << dwD3DVersion << "_0, ITA_D3D" << dwD3DVersion << "INTERFACES::ITA_D3D" << dwD3DVersion << "Interfaces::" << szClassName << ", VMT_" << szUpper << "::" << aszMethodNames[nMethodIndex] << ");\n";
			szStream5 << "AQU_D3D_NODE_FOOTER___;\n\n";
		}
		szStream5 << "#pragma endregion\n\n";

		szStream5 << "#pragma region " << szClassName << " node provider method\n\n";
		szStream5 << "/**\n* Node provider class.\n***/\nclass NOD_" << szClassName << "\n{\npublic:\n/**\n* Provides " << szClassName << " node by index.\n***/\nNOD_Basic* Get_" << szClassName << "Node(UINT dwIndex, LONG nX, LONG nY)\n";
		szStream5 << "{\nif (dwIndex > (UINT)VMT_" << szClassNameUpper << "::VMT_" << szClassName << "::" << aszMethodNames[aszMethodNames.size()-1] << ") return nullptr;\n";
		szStream5 << "if (dwIndex <= (UINT)VMT_IUNKNOWN::Release) return Get_" << szClassName << "_Node((VMT_IUNKNOWN::VMT_IUnknown)dwIndex, nX, nY);\n";
		szStream5 << "return Get_" << szClassName << "_Node((VMT_" << szClassNameUpper << "::VMT_" << szClassName << ")dwIndex, nX, nY);\n}\n\n";

		// loop through class types
		dwMethodIndex = 0;
		dwNextTypeMethodIndex = 0;
		for (size_t nClassTypeIndex = 0; nClassTypeIndex < aszClassTypes.size(); nClassTypeIndex++)
		{
			std::wstring szUpper(aszClassTypes[nClassTypeIndex]);
			for (auto & c: szUpper) c = toupper(c);


			szStream5 << "/**\n* Provides " << aszClassTypes[nClassTypeIndex] << " node by enumeration.\n***/\n";
			szStream5 << "NOD_Basic* Get_" << szClassName << "_Node(VMT_" << szUpper << "::VMT_" << aszClassTypes[nClassTypeIndex] << " eMethod, LONG nX, LONG nY)\n{\nswitch (eMethod)\n{\n";

			// get the range of methods for that class type
			if (nClassTypeIndex < (aszClassTypes.size()-1))
				dwNextTypeMethodIndex = adwClassTypeIndices[nClassTypeIndex+1];
			else
				dwNextTypeMethodIndex = (UINT)aszMethodTypes.size();

			while (dwMethodIndex < dwNextTypeMethodIndex)
			{
				/*case VMT_IUNKNOWN::QueryInterface:
				return (NOD_Basic*)new NOD_IDirect3DDevice9_QueryInterface(nX, nY);
				break;*/
				szStream5 << "case VMT_" << szUpper << "::VMT_" << szClassName << "::" << aszMethodNames[dwMethodIndex] << ":\nreturn (NOD_Basic*)new NOD_" << szClassName << "_"  << aszMethodNames[dwMethodIndex] << "(nX, nY);\nbreak;\n";
				dwMethodIndex++;
			}
			szStream5 << "default:\nbreak;\n}\n\nreturn nullptr;\n}\n\n";
		}
		szStream5 << "};\n#pragma endregion\n\n#endif\n";

		// output the string
		// OutputDebugString(szStream5.str().c_str());

		// get the filename
		std::wstring szFileName5;
		szFileName5 = L"NOD_";
		szFileName5 += szClassName.c_str();
		szFileName5 += L".h";

		// write the file
		std::wofstream ioFile5;
		ioFile5.open(szFileName5);
		ioFile5.write( szStream5.str().c_str(), szStream5.str().length() );
		ioFile5.close();

#pragma endregion

#pragma region Virtual methods table enumeration

		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

		// First file : VMT_<MyClass>.h

		// get stream
		std::wstringstream szStream6;

		// header
		szStream6 << "/***************************************************************\n"
			"Aquilinus - 3D Modification Studio\n"
			"Copyright © 2014 Denis Reischl\n\n"
			"File <VMT_";
		szStream6 << szClassName;
		szStream6 << ".h> :\n"
			"Copyright © 2014 Denis Reischl\n\n"
			"This code is private and MUST NOT be set public for any\n"
			"reason. This code is intended to be used, changed, compiled\n"
			"and its build published only by Denis Reischl.\n\n"
			"This code is intended to be used by its author,\n"
			"Denis Reischl,\n"
			"for any commercial and non-commercial purpose.\n\n"
			"Following persons are granted to read the full Aquilinus\n"
			"source code for life time :\n"
			"Neil Schneider, Grant Bagwell and Simon Brown.\n"
			"If the original author of Aquilinus, Denis Reischl,\n"
			"discontinues the work on this software, the named persons\n"
			"automatically inherit all rights to continue this work.\n\n"
			"Except where otherwise noted, this work is licensed under :\n"
			"<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.\n"
			"****************************************************************/\n\n";

		// get the filename
		std::wstring szFileName6;
		szFileName6 = L"VMT_";
		szFileName6 += szClassName.c_str();
		szFileName6 += L".h";

		// defines, includes
		szStream6 << "#ifndef VMT_" << szClassNameUpper << "_ENUM\n";
		szStream6 << "#define VMT_" << szClassNameUpper << "_ENUM\n\n";

		for (size_t nClassTypeIndex = 0; nClassTypeIndex < aszClassTypes.size()-1; nClassTypeIndex++)
		{
			szStream6 << "#include" << '"' << "VMT_" << aszClassTypes[nClassTypeIndex] << ".h" << '"' << "\n";
		}

		// namespace
		szStream6 << "\nnamespace VMT_" << szClassNameUpper << "\n{\n";

		// enumeration
		szStream6 << "/**\n" << "* Virtual Methods Table for " << szClassName << "\n***/\n";
		szStream6 << "enum VMT_" << szClassName << "\n{\n";
		szStream6 << "/*** " << szClassName << " methods ***/\n";

		// methods
		for (size_t nMethodIndex = 0; nMethodIndex < aszMethodNames.size(); nMethodIndex++)
		{
			if (nMethodIndex >= adwClassTypeIndices[adwClassTypeIndices.size()-1])
				szStream6 << aszMethodNames[nMethodIndex] << " = " << nMethodIndex << ",\n";
		}
		szStream6 << "};\n}\n#endif\n";

		// write the file
		std::wofstream ioFile6;
		ioFile6.open(szFileName6);
		ioFile6.write( szStream6.str().c_str(), szStream6.str().length() );
		ioFile6.close();

#pragma endregion

	}

	return 0;
}

