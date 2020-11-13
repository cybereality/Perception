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
#ifndef ITA_D3D11INTERFACES_ENUM
#define ITA_D3D11INTERFACES_ENUM

namespace ITA_D3D11INTERFACES
{
	/**
	* Direct3D 11.0 interfaces enumeration.
	* The following interfaces are used with Direct3D 11.
	***/
	enum ITA_D3D11Interfaces
	{
		/*** D3D11 core interfaces ***/
		ID3D11Asynchronous,                       /**< Retrieves data from the GPU asynchronously. **/
		ID3D11BlendState,                         /**< Accesses blending state. **/
		ID3D11BlendState1,                        /**< The blend-state interface holds a description for blending state that you can bind to the output-merger stage. **/
		ID3D11Counter,                            /**< Measures GPU performance. **/
		ID3D11CommandList,                        /**< Encapsulates a recorded list of graphics commands that can be later played back. **/
		ID3D11DepthStencilState,                  /**< Accesses depth-stencil state. **/
		ID3D11Device,                             /**< Represents a virtual adapter. **/
		ID3D11Device1,                            /**< Represents a virtual adapter. Adds new methods to those of ID3D11Device. **/
		ID3D11Device2,                            /**< Represents a virtual adapter. Adds new methods to those of ID3D11Device1. **/
		ID3D11Device3,                            /**< ID3D11Device3 adds new methods to those in ID3D11Device2. **/
		ID3D11DeviceChild,                        /**< Accesses data used by a device. **/
		ID3D11DeviceContext,                      /**< Generates rendering commands. **/
		ID3D11DeviceContext1,                     /**< Represents a device context. Adds new methods to those of ID3D11DeviceContext. **/
		ID3D11DeviceContext2,                     /**< Represents a device context. Adds new methods to those of ID3D11DeviceContext1. **/
		ID3D11DeviceContext3,                     /**< ID3D11DeviceContext3 adds new methods to those in ID3D11DeviceContext2. **/
		ID3DDeviceContextState,                   /**< Represents a context state object, which holds state and behavior information about a Direct3D device. **/
		ID3D11InputLayout,                        /**< Accesses pipeline-input data from memory. **/
		ID3D11Predicate,                          /**< Determines whether geometry should be processed. **/
		ID3D11Query,                              /**< Queries information from the GPU. **/
		ID3D11RasterizerState,                    /**< Accesses rasterizer state. **/
		ID3D11RasterizerState1,                   /**< The rasterizer-state interface holds a description for rasterizer state that you can bind to the rasterizer stage. **/
		ID3D11SamplerState,                       /**< Accesses sampler state. **/
		/*** D3D11 layer interfaces ***/
		ID3D11Debug,                              /**< Enables/disables the debug layer. **/
		ID3D11InfoQueue,                          /**< Stores, retrieves, and filters debug messages. **/
		ID3D11SwitchToRef,                        /**< Not supported in Direct3D 11. **/
		D3D11RefDefaultTrackingOptions,           /**< Sets reference default tracking options. **/
		ID3D11RefTrackingOptions,                 /**< Sets reference tracking options. **/
		ID3D11TracingDevice,                      /**< Sets shader tracking information. **/
		/*** D3D11 resource interfaces ***/
		ID3D11Buffer,                             /**< Accesses buffer data. **/
		ID3D11DepthStencilView,                   /**< Accesses data in a depth-stencil texture. **/
		ID3D11RenderTargetView,                   /**< Accesses data in a render target. **/
		ID3D11Resource,                           /**< Base class for a resource. **/
		ID3D11ShaderResourceView,                 /**< Accesses data in a shader-resource. **/
		ID3D11Texture1D,                          /**< Accesses data in a 1D texture or a 1D texture array. **/
		ID3D11Texture2D,                          /**< Accesses data in a 2D texture or a 2D texture array **/
		ID3D11Texture3D,                          /**< Accesses data in a 3D texture or a 3D texture array. **/
		ID3D11UnorderedAccessView,                /**< Accesses data in an unordered resource. **/
		ID3D11View,                               /**< Accesses data in a resource. **/
		/*** D3D11 shader interfaces ***/
		ID3D11ClassInstance,                      /**< Encapsulates an HLSL class. **/
		ID3D11ClassLinkage,                       /**< Encapsulates an HLSL dynamic linkage. **/
		ID3D11ComputeShader,                      /**< Manages an executable program (a compute shader) that controls the compute-shader stage. **/
		ID3D11DomainShader,                       /**< A domain shader evaluates a control point at a corresponding uv coordinate to find a vertex position on a surface, it is the third stage of three that implements tessellation. **/
		ID3D11FunctionLinkingGraph,               /**< Makes shaders that consist of a sequence of precompiled function calls that pass values to each other. **/
		ID3D11FunctionReflection,                 /**< Accesses function info. **/
		ID3D11FunctionParameterReflection,        /**< Accesses function-parameter info. **/
		ID3D11GeometryShader,                     /**< A geometry shader implements per-primitive processing in the geometry-shader stage. **/
		ID3D11HullShader,                         /**< A hull shader transforms a high-order surface into a patch approximation, it is the first stage of three that implements tessellation. **/
		ID3D11LibraryReflection,                  /**< Accesses library info. **/
		ID3D11Linker,                             /**< Links a shader module. **/
		ID3D11LinkingNode,                        /**< Used for shader linking. **/
		ID3D11Module,                             /**< Creates an instance of a module that is used for resource re-binding. **/
		ID3D11ModuleInstance,                     /**< Used for resource re-binding. **/
		ID3D11PixelShader,                        /**< A pixel-shader implements per-pixel processing in the pixel-shader stage. **/
		ID3D11ShaderReflection,                   /**< A COM interface for reading information from a compiled shader at author time. **/
		ID3D11ShaderReflectionConstantBuffer,     /**< A helper interface for getting a shader-reflection constant-buffer interface. **/
		ID3D11ShaderReflectionType,               /**< A helper interface for getting a shader-reflection-type interface. **/
		ID3D11ShaderReflectionVariable,           /**< A helper interface for getting a shader-reflection-variable interface. **/
		ID3D11ShaderTrace,                        /**< A helper interface for obtaining traces of shader executions. **/
		ID3D11ShaderTraceFactory,                 /**< A helper interface for generating shader trace information objects. **/
		ID3D11VertexShader,                       /**< A vertex-shader implements per-vertex processing in the vertex-shader stage. **/
		/*** D3D11 D3DX interfaces ***/
		ID3DX11DataLoader,                        /**< Data loading object used by ID3DX11ThreadPump Interface for loading data asynchronously. **/
		ID3DX11DataProcessor,                     /**< Data processing object used by ID3DX11ThreadPump Interface for processing loaded data asynchronously. **/
		ID3DX11ThreadPump,                        /**< Used to execute tasks asynchronously. This object takes up a substantial amount of resources, so generally only one should be created per application. **/
		/*** D3D11 effect interfaces ***/
		ID3DX11Effect,                            /**< Manages a set of state objects, resources and shaders for implementing a rendering effect. **/
		ID3DX11EffectBlendVariable,               /**< Accesses blend state. **/
		ID3DX11EffectClassInstanceVariable,       /**< Accesses a class instance. **/
		ID3DX11EffectConstantBuffer,              /**< Accesses constant buffers or texture buffers. **/
		ID3DX11EffectDepthStencilVariable,        /**< Accesses depth-stencil state. **/
		ID3DX11EffectDepthStencilViewVariable,    /**< Accesses a depth-stencil view. **/
		ID3DX11EffectGroup,                       /**< Accesses an effect group. **/
		ID3DX11EffectInterfaceVariable,           /**< Accesses an interface. **/
		ID3DX11EffectMatrixVariable,              /**< Accesses a matrix. **/
		ID3DX11EffectPass,                        /**< Encapsulates state assignments within a technique. **/
		ID3DX11EffectRasterizerVariable,          /**< Accesses rasterizer state. **/
		ID3DX11EffectRenderTargetViewVariable,    /**< Accesses a render target. **/
		ID3DX11EffectSamplerVariable,             /**< Accesses sampler state. **/
		ID3DX11EffectScalarVariable,              /**< Accesses scalar values. **/
		ID3DX11EffectShaderResourceVariable,      /**< Accesses a shader resource. **/
		ID3DX11EffectShaderVariable,              /**< Accesses a shader variable. **/
		ID3DX11EffectStringVariable,              /**< Accesses a string variable. **/
		ID3DX11EffectTechnique,                   /**< A collection of passes. **/
		ID3DX11EffectType,                        /**< Accesses effect variables by type. **/
		ID3DX11EffectUnorderedAccessViewVariable, /**< Accesses an Unordered Access View. **/
		ID3DX11EffectVariable,                    /**< Base class for all effect variables. **/
		ID3DX11EffectVectorVariable,              /**< Accesses a four-component vector. **/
	};

}
#endif