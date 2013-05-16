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

#ifndef SHADERCONSTANTTRACKER_H_INCLUDED
#define SHADERCONSTANTTRACKER_H_INCLUDED

#include "Direct3DDevice9.h"
#include "D3DProxyDevice.h"
#include "ConstantRecord.h"


class ShaderConstantTracker
{
public:
	ShaderConstantTracker(IDirect3DDevice9* pActualDevice);
	virtual ~ShaderConstantTracker();
	
	/* Call these methods each time the relevant method of device is called by the calling application to store a copy of
		the constant. OnShaderSet should be called when SetShader is called by the calling application. 
		After that the next call to one of these methods will clear all the stored constants.  */
	void RecordShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	void RecordShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
	void RecordShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);

	/* Call this when the calling application calls SetShader. The next shader constant recorded will
		clear all existing constants. */
	void OnShaderSet();

	/* Modify a constant that has already been set. These methods should be used to modify
		data when overriding normal values from the game engine. These methods won't clear
		all the constants if called after a shader has been set. */
	void ModifyShaderConstantF(UINT StartRegister,CONST float* pConstantData,UINT Vector4fCount);
	void ModifyShaderConstantI(UINT StartRegister,CONST int* pConstantData,UINT Vector4iCount);
	void ModifyShaderConstantB(UINT StartRegister,CONST BOOL* pConstantData,UINT  BoolCount);

	/* Will call relevant SetVertexShaderConstant method on actual device for every recorded shader.
		This allows you to to switch render sides, modify the relevant matricies and then reapply
		all the shader constants including modifications. After that you should call setshader
		on the actual devie with the current shader and you will be ready to draw the other side.*/
	void SetAll();

	/* Clear all recorded constants from the tracker */
	void Clear();

private:

	std::unordered_map<UINT, ConstantRecord<float>> m_floats;
	std::unordered_map<UINT, ConstantRecord<int>> m_ints;
	std::unordered_map<UINT, ConstantRecord<BOOL>> m_bools;

	IDirect3DDevice9* m_pActualDevice;

	bool m_bClearPending;

};

#endif