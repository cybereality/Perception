/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2013 Chris Drain

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


#include "ShaderConstantTracker.h"
#include <utility>
#include <assert.h>




ShaderConstantTracker::ShaderConstantTracker(IDirect3DDevice9* pActualDevice) :
		m_pActualDevice(pActualDevice),
		m_floats(),
		m_ints(),
		m_bools(),
		m_bClearPending(false)
{
	m_pActualDevice->AddRef();
}

ShaderConstantTracker::~ShaderConstantTracker()
{
	m_pActualDevice->Release();
}
	
void ShaderConstantTracker::RecordShaderConstantF(UINT StartRegister, CONST float* pConstantData,UINT Vector4fCount)
{
	if (m_bClearPending) {
		Clear();
	}

	ModifyShaderConstantF(StartRegister, pConstantData, Vector4fCount);
}

void ShaderConstantTracker::RecordShaderConstantI(UINT StartRegister, CONST int* pConstantData,UINT Vector4iCount)
{
	if (m_bClearPending) {
		Clear();
	}

	ModifyShaderConstantI(StartRegister, pConstantData, Vector4iCount);
}

void ShaderConstantTracker::RecordShaderConstantB(UINT StartRegister, CONST BOOL* pConstantData, UINT  BoolCount)
{
	if (m_bClearPending) {
		Clear();
	}

	ModifyShaderConstantB(StartRegister, pConstantData, BoolCount);
}



void ShaderConstantTracker::OnShaderSet()
{
	m_bClearPending = true;
}


void ShaderConstantTracker::ModifyShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount)
{
	assert( _CrtCheckMemory( ) );
	// delete existing entry if there is one
	m_floats.erase(StartRegister);
	 _CrtCheckMemory( ) ;
	
	ConstantRecord<float> moo (StartRegister, pConstantData, Vector4fCount, 4);
	std::pair<UINT, ConstantRecord<float>> toInsert = std::pair<UINT, ConstantRecord<float>>(StartRegister, moo);
	
	m_floats.insert(toInsert);
	_CrtCheckMemory( );
	//ConstantRecord<float> moo2 = m_floats.at(4);

	////m_floats.insert(std::pair<UINT, ConstantRecord<float>>(StartRegister, ConstantRecord<float>(StartRegister, pConstantData, Vector4fCount, 4));
	//OutputDebugString("meep\n");
}

void ShaderConstantTracker::ModifyShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount)
{
	// delete existing entry if there is one
	m_ints.erase(StartRegister);
	
	m_ints.insert(std::pair<UINT, ConstantRecord<int>>(StartRegister, ConstantRecord<int>(StartRegister, pConstantData, Vector4iCount, 4)));
}

void ShaderConstantTracker::ModifyShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount)
{
	// delete existing entry if there is one
	m_bools.erase(StartRegister);
	
	m_bools.insert(std::pair<UINT, ConstantRecord<BOOL>>(StartRegister, ConstantRecord<BOOL>(StartRegister, pConstantData, BoolCount, 1)));
}

void ShaderConstantTracker::SetAll()
{
	auto itF = m_floats.begin();
	ConstantRecord<float>* currentFloatConstant = NULL;
	while (itF != m_floats.end()) {
		currentFloatConstant = &(itF->second);
		m_pActualDevice->SetVertexShaderConstantF(currentFloatConstant->StartRegister, currentFloatConstant->DataPointer(), currentFloatConstant->Count);
	}

	auto itI = m_ints.begin();
	ConstantRecord<int>* currentIntConstant = NULL;
	while (itI != m_ints.end()) {
		currentIntConstant = &(itI->second);
		m_pActualDevice->SetVertexShaderConstantI(currentIntConstant->StartRegister, currentIntConstant->DataPointer(), currentIntConstant->Count);
	}

	auto itB = m_bools.begin();
	ConstantRecord<BOOL>* currentBoolConstant = NULL;
	while (itB != m_bools.end()) {
		currentBoolConstant = &(itB->second);
		m_pActualDevice->SetVertexShaderConstantB(currentBoolConstant->StartRegister, currentBoolConstant->DataPointer(), currentBoolConstant->Count);
	}
}

void ShaderConstantTracker::Clear()
{
	m_floats.clear();
	m_ints.clear();
	m_bools.clear();
	
	m_bClearPending = false;
}