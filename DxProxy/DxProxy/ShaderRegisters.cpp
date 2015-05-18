/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

File <ShaderRegisters.cpp> and
Class <ShaderRegisters> :
Copyright (C) 2013 Chris Drain

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

#include "ShaderRegisters.h"
#include "vireio.h"
#include <assert.h>

using namespace vireio;

/**
* Constructor, creates register vector.
* @param maxVSConstantRegistersF Maximum number of constant registers.
* @param pActualDevice Pointer to actual (not wrapped) D3D device.
***/
ShaderRegisters::ShaderRegisters(DWORD maxPSConstantRegistersF, DWORD maxVSConstantRegistersF, IDirect3DDevice9* pActualDevice) :
	m_maxPSConstantRegistersF(maxPSConstantRegistersF),
	m_maxVSConstantRegistersF(maxVSConstantRegistersF),
	m_psRegistersF(maxPSConstantRegistersF * VECTOR_LENGTH, 0), // VECTOR_LENGTH floats per register
	m_vsRegistersF(maxVSConstantRegistersF * VECTOR_LENGTH, 0), // VECTOR_LENGTH floats per register
	m_pActualDevice(pActualDevice),
	m_pActivePixelShader(NULL),
	m_pActiveVertexShader(NULL)
{
	assert(pActualDevice != NULL);

	//TODO assignment and copy - add ref to device (remove ref from old device on assign)? or prevent
	m_pActualDevice->AddRef();
}

/**
* Destructor, releases device and vertex shader.
***/
ShaderRegisters::~ShaderRegisters()
{
	if (m_pActualDevice)
		m_pActualDevice->Release();

	if (m_pActivePixelShader)
		m_pActivePixelShader->Release();

	if (m_pActiveVertexShader)
		m_pActiveVertexShader->Release();
}

/**
* Sets proxy registers and marks them dirty.
* Register should only have clean set to true when restoring from stateblock. It marks all registers
* that are set as clean instead of marking dirty.
*
* Return D3DERR_INVALIDCALL if any registers out of internal range.
* @param StartRegister Look at D3DProxyDevice::SetVertexShaderConstantF(). 
* @param pConstantData Look at D3DProxyDevice::SetVertexShaderConstantF().
* @param Vector4fCount Look at D3DProxyDevice::SetVertexShaderConstantF().
***/
HRESULT WINAPI ShaderRegisters::SetVertexShaderConstantF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount)
{
	if ((StartRegister >= m_maxVSConstantRegistersF) || ((StartRegister + Vector4fCount) >= m_maxVSConstantRegistersF))
		return D3DERR_INVALIDCALL;

	// Set proxy registers
	std::copy(pConstantData, pConstantData + (VECTOR_LENGTH * Vector4fCount), m_vsRegistersF.begin() + RegisterIndex(StartRegister));

	// Mark registers dirty
	dirtyVSRegisters.MarkRangeDirty(StartRegister, Vector4fCount);

	return D3D_OK;
}

/**
* Gets shader constant register.
* @param StartRegister Look at D3DProxyDevice::GetVertexShaderConstantF(). 
* @param pConstantData Look at D3DProxyDevice::GetVertexShaderConstantF().
* @param Vector4fCount Look at D3DProxyDevice::GetVertexShaderConstantF().
***/
HRESULT WINAPI ShaderRegisters::GetVertexShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	if ((StartRegister >= m_maxVSConstantRegistersF) || ((StartRegister + Vector4fCount) >= m_maxVSConstantRegistersF))
		return D3DERR_INVALIDCALL;

	pConstantData = &m_vsRegistersF[RegisterIndex(StartRegister)];

	return D3D_OK;
}

/**
* Sets proxy registers and marks them dirty.
* Register should only have clean set to true when restoring from stateblock. It marks all registers
* that are set as clean instead of marking dirty.
*
* Return D3DERR_INVALIDCALL if any registers out of internal range.
* @param StartRegister Look at D3DProxyDevice::SetPixelShaderConstantF(). 
* @param pConstantData Look at D3DProxyDevice::SetPixelShaderConstantF().
* @param Vector4fCount Look at D3DProxyDevice::SetPixelShaderConstantF().
***/
HRESULT WINAPI ShaderRegisters::SetPixelShaderConstantF(UINT StartRegister, const float* pConstantData, UINT Vector4fCount)
{
	if ((StartRegister >= m_maxPSConstantRegistersF) || ((StartRegister + Vector4fCount) >= m_maxPSConstantRegistersF))
		return D3DERR_INVALIDCALL;

	// Set proxy registers
	std::copy(pConstantData, pConstantData + (VECTOR_LENGTH * Vector4fCount), m_psRegistersF.begin() + RegisterIndex(StartRegister));

	// Mark registers dirty
	dirtyPSRegisters.MarkRangeDirty(StartRegister, Vector4fCount);

	return D3D_OK;
}

/**
* Gets shader constant register.
* @param StartRegister Look at D3DProxyDevice::GetPixelShaderConstantF(). 
* @param pConstantData Look at D3DProxyDevice::GetPixelShaderConstantF().
* @param Vector4fCount Look at D3DProxyDevice::GetPixelShaderConstantF().
***/
HRESULT WINAPI ShaderRegisters::GetPixelShaderConstantF(UINT StartRegister, float* pConstantData, UINT Vector4fCount)
{
	if ((StartRegister >= m_maxPSConstantRegistersF) || ((StartRegister + Vector4fCount) >= m_maxPSConstantRegistersF))
		return D3DERR_INVALIDCALL;

	pConstantData = &m_psRegistersF[RegisterIndex(StartRegister)];

	return D3D_OK;
}

/**
* Returns the vertex shader register vector containing all constant registers.
***/
std::vector<float> ShaderRegisters::GetAllVSConstantRegistersF()
{
	return m_vsRegistersF;
}

/**
* Returns the pixel shader register vector containing all constant registers.
***/
std::vector<float> ShaderRegisters::GetAllPSConstantRegistersF()
{
	return m_psRegistersF;
}

/**
* For restoring states from D3DProxyStateBlock.
* If sides during capture were mixed or the current device side doesn't match side at time of
* capture then updateStereoConstants should be true.
* (basically if it's possible that the actual device values for the register don't match the
* appropriate stereo versin then updateStereoConstants should be set to true)
* NOTE: Above is irrelevant; Due to questionable lack of copying of vertex shader into stateblock we
* have no idea what the state of the constants might be so updating isn't optional. Fix? See
* stateblock textures for further thoughts.
* The only time you restore all registers is when the whole vertex shader state is saved, in which
* case there will always be a vertex shader to go with the registers (it may be null).
* @param storedVShader Pointer to stored vertex shader.
***/
void ShaderRegisters::SetFromStateBlockVertexShader(D3D9ProxyVertexShader* storedVShader)
{
	// vertex shader
	_SAFE_RELEASE(m_pActiveVertexShader);
	m_pActiveVertexShader = storedVShader;
	if (m_pActiveVertexShader)
		m_pActiveVertexShader->AddRef();
}

/**
* For restoring states from D3DProxyStateBlock.
* If sides during capture were mixed or the current device side doesn't match side at time of
* capture then updateStereoConstants should be true.
* (basically if it's possible that the actual device values for the register don't match the
* appropriate stereo versin then updateStereoConstants should be set to true)
* NOTE: Above is irrelevant; Due to questionable lack of copying of vertex shader into stateblock we
* have no idea what the state of the constants might be so updating isn't optional. Fix? See
* stateblock textures for further thoughts.
* The only time you restore all registers is when the whole vertex shader state is saved, in which
* case there will always be a vertex shader to go with the registers (it may be null).
* @param storedPShader Pointer to stored pixel shader.
***/
void ShaderRegisters::SetFromStateBlockPixelShader(D3D9ProxyPixelShader* storedPShader)
{
	// pixel shader
	_SAFE_RELEASE(m_pActivePixelShader);
	m_pActivePixelShader = storedPShader;
	if (m_pActivePixelShader)
		m_pActivePixelShader->AddRef();
}

/**
* For restoring states from D3DProxyStateBlock.
* If sides during capture were mixed or the current device side doesn't match side at time of
* capture then updateStereoConstants should be true.
* (basically if it's possible that the actual device values for the register don't match the
* appropriate stereo versin then updateStereoConstants should be set to true)
* NOTE: Above is irrelevant; Due to questionable lack of copying of vertex shader into stateblock we
* have no idea what the state of the constants might be so updating isn't optional. Fix? See
* stateblock textures for further thoughts.
* The only time you restore all registers is when the whole vertex shader state is saved, in which
* case there will always be a vertex shader to go with the registers (it may be null).
* @param storedPSRegisters Pointer to stored pixel shader register map.
* @param storedVSRegisters Pointer to stored vertex shader register map.
***/
void ShaderRegisters::SetFromStateBlockData(std::map<UINT, D3DXVECTOR4> * storedVSRegisters, std::map<UINT, D3DXVECTOR4> * storedPSRegisters)
{
	// vertex shader registers
	for (auto itNewRegsVS = storedVSRegisters->begin();
	     itNewRegsVS != storedVSRegisters->end();
	     ++itNewRegsVS)
	{
		if ((RegisterIndex(itNewRegsVS->first) + VECTOR_LENGTH) >= m_vsRegistersF.size())
			throw std::out_of_range("Register from stateblock is out of range, implosion imminent");

		//std::copy(static_cast<float*>(itNewRegsVS->second), static_cast<float*>(itNewRegsVS->second) + VECTOR_LENGTH,  &m_vsRegistersF[RegisterIndex(itNewRegsVS->first)]);
		// copy produces warnings, this does not.
		int regStartIndexInVector = RegisterIndex(itNewRegsVS->first);
		m_vsRegistersF[regStartIndexInVector	] = itNewRegsVS->second.x;
		m_vsRegistersF[regStartIndexInVector + 1] = itNewRegsVS->second.y;
		m_vsRegistersF[regStartIndexInVector + 2] = itNewRegsVS->second.z;
		m_vsRegistersF[regStartIndexInVector + 3] = itNewRegsVS->second.w;

		// register is clean (now matches device state - unless it's stereo in which case it might not, that is handled at the end)
		dirtyVSRegisters.MarkClean(itNewRegsVS->first);
	}

	MarkAllVSStereoConstantsDirty();

	// pixel shader registers
	for (auto itNewRegsPS = storedPSRegisters->begin();
	    itNewRegsPS != storedPSRegisters->end();
	    ++itNewRegsPS)
	{
		if ((RegisterIndex(itNewRegsPS->first) + VECTOR_LENGTH) >= m_psRegistersF.size())
			throw std::out_of_range("Register from stateblock is out of range, implosion imminent");

		// copy produces warnings, this does not.
		int regStartIndexInVector = RegisterIndex(itNewRegsPS->first);
		m_psRegistersF[regStartIndexInVector	] = itNewRegsPS->second.x;
		m_psRegistersF[regStartIndexInVector + 1] = itNewRegsPS->second.y;
		m_psRegistersF[regStartIndexInVector + 2] = itNewRegsPS->second.z;
		m_psRegistersF[regStartIndexInVector + 3] = itNewRegsPS->second.w;

		// register is clean (now matches device state - unless it's stereo in which case it might not, that is handled at the end)
		dirtyPSRegisters.MarkClean(itNewRegsPS->first);
	}

	MarkAllPSStereoConstantsDirty();
}

/**
* For restoring states from D3DProxyStateBlock.
* If sides during capture were mixed or the current device side doesn't match side at time of
* capture then updateStereoConstants should be true.
* (basically if it's possible that the actual device values for the register don't match the
* appropriate stereo versin then updateStereoConstants should be set to true)
* NOTE: Above is irrelevant; Due to questionable lack of copying of vertex shader into stateblock we
* have no idea what the state of the constants might be so updating isn't optional. Fix? See
* stateblock textures for further thoughts.
* The only time you restore all registers is when the whole vertex shader state is saved, in which
* case there will always be a vertex shader to go with the registers (it may be null).
* @param storedPSRegisters Pointer to stored pixel shader register vector.
* @param storedVSRegisters Pointer to stored vertex shader register vector.
* @param storedPShader Pointer to stored pixel shader.
* @param storedVShader Pointer to stored vertex shader.
***/
void ShaderRegisters::SetFromStateBlockData(std::vector<float> * storedVSRegisters, std::vector<float> *storedPSRegisters)
{
	if (storedVSRegisters)
	{
		// Full register capture should always match the size of the existing register set as size is fixed and register was captured from this
		assert(storedVSRegisters->size() == m_vsRegistersF.size());

		std::copy(storedVSRegisters->begin(), storedVSRegisters->end(), m_vsRegistersF.begin());

		// Data should match registers that are already on device (unless it's stereo in which case it might not, that is handled next)
		dirtyVSRegisters.MarkAllClean();

		MarkAllVSStereoConstantsDirty();
	}

	if (storedPSRegisters)
	{
		// Full register capture should always match the size of the existing register set as size is fixed and register was captured from this
		assert(storedPSRegisters->size() == m_psRegistersF.size());

		std::copy(storedPSRegisters->begin(), storedPSRegisters->end(), m_psRegistersF.begin());

		// Data should match registers that are already on device (unless it's stereo in which case it might not, that is handled next)
		dirtyPSRegisters.MarkAllClean();

		MarkAllPSStereoConstantsDirty();
	}
}

/**
* This will apply all dirty (vertex and pixel shader) registers to actual device. 
* StereoShaderConstants are updated and applied followed by unmodified.
* Note that stereo constants will only be applied if the underlying register has changed. To apply a 
* specific side whether dirty or not use ApplyStereoConstantsVS with forceApply true.
* @param currentSide Left or Right side.
***/
void ShaderRegisters::ApplyAllDirty(vireio::RenderPosition currentSide) 
{
	int start = 0;
	
	// vertex shader
	start = dirtyVSRegisters.FirstDirtyAfter(0);
	if(start >= 0)
	{
		if (m_pActiveVertexShader) {
			ApplyStereoConstantsVS(currentSide, true);
		}
		
		// Apply all remaining dirty registers (should just be non-stereo that remain dirty) to device
		start = dirtyVSRegisters.FirstDirtyAfter(0);
		while(start >= 0) {
			int end = dirtyVSRegisters.LastInSpan(start);
			m_pActualDevice->SetVertexShaderConstantF(start, &m_vsRegistersF[RegisterIndex(start)], end-start+1);
			
			start = dirtyVSRegisters.FirstDirtyAfter(end+1);
		}
		
		dirtyVSRegisters.MarkAllClean();
	}
	
	// pixel shader
	start = dirtyPSRegisters.FirstDirtyAfter(0);
	if(start >= 0)
	{
		if (m_pActivePixelShader) {
			ApplyStereoConstantsPS(currentSide, true);
		}
		
		// Apply all remaining dirty registers (should just be non-stereo that remain dirty) to device
		start = dirtyPSRegisters.FirstDirtyAfter(0);
		while(start >= 0) {
			int end = dirtyPSRegisters.LastInSpan(start);
			m_pActualDevice->SetPixelShaderConstantF(start, &m_psRegistersF[RegisterIndex(start)], end-start+1);
			
			start = dirtyPSRegisters.FirstDirtyAfter(end+1);
		}
		
		dirtyPSRegisters.MarkAllClean();
	}
}

/**
* This will apply all (vertex and pixel shader) StereoShaderConstants to the device (updating dirty ones before applying them).
* @param currentSide Left or Right side.
***/
void ShaderRegisters::ApplyAllStereoConstants(vireio::RenderPosition currentSide)
{
	ApplyStereoConstantsVS(currentSide, false);
	ApplyStereoConstantsPS(currentSide, false);
}

/**
* Changes the active vertex shader.
* Updates the data in new shader constants with data from matching constants from last shader.
* @param pNewVertexShader Pointer to the new vertex shader.
***/
void ShaderRegisters::ActiveVertexShaderChanged(D3D9ProxyVertexShader* pNewVertexShader)
{
	if (m_pActiveVertexShader == pNewVertexShader)
		return;

	if (pNewVertexShader) {

		std::map<UINT, StereoShaderConstant<float>>* pNewShaderModConstants = pNewVertexShader->ModifiedConstants();

		std::map<UINT, StereoShaderConstant<float>>* pOldShaderModConstants = NULL;
		if (m_pActiveVertexShader)
			pOldShaderModConstants = m_pActiveVertexShader->ModifiedConstants();

		// Update the data in new shader constants with data from matching constants from last shader.
		for (auto itNewConstants = pNewShaderModConstants->begin();
		    itNewConstants != pNewShaderModConstants->end();
		    ++itNewConstants)
		{
			bool mightBeDirty = true;

			if (pOldShaderModConstants) {
				// No idea if this is saving any time or if it would be better to just mark all the registers dirty 
				// and re-apply the constants on first draw
				if (m_pActiveVertexShader->ModifiedConstants()->count(itNewConstants->first) == 1) {
					if (pOldShaderModConstants->at(itNewConstants->first).SameConstantAs(itNewConstants->second)) {
						(*pNewShaderModConstants).at(itNewConstants->first) = (*pOldShaderModConstants).at(itNewConstants->first);
						mightBeDirty = false;
					}
				}
			}

			// If there isn't a corresponding old modification then this modified constant will need updating
			if (mightBeDirty) {
				dirtyVSRegisters.MarkDirty(itNewConstants->first);
			}
		}
	}

	_SAFE_RELEASE(m_pActiveVertexShader);
	m_pActiveVertexShader = pNewVertexShader;
	if (m_pActiveVertexShader)
		m_pActiveVertexShader->AddRef();
}

/**
* Changes the active pixel shader.
* Updates the data in new shader constants with data from matching constants from last shader.
* @param pNewPixelShader Pointer to the new pixel shader.
***/
void ShaderRegisters::ActivePixelShaderChanged(D3D9ProxyPixelShader* pNewPixelShader)
{
	if (m_pActivePixelShader == pNewPixelShader)
		return;

	if (pNewPixelShader) {

		std::map<UINT, StereoShaderConstant<float>>* pNewShaderModConstants = pNewPixelShader->ModifiedConstants();

		std::map<UINT, StereoShaderConstant<float>>* pOldShaderModConstants = NULL;
		if (m_pActivePixelShader)
			pOldShaderModConstants = m_pActivePixelShader->ModifiedConstants();

		// Update the data in new shader constants with data from matching constants from last shader.
		for (auto itNewConstants = pNewShaderModConstants->begin();
		     itNewConstants != pNewShaderModConstants->end();
			++itNewConstants)
		{
			bool mightBeDirty = true;

			if (pOldShaderModConstants) {
				// No idea if this is saving any time or if it would be better to just mark all the registers dirty 
				// and re-apply the constants on first draw
				if (m_pActivePixelShader->ModifiedConstants()->count(itNewConstants->first) == 1) {
					if (pOldShaderModConstants->at(itNewConstants->first).SameConstantAs(itNewConstants->second)) {
						(*pNewShaderModConstants).at(itNewConstants->first) = (*pOldShaderModConstants).at(itNewConstants->first);
						mightBeDirty = false;
					}
				}
			}

			// If there isn't a corresponding old modification then this modified constant will need updating
			if (mightBeDirty) {
				dirtyPSRegisters.MarkDirty(itNewConstants->first);
			}
		}
	}

	_SAFE_RELEASE(m_pActivePixelShader);
	m_pActivePixelShader = pNewPixelShader;
	if (m_pActivePixelShader)
		m_pActivePixelShader->AddRef();
}

/**
* Releases any d3d resources (does not include device, that is only release on destruction).
***/
void ShaderRegisters::ReleaseResources()
{
	if (m_pActiveVertexShader)
		m_pActiveVertexShader->Release();

	m_pActiveVertexShader = NULL;

	// TODO Do the constant registers get cleared on device reset? Current guess is no. Needs testing or a reference finding that says what happens.
}

/**
* This will apply all vertex StereoShaderConstants to the device (updating dirty ones before applying them).
* @param currentSide Left or Right side.
* @param dirtyOnly Set to true if only dirty registers are to be applied.
***/
void ShaderRegisters::ApplyStereoConstantsVS(vireio::RenderPosition currentSide, const bool dirtyOnly)
{
	if (!m_pActiveVertexShader)
		return;

	for (auto itStereoConstant = m_pActiveVertexShader->ModifiedConstants()->begin();
	    itStereoConstant != m_pActiveVertexShader->ModifiedConstants()->end();
		++itStereoConstant)
	{
		// if any of the registers that make up this constant are dirty update before setting
		if (dirtyVSRegisters.AnyDirty(itStereoConstant->second.StartRegister(), itStereoConstant->second.Count()))
		{
			// Should we do this or make this method just switch sides without checking for updated data? 

			itStereoConstant->second.Update(&m_vsRegistersF[RegisterIndex(itStereoConstant->second.StartRegister())]);

			if (dirtyOnly) {
				// Apply this dirty constant to device
				m_pActualDevice->SetVertexShaderConstantF(itStereoConstant->second.StartRegister(), (currentSide == vireio::Left) ? itStereoConstant->second.DataLeftPointer() : itStereoConstant->second.DataRightPointer(), itStereoConstant->second.Count());
			}

			// These registers are no longer dirty
			dirtyVSRegisters.MarkRangeClean(itStereoConstant->second.StartRegister(), itStereoConstant->second.Count());
		}

		if (!dirtyOnly) {
			// Apply this constant to device
			m_pActualDevice->SetVertexShaderConstantF(itStereoConstant->second.StartRegister(), (currentSide == vireio::Left) ? itStereoConstant->second.DataLeftPointer() : itStereoConstant->second.DataRightPointer(), itStereoConstant->second.Count());
		}
	}
}

/**
* This will apply all pixel StereoShaderConstants to the device (updating dirty ones before applying them).
* @param currentSide Left or Right side.
* @param dirtyOnly Set to true if only dirty registers are to be applied.
***/
void ShaderRegisters::ApplyStereoConstantsPS(vireio::RenderPosition currentSide, const bool dirtyOnly)
{
	if (!m_pActivePixelShader)
		return;

	for (auto itStereoConstant = m_pActivePixelShader->ModifiedConstants()->begin();
	    itStereoConstant != m_pActivePixelShader->ModifiedConstants()->end();
		++itStereoConstant)
	{

		// if any of the registers that make up this constant are dirty update before setting
		if ( dirtyPSRegisters.AnyDirty(itStereoConstant->second.StartRegister(), itStereoConstant->second.Count()))
		{
			// Should we do this or make this method just switch sides without checking for updated data? 

			itStereoConstant->second.Update(&m_psRegistersF[RegisterIndex(itStereoConstant->second.StartRegister())]); //HOTSPOT 3.9%

			if (dirtyOnly) {
				// Apply this dirty constant to device
				m_pActualDevice->SetPixelShaderConstantF(itStereoConstant->second.StartRegister(), (currentSide == vireio::Left) ? itStereoConstant->second.DataLeftPointer() : itStereoConstant->second.DataRightPointer(), itStereoConstant->second.Count());
			}

			// These registers are no longer dirty
			dirtyPSRegisters.MarkRangeClean(itStereoConstant->second.StartRegister(), itStereoConstant->second.Count()); //HOTSPOT 4.8%
		}

		if (!dirtyOnly) {
			// Apply this constant to device
			m_pActualDevice->SetPixelShaderConstantF(itStereoConstant->second.StartRegister(), (currentSide == vireio::Left) ? itStereoConstant->second.DataLeftPointer() : itStereoConstant->second.DataRightPointer(), itStereoConstant->second.Count());
		}
	}
}

/**
* Marks all pixel shader StereoShaderConstants dirty so they are updated before drawing.
***/
void ShaderRegisters::MarkAllVSStereoConstantsDirty()
{
	// vertex shader
	if (m_pActiveVertexShader) {
		// Mark all StereoShaderConstants dirty so they are updated before drawing
		auto itStereoConstant = m_pActiveVertexShader->ModifiedConstants()->begin();
		while (itStereoConstant != m_pActiveVertexShader->ModifiedConstants()->end()) {

			dirtyVSRegisters.MarkDirty(itStereoConstant->first);
			++itStereoConstant;
		}
	}
}

/**
* Marks all pixel shader StereoShaderConstants dirty so they are updated before drawing.
***/
void ShaderRegisters::MarkAllPSStereoConstantsDirty()
{
	// pixel shader
	if (m_pActivePixelShader) {
		// Mark all StereoShaderConstants dirty so they are updated before drawing
		for (auto itStereoConstant = m_pActivePixelShader->ModifiedConstants()->begin();
		    itStereoConstant != m_pActivePixelShader->ModifiedConstants()->end();
			++itStereoConstant)
		{
			dirtyPSRegisters.MarkDirty(itStereoConstant->first);
		}
	}
}


RegisterDirtyStore::RegisterDirtyStore()
{
	Expand(32);
	firstDirtyReg = -1;
	lastDirtyReg = -1;
}

void RegisterDirtyStore::MarkDirty(UINT index)
{
	if(index+1 >= dirtyRegisters.size()) Expand(index);
	
	if (firstDirtyReg==-1 || firstDirtyReg>index)
		firstDirtyReg = index;
	if (lastDirtyReg==-1 || lastDirtyReg<index)
		lastDirtyReg = index;
	
	dirtyRegisters[index] = 1;
}

void RegisterDirtyStore::MarkRangeDirty(UINT start, UINT count)
{
	if(start+count+1 >= dirtyRegisters.size()) Expand(start+count);
	
	if (firstDirtyReg==-1 || firstDirtyReg>start)
		firstDirtyReg = start;
	if (lastDirtyReg==-1 || lastDirtyReg<start+count-1)
		lastDirtyReg = start+count-1;
	
	for (UINT ii=start; ii<start+count; ii++)
		dirtyRegisters[ii] = 1;
}

void RegisterDirtyStore::MarkClean(UINT index)
{
	if(index+1 >= dirtyRegisters.size()) Expand(index);
	
	dirtyRegisters[index] = 0;
}

void RegisterDirtyStore::MarkRangeClean(UINT start, UINT count)
{
	if(start+count+1 >= dirtyRegisters.size()) Expand(start+count);
	
	for(UINT ii=start; ii<start+count; ii++)
		dirtyRegisters[ii] = 0;
}

void RegisterDirtyStore::MarkAllClean()
{
	if(firstDirtyReg != -1) {
		for(UINT ii=firstDirtyReg; ii<=lastDirtyReg; ii++)
			dirtyRegisters[ii] = 0;
		firstDirtyReg = -1;
		lastDirtyReg = -1;
	}
}

/**
* Returns true if any dirty pixel shader register found in the specified range.
* @param start Start register.
* @param count Register count.
***/
bool RegisterDirtyStore::AnyDirty(UINT start, UINT count)
{
	if(start+count+1 >= dirtyRegisters.size()) Expand(start+count);
	
	for(UINT ii=start; ii<start+count; ii++) {
		if(dirtyRegisters[ii])
			return true;
	}
	return false;
}

/**
 * Return the lowest-numbered dirty pixel shader register after start (inclusive),
 * or -1 if there is none.
 */
int RegisterDirtyStore::FirstDirtyAfter(UINT start)
{
	if(firstDirtyReg == -1)
		return -1;
	
	UINT pos = start;
	while(!dirtyRegisters[pos])
	{
		if(pos+1 > lastDirtyReg)
			return -1;
		pos++;
	}
	return pos;
}

int RegisterDirtyStore::GetLastDirty()
{
	return lastDirtyReg;
}

/**
 * Given the index of a dirty pixel shader register, return the index of the
 * last dirty one so that there are no gaps between it and the start.
 */
int RegisterDirtyStore::LastInSpan(UINT start)
{
	UINT pos = start;
	
	while(pos+1<dirtyRegisters.size() && dirtyRegisters[pos+1])
	{
		pos++;
	}
	return pos;
}

void RegisterDirtyStore::Expand(UINT capacity)
{
	while(dirtyRegisters.size() < capacity+1) {
		dirtyRegisters.push_back(0);
	}
}
