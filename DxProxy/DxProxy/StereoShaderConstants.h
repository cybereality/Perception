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

#ifndef STEREOSHADERCONSTANTS_H_INCLUDED
#define STEREOSHADERCONSTANTS_H_INCLUDED

#include <vector>
#include <string>
#include <unordered_map>
#include <memory>
#include "d3d9.h"
#include "StereoShaderConstant.h"
#include "ShaderConstantModification.h"
#include "ShaderRegisters.h"
#include "ShaderConfig.h"



/*typedef struct _ShaderConstDesc
{
	UINT StartReg;
	UINT RegCount;
	UINT RegLength;
	std::string Name;

} ShaderConstDesc;*/





class StereoShaderConstants
{
public:
	StereoShaderConstants(std::shared_ptr<ShaderRegisters> spProxyDeviceShaderRegisters,  std::vector<ShaderConstantConfig> constantConfigs);
	/* Load from shader profile(loaded in shader?) (indexed by shader hash, containing list of name + start reg + type (vector/matrix) + modification)*/
	/*
		Create StereoShaderConstant for each modified shader.
		 - Init'd with identity matrix or 0 vector as appropriate.
		 -	and modification to apply (ModificationFactory->Getmodification("SourceViewModel"))
	*/

	virtual ~StereoShaderConstants();

	/* Copying any constants from other that still apply */
	void UpdateFrom(StereoShaderConstants &other);
	/*
		for all non-copied constants set the first device register for each constant dirty to make sure they are recalculated before any drawing
	*/


	void SetDirtyStereoConstants(/* current side.  */);
	/*
		 clean dirty registers that are part of stereo constants and update the stereo values, then send current side to actual device for all updated constants
	*/


	

	/* Does the active vertex shader have a stereo constant starting at startReg with name name. */
	//virtual bool HasStereoConstantF(std::string modificationName, UINT startReg);

	/* 0 = No, 1+ = count of registers that are part of constant starting here */
	//virtual UINT IsStartOfStereoConstantF(UINT startReg) = 0;

	//virtual std::vector<ShaderConstDesc> ConstantsFThatNeedStereo() = 0;

private:

	std::shared_ptr<ShaderRegisters> m_spProxyDeviceShaderRegisters;

	std::unordered_map<UINT, StereoShaderConstant<float>> m_stereoConstants;
	//std::unordered_map<std::string, MatrixModification> m_matrixModifications;
	//std::unordered_map<std::string, VectorModification> m_vectorModifications;
};

#endif