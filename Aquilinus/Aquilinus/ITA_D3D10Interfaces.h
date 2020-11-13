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
#ifndef ITA_D3D10INTERFACES_ENUM
#define ITA_D3D10INTERFACES_ENUM

namespace ITA_D3D10INTERFACES
{
	/**
	* Direct3D 10.0 interfaces enumeration.
	* The following interfaces are used with Direct3D 10.
	***/
	enum ITA_D3D10Interfaces
	{
		/*** D3D10 core interfaces ***/
		ID3D10Asynchronous,                   /**< Retrieves data from the GPU asynchronously. **/
		ID3D10BlendState,                     /**< Accesses blending state for a Direct3D 10.0 device. **/
		ID3D10BlendState1,                    /**< Accesses blending state for a Direct3D 10.1 device. **/
		ID3D10Blob,                           /**< Return data from memory. **/
		ID3D10Counter,                        /**< Measures GPU performance. **/
		ID3D10Debug,                          /**< Enables/disables the debug layer. **/
		ID3D10DepthStencilState,              /**< Accesses depth-stencil state. **/
		ID3D10Device,                         /**< Represents a virtual adapter for Direct3D 10.0. **/
		ID3D10Device1,                        /**< Represents a virtual adapter for Direct3D 10.1. **/
		ID3D10DeviceChild,                    /**< Accesses data used by a device. **/
		ID3D10Include,                        /**< Provides user-overridable methods for handling include files when loading an effect. **/
		ID3D10InfoQueue,                      /**< Stores, retrieves, and filters debug messages. **/
		ID3D10InputLayout,                    /**< Accesses pipeline-input data from memory. **/
		ID3D10Multithread,                    /**< Accesses multithread settings. **/
		ID3D10Predicate,                      /**< Determines whether geometry should be processed. **/
		ID3D10Query,                          /**< Queries information from the GPU. **/
		ID3D10RasterizerState,                /**< Accesses rasterizer state. **/
		ID3D10SamplerState,                   /**< Accesses sampler state. **/
		ID3D10StateBlock,                     /**< Encapsulates render states. **/
		ID3D10SwitchToRef,                    /**< Switches between a hardware and software device. **/
		/*** D3D10 resource interfaces ***/
		ID3D10Buffer,                         /**< Accesses buffer data. **/
		ID3D10Resource,                       /**< Base class for a resource. **/
		ID3D10Texture1D,                      /**< Accesses data in a 1D texture or a 1D texture array. **/
		ID3D10Texture2D,                      /**< Accesses data in a 2D texture or a 2D texture array **/
		ID3D10Texture3D,                      /**< Accesses data in a 3D texture or a 3D texture array. **/
		/*** D3D10 shader interfaces ***/
		ID3D10GeometryShader,                 /**< A geometry-shader implements per-primitive processing in the geometry-shader stage. **/
		ID3D10PixelShader,                    /**< A pixel-shader implements per-pixel processing in the pixel-shader stage. **/
		ID3D10VertexShader,                   /**< A vertex-shader implements per-vertex processing in the vertex-shader stage. **/
		/*** D3D10 effect interfaces ***/
		ID3D10EffectBlendVariable,            /**< Accesses blend state. **/
		ID3D10EffectConstantBuffer,           /**< Accesses a texture-buffer or a constant-buffer. **/
		ID3D10EffectDepthStencilVariable,     /**< Accesses depth-stencil state. **/
		ID3D10EffectDepthStencilViewVariable, /**< Accesses a depth-stencil view. **/
		ID3D10Effect,                         /**< Encapsulates pipeline state in one or more rendering techniques. **/
		ID3D10EffectMatrixVariable,           /**< Accesses a matrix. **/
		ID3D10EffectPass,                     /**< Encapsulates effect state in a pass. **/
		ID3D10EffectPool,                     /**< Identifies shared-effect variables. **/
		ID3D10EffectRasterizerVariable,       /**< Accesses rasterizer state. **/
		ID3D10EffectRenderTargetViewVariable, /**< Accesses a render target. **/
		ID3D10EffectSamplerVariable,          /**< Accesses sampler state. **/
		ID3D10EffectScalarVariable,           /**< Accesses a scalar variable. **/
		ID3D10EffectShaderResourceVariable,   /**< Accesses a shader resource. **/
		ID3D10EffectShaderVariable,           /**< Accesses a shader variable. **/
		ID3D10EffectStringVariable,           /**< Accesses a string. **/
		ID3D10EffectTechnique,                /**< Encapsulates one or more passes. **/
		ID3D10EffectType,                     /**< Implements methods for accessing effect variables. **/
		ID3D10EffectVectorVariable,           /**< Accesses a vector. **/
	};

}
#endif