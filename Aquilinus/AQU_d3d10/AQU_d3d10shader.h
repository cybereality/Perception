/***************************************************************
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
****************************************************************/

/*** d3d10shader super method prototypes ***/
typedef HRESULT (WINAPI *LPD3D10CompileShader)                  (LPCSTR pSrcData, SIZE_T SrcDataSize, LPCSTR pFileName, CONST D3D10_SHADER_MACRO *pDefines, LPD3D10INCLUDE pInclude, LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags, ID3D10Blob **ppShader, ID3D10Blob **ppErrorMsgs);
typedef HRESULT (WINAPI *LPD3D10DisassembleShader)              (CONST void *pShader, SIZE_T BytecodeLength, BOOL EnableColorCode, LPCSTR pComments, ID3D10Blob **ppDisassembly);
typedef LPCSTR  (WINAPI *LPD3D10GetPixelShaderProfile)          (ID3D10Device *pDevice);
typedef LPCSTR  (WINAPI *LPD3D10GetVertexShaderProfile)         (ID3D10Device *pDevice);
typedef LPCSTR  (WINAPI *LPD3D10GetGeometryShaderProfile)       (ID3D10Device *pDevice);
typedef HRESULT (WINAPI *LPD3D10ReflectShader)                  (CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10ShaderReflection **ppReflector);
typedef HRESULT (WINAPI *LPD3D10PreprocessShader)               (LPCSTR pSrcData, SIZE_T SrcDataSize, LPCSTR pFileName, CONST D3D10_SHADER_MACRO *pDefines, LPD3D10INCLUDE pInclude, ID3D10Blob **ppShaderText, ID3D10Blob **ppErrorMsgs);
typedef HRESULT (WINAPI *LPD3D10GetInputSignatureBlob)          (CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10Blob **ppSignatureBlob);
typedef HRESULT (WINAPI *LPD3D10GetOutputSignatureBlob)         (CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10Blob **ppSignatureBlob);
typedef HRESULT (WINAPI *LPD3D10GetInputAndOutputSignatureBlob) (CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10Blob **ppSignatureBlob);
typedef HRESULT (WINAPI *LPD3D10GetShaderDebugInfo)             (CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10Blob **ppDebugInfo);


/*** d3d10shader super methods ***/
LPD3D10CompileShader                  g_pD3D10CompileShader_Super       = NULL;
LPD3D10DisassembleShader              g_pD3D10DisassembleShader_Super = NULL;
LPD3D10GetPixelShaderProfile          g_pD3D10GetPixelShaderProfile_Super = NULL;
LPD3D10GetVertexShaderProfile         g_pD3D10GetVertexShaderProfile_Super = NULL;
LPD3D10GetGeometryShaderProfile       g_pD3D10GetGeometryShaderProfile_Super = NULL;
LPD3D10ReflectShader                  g_pD3D10ReflectShader_Super = NULL;
LPD3D10PreprocessShader               g_pD3D10PreprocessShader_Super = NULL;
LPD3D10GetInputSignatureBlob          g_pD3D10GetInputSignatureBlob_Super = NULL;
LPD3D10GetOutputSignatureBlob         g_pD3D10GetOutputSignatureBlob_Super = NULL;
LPD3D10GetInputAndOutputSignatureBlob g_pD3D10GetInputAndOutputSignatureBlob_Super = NULL;
LPD3D10GetShaderDebugInfo             g_pD3D10GetShaderDebugInfo_Super = NULL;

/*** d3d10shader methods wrapped ***/
HRESULT WINAPI D3D10CompileShader(LPCSTR pSrcData, SIZE_T SrcDataSize, LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude, LPCSTR pFunctionName, LPCSTR pProfile, UINT Flags, ID3D10Blob** ppShader, ID3D10Blob** ppErrorMsgs) { return g_pD3D10CompileShader_Super(pSrcData, SrcDataSize, pFileName, pDefines, pInclude, pFunctionName, pProfile, Flags, ppShader, ppErrorMsgs); }
HRESULT WINAPI D3D10DisassembleShader(CONST void *pShader, SIZE_T BytecodeLength, BOOL EnableColorCode, LPCSTR pComments, ID3D10Blob** ppDisassembly) { return g_pD3D10DisassembleShader_Super(pShader, BytecodeLength, EnableColorCode, pComments, ppDisassembly); }
LPCSTR  WINAPI D3D10GetPixelShaderProfile(ID3D10Device *pDevice) { return g_pD3D10GetPixelShaderProfile_Super(pDevice); }
LPCSTR  WINAPI D3D10GetVertexShaderProfile(ID3D10Device *pDevice) { return g_pD3D10GetVertexShaderProfile_Super(pDevice); }
LPCSTR  WINAPI D3D10GetGeometryShaderProfile(ID3D10Device *pDevice) { return g_pD3D10GetGeometryShaderProfile_Super(pDevice); }
HRESULT WINAPI D3D10ReflectShader(CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10ShaderReflection **ppReflector) { return g_pD3D10ReflectShader_Super(pShaderBytecode, BytecodeLength, ppReflector); }
HRESULT WINAPI D3D10PreprocessShader(LPCSTR pSrcData, SIZE_T SrcDataSize,  LPCSTR pFileName, CONST D3D10_SHADER_MACRO* pDefines, LPD3D10INCLUDE pInclude, ID3D10Blob** ppShaderText, ID3D10Blob** ppErrorMsgs) { return g_pD3D10PreprocessShader_Super(pSrcData, SrcDataSize, pFileName, pDefines, pInclude, ppShaderText, ppErrorMsgs); }
HRESULT WINAPI D3D10GetInputSignatureBlob(CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10Blob **ppSignatureBlob) { return g_pD3D10GetInputSignatureBlob_Super(pShaderBytecode, BytecodeLength, ppSignatureBlob); }
HRESULT WINAPI D3D10GetOutputSignatureBlob(CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10Blob **ppSignatureBlob) { return g_pD3D10GetOutputSignatureBlob_Super(pShaderBytecode, BytecodeLength, ppSignatureBlob); }
HRESULT WINAPI D3D10GetInputAndOutputSignatureBlob(CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10Blob **ppSignatureBlob) { return g_pD3D10GetInputAndOutputSignatureBlob_Super(pShaderBytecode, BytecodeLength, ppSignatureBlob); }
HRESULT WINAPI D3D10GetShaderDebugInfo(CONST void *pShaderBytecode, SIZE_T BytecodeLength, ID3D10Blob** ppDebugInfo) { return g_pD3D10GetShaderDebugInfo_Super(pShaderBytecode, BytecodeLength, ppDebugInfo); }
