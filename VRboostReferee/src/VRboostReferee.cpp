/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <VRboostReferee.cpp> :
Copyright (C) 2013 Denis Reischl

Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown

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


#include "VRboostReferee.h"

using namespace pugi;
using namespace VRBoost;

/**
* Returns the enumeration value of the string rule, -1 if not found.
***/
int VRboost_RuleType(std::string ruleName)
{
	// be careful and avoid shorter strings first containing the same value since we use "find" and not "compare"
	// for some reason
	if (ruleName.find("FloatDoNothing") != std::string::npos) return FloatModificationTypes::FloatDoNothing;
	else if (ruleName.find("FloatSimpleApply") != std::string::npos) return FloatModificationTypes::FloatSimpleApply;
	else if (ruleName.find("FloatSimpleNegativeApply") != std::string::npos) return FloatModificationTypes::FloatSimpleNegativeApply;
	else if (ruleName.find("FloatScale") != std::string::npos) return FloatModificationTypes::FloatScale;
	else if (ruleName.find("FloatToDoubleScale") != std::string::npos) return FloatModificationTypes::FloatToDoubleScale;
	else if (ruleName.find("FloatToBoolScale") != std::string::npos) return FloatModificationTypes::FloatToBoolScale;
	else if (ruleName.find("FloatToByteScale") != std::string::npos) return FloatModificationTypes::FloatToByteScale;
	else if (ruleName.find("FloatToShortScale") != std::string::npos) return FloatModificationTypes::FloatToShortScale;
	else if (ruleName.find("FloatToWordScale") != std::string::npos) return FloatModificationTypes::FloatToWordScale;
	else if (ruleName.find("FloatToIntegerScale") != std::string::npos) return FloatModificationTypes::FloatToIntegerScale;
	else if (ruleName.find("FloatToDWordScale") != std::string::npos) return FloatModificationTypes::FloatToDWordScale;
	else if (ruleName.find("FloatToLongScale") != std::string::npos) return FloatModificationTypes::FloatToLongScale;        
	else if (ruleName.find("FloatToQWordScale") != std::string::npos) return FloatModificationTypes::FloatToQWordScale;
	else if (ruleName.find("FloatDegreeApply") != std::string::npos) return FloatModificationTypes::FloatDegreeApply;
	else if (ruleName.find("FloatDegreeCompass") != std::string::npos) return FloatModificationTypes::FloatDegreeCompass;
	else if (ruleName.find("FloatDegreeNegativeApply") != std::string::npos) return FloatModificationTypes::FloatDegreeNegativeApply;
	else if (ruleName.find("FloatDegreeNegativeCompass") != std::string::npos) return FloatModificationTypes::FloatDegreeNegativeCompass;
	else if (ruleName.find("FloatGaussianCompass") != std::string::npos) return FloatModificationTypes::FloatGaussianCompass;
	else if (ruleName.find("FloatUnrealCompass") != std::string::npos) return FloatModificationTypes::FloatUnrealCompass;
	else if (ruleName.find("FloatUnrealAxis") != std::string::npos) return FloatModificationTypes::FloatUnrealAxis;
	else if (ruleName.find("FloatUnrealNegativeAxis") != std::string::npos) return FloatModificationTypes::FloatUnrealNegativeAxis;
	else return -1;
}

/**
* Returns the enumeration value of the string axis, -1 if not found.
***/
int VRboost_Axis(std::string axisName)
{
	if (axisName.find("TrackerYaw") != std::string::npos) return VRboostAxis::TrackerYaw;
	else if (axisName.find("TrackerPitch") != std::string::npos) return VRboostAxis::TrackerPitch;
	else if (axisName.find("TrackerRoll") != std::string::npos) return VRboostAxis::TrackerRoll;
	else if (axisName.find("Zero") != std::string::npos) return VRboostAxis::Zero;
	else if (axisName.find("One") != std::string::npos) return VRboostAxis::One;
	else if (axisName.find("WorldFOV") != std::string::npos) return VRboostAxis::WorldFOV;
	else if (axisName.find("PlayerFOV") != std::string::npos) return VRboostAxis::PlayerFOV;
	else if (axisName.find("FarPlaneFOV") != std::string::npos) return VRboostAxis::FarPlaneFOV;
	else if (axisName.find("CameraTranslateX") != std::string::npos) return VRboostAxis::CameraTranslateX;
	else if (axisName.find("CameraTranslateY") != std::string::npos) return VRboostAxis::CameraTranslateY;
	else if (axisName.find("CameraTranslateZ") != std::string::npos) return VRboostAxis::CameraTranslateZ;
	else if (axisName.find("CameraDistance") != std::string::npos) return VRboostAxis::CameraDistance;
	else if (axisName.find("CameraZoom") != std::string::npos) return VRboostAxis::CameraZoom;
	else if (axisName.find("CameraHorizonAdjustment") != std::string::npos) return VRboostAxis::CameraHorizonAdjustment;
	else if (axisName.find("ConstantValue1") != std::string::npos) return VRboostAxis::ConstantValue1;
	else if (axisName.find("ConstantValue2") != std::string::npos) return VRboostAxis::ConstantValue2;
	else if (axisName.find("ConstantValue3") != std::string::npos) return VRboostAxis::ConstantValue3;
	else return -1;
}

/**
* Project template for a simple tool to create VRboost rules
***/
int _tmain(int argc, char* argv[])
{
	// explicit VRboost dll import
	HMODULE hmVRboost = LoadLibrary("..//..//bin//VRboost.dll");

	// get VRboost methods
	if (hmVRboost != NULL)
	{
		// get methods explicit
		m_pVRboost_LoadMemoryRules = (LPVRBOOST_LoadMemoryRules)GetProcAddress(hmVRboost, "VRboost_LoadMemoryRules");
		m_pVRboost_SaveMemoryRules = (LPVRBOOST_SaveMemoryRules)GetProcAddress(hmVRboost, "VRboost_SaveMemoryRules");
		m_pVRboost_CreateFloatMemoryRule = (LPVRBOOST_CreateFloatMemoryRule)GetProcAddress(hmVRboost, "VRboost_CreateFloatMemoryRule");
		m_pVRboost_SetProcess = (LPVRBOOST_SetProcess)GetProcAddress(hmVRboost, "VRboost_SetProcess");
		m_pVRboost_ReleaseAllMemoryRules = (LPVRBOOST_ReleaseAllMemoryRules)GetProcAddress(hmVRboost, "VRboost_ReleaseAllMemoryRules");
		m_pVRboost_ApplyMemoryRules = (LPVRBOOST_ApplyMemoryRules)GetProcAddress(hmVRboost, "VRboost_ApplyMemoryRules");
		if ((!m_pVRboost_LoadMemoryRules) || 
			(!m_pVRboost_SaveMemoryRules) || 
			(!m_pVRboost_CreateFloatMemoryRule) || 
			(!m_pVRboost_SetProcess) || 
			(!m_pVRboost_ReleaseAllMemoryRules) || 
			(!m_pVRboost_ApplyMemoryRules))
		{
			OutputDebugString("VRboost.dll not found in specified directory (..//..//bin//VRboost.dll)");
			hmVRboost = NULL;
			FreeLibrary(hmVRboost);
			return -1;
		}
		else
		{
			OutputDebugString("Success loading VRboost methods.");
		}
	} else return -1;

	if ( argc != 2 ) 
	{
		OutputDebugString("VRboostReferee <filename>");
	}
	else 
	{
		// VRboost rule simple data structure
		DWORD rule = 0;
		DWORD axis = 0;
		DWORD offsets[] = { 0, 0x0, 0x0, 0x0, 0x0, 0x0};
		DWORD cOffsets1[] = { 0, 0x0, 0x0, 0x0, 0x0, 0x0};
		DWORD cOffsets2[] = { 0, 0x0, 0x0, 0x0, 0x0, 0x0};
		DWORD address[] = { 0x0, 0x0, 0x0 };
		D3DXVECTOR4 values = D3DXVECTOR4(0,0,0,0);
		int valuenumber = 0;

		// get file
		std::string file = std::string(argv[1]);
		OutputDebugString(file.c_str());

		if (file.size()<4)
		{
			OutputDebugString("Invalid parameter 1 : should be .CT filename");
			return -1;
		}

		// get extension
		std::string ext = file.substr(file.size()-3);
		OutputDebugString(ext.c_str());

		// verify ext
		if ((ext.compare(".CT") != 0) && (ext.compare(".ct") != 0))
		{
			OutputDebugString("Wrong extension ! (should be .CT)");
			return -1;
		}

		// create output filename for VRboost directory path
		char path[1024];
		GetModuleFileName(NULL, path, 1024);
		std::string pathStr = std::string(path);
		pathStr = pathStr.substr(0, pathStr.find_last_of("\\/")+1);
		std::string process = file.substr(0, file.size()-3);
		process = process.substr(process.find_last_of("\\/")+1);
		std::string outfile = pathStr + process +".MTBS";
		process += ".exe";
		OutputDebugString(outfile.c_str());
		OutputDebugString(process.c_str());
		std::string module = std::string(process);

		// set the process exe name (we go to VRboost safe mode first)
		m_pVRboost_ReleaseAllMemoryRules();
		m_pVRboost_SetProcess("SAFE_MODE", "SAFE_MODE");
		m_pVRboost_SetProcess(process, module);

		// create pugi xml file
		xml_document ctFile;
		xml_parse_result resultCT = ctFile.load_file(file.c_str());

		if(resultCT.status == status_ok)
		{
			xml_node cheatTable = ctFile.child("CheatTable");
			xml_node cheatEntries = cheatTable.child("CheatEntries");

			for (pugi::xml_node entry = cheatEntries.first_child(); entry; entry = entry.next_sibling())
			{
				//OutputDebugString(entry.name());
				if (strcmp(entry.name(), "CheatEntry") == 0)
				{
					// get the id .. need that ?
					UINT id = atoi(entry.child("ID").child_value());

					// get the description...parse to possible rule, possible value, possible
					std::string desc = std::string(entry.child("Description").child_value());
					int rule_type = VRboost_RuleType(desc);
					float value = (float)atof(desc.c_str());
					int axis_type = VRboost_Axis(desc);

					// get the address
					DWORD dwAddress = 0;
					std::string addr = std::string(entry.child("Address").child_value());
					sscanf_s(addr.substr(addr.find_last_of("+")+1).c_str(), "%x", &dwAddress);

					// set process respective address string process
					if (addr.find('"')!=std::string::npos)
					{
						module = addr.substr(1, addr.find_last_of('"')-1);
						OutputDebugString(module.c_str());
						m_pVRboost_SetProcess(process, module);
					}
					else
					{
						// no base address - static pointers
						module = std::string(process);
						OutputDebugString(module.c_str());
						m_pVRboost_SetProcess(process, module);
					}

					// get the offsets - note that offsets are stored from last to first !!
					pugi::xml_node offsets_node = entry.child("Offsets");
					DWORD dwOffset[5];
					BYTE index = 0;
					for (pugi::xml_node offset_node = offsets_node.first_child(); offset_node; offset_node = offset_node.next_sibling())
					{
						sscanf_s(offset_node.child_value(), "%x", &dwOffset[index]);
						index++;
					}

					if (rule_type>=0)
					{
						OutputDebugString("rule started....");
						// valid new rule, first erase data structure
						ZeroMemory(&offsets[0], 6*sizeof(DWORD));
						ZeroMemory(&cOffsets1[0], 6*sizeof(DWORD));
						ZeroMemory(&cOffsets2[0], 6*sizeof(DWORD));
						ZeroMemory(&address[0], 3*sizeof(float));
						values = D3DXVECTOR4(0,0,0,0);
						valuenumber = 0;

						// set address, then offsets
						address[0] = dwAddress;

						// offsets start from last to first, offset[0] = number of offsets
						offsets[0] = (DWORD)index;
						for (int i = 0; i < index; i++)
							offsets[i+1] = dwOffset[index-i-1];

						// set rule type
						rule = rule_type;
					}
					else if (axis_type>=0)
					{
						// valid new axis, set data and create rule

						// set address, then offsets
						address[2] = dwAddress;

						// offsets start from last to first, offset[0] = number of offsets
						cOffsets2[0] = (DWORD)index;
						for (int i = 0; i < index; i++)
							cOffsets2[i+1] = dwOffset[index-i-1];

						// set axis type
						axis = axis_type;

						char buf[256];
						sprintf_s(buf, 256, "rule %u", rule);
						OutputDebugString(buf);
						for (int i = 0; i < 3; i++)
						{
							sprintf_s(buf, 256, "addr %x", address[i]);
							OutputDebugString(buf);
						}
						for (int i = 0; i < 6; i++)
						{
							sprintf_s(buf, 256, "off %x", offsets[i]);
							OutputDebugString(buf);
						}
						for (int i = 0; i < 6; i++)
						{
							sprintf_s(buf, 256, "off %x", cOffsets1[i]);
							OutputDebugString(buf);
						}
						for (int i = 0; i < 6; i++)
						{
							sprintf_s(buf, 256, "off %x", cOffsets2[i]);
							OutputDebugString(buf);
						}

						sprintf_s(buf, 256, "value %g", values.x);
						OutputDebugString(buf);
						sprintf_s(buf, 256, "value %g", values.y);
						OutputDebugString(buf);
						sprintf_s(buf, 256, "value %g", values.z);
						OutputDebugString(buf);
						sprintf_s(buf, 256, "value %g", values.w);
						OutputDebugString(buf);

						// create skyrim shader rules here
						m_pVRboost_CreateFloatMemoryRule(rule, axis, values, address[0], offsets, 0x00001000, 0xA0000000, address[1], cOffsets1, 0, address[2], cOffsets2, 0);

					}
					else
					{
						// data, parse pointer to mid comparisation data pointer

						// set address, then offsets
						address[1] = dwAddress;

						// offsets start from last to first, offset[0] = number of offsets
						cOffsets1[0] = (DWORD)index;
						for (int i = 0; i < index; i++)
							cOffsets1[i+1] = dwOffset[index-i-1];

						// set vector data
						if (valuenumber==0)
							values.x = value;
						else if (valuenumber==1)
							values.y = value;
						else if (valuenumber==2)
							values.z = value;
						else if (valuenumber==3)
							values.w = value;

						valuenumber++;
					}
				}

			}

			// save VRboost rules
			OutputDebugString("Save VRboost rules...");
			m_pVRboost_SaveMemoryRules(outfile);

			return true;
		}
		else 
		{
			OutputDebugString("Could not load File : ");
			OutputDebugString(file.c_str());
		}
	}
	return 0;
}

