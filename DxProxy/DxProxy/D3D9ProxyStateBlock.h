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

#ifndef D3D9PROXYSTATEBLOCK_H_INCLUDED
#define D3D9PROXYSTATEBLOCK_H_INCLUDED

#include <d3d9.h>
#include "D3DProxyDevice.h"
#include "Direct3DStateBlock9.h"

class D3DProxyDevice;

class D3D9ProxyStateBlock : public BaseDirect3DStateBlock9
{
public:
	D3D9ProxyStateBlock(IDirect3DStateBlock9* pActualStateBlock, D3DProxyDevice* pOwningDevice, int CaptureType);
	virtual ~D3D9ProxyStateBlock();

	// ID3D9ProxyStateBlock
	virtual HRESULT WINAPI Capture();
	virtual HRESULT WINAPI Apply();





	/*	Were all states in this block captured while the device side was set to the same side. */
	// side flag - AllLeft, AllRight or Mixed

	/*
		All simple states can be tracked by the device. This is basically all states that are basic value types rather 
		than interfaces (For example, everything set by SetSamplerState).


		Any states that are wrapped need to have the wrapped state stored. (Textures, Vertex/Pixel Shaders, etc.)

		For stereo components like Textures all parameters needed to manually re-apply them need to be saved (textures only 
		need the texture and the number of the sampler the texture is in). This is because we can't guarantee that all states
		were saved to the same side. If all states were saved on the same side then the stereo components won't need to be 
		reappplied and the procedure will be the same as for non-stereo components stated below.

		Non-stereo componenets like Vertex Buffers and Shaders just need the wrapper reference stored in order to be able
		to restore the wrapper in the device proxy. The actual state will be restored on the device by the actual StateBlock.


		View, projection and world matrix because we keep a left and right version. (TODO World matrix might be world-view 
		in which case there will be left and right versions. Don't have a good way to handle this yet (or a test case), 
		leaving world matrix to actual device for now)
	 */
	
	// General State

	// Textures in samplers (normal, vertex and displacement)
	// Vertex Buffers
	// Index Buffer
	// Viewport - Viewport handled by actual device but need to update proxy device viewport state (update m_bActiveViewportIsDefault and m_LastViewportSet) 
	// View and Projection matricies

	// Assumed to always be mono at the moment
	// Texture transforms.




	/*
	Modified vertex shader constants
	Modified vertex shader constants*/

	// Vertex State

	// vertex shader
	// vertex declaration
	// vertex shader constants - if stereo constants enabled




	// Pixel State

	// pixel shader 
	// Pixel shader constants TODO - if needed (if stereo'ified for some reason), otherwise leave these to the actual device.
};

#endif
