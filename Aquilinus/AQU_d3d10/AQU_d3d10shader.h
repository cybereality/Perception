/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <AQU_d3d10shader.h>
Copyright © 2014 Denis Reischl

This code is private and MUST NOT be set public for any 
reason. This code is intended to be used, changed, compiled 
and its build published only by Denis Reischl.

This code is intended to be used by its author,
Denis Reischl,
for any commercial and non-commercial purpose.

Following persons are granted to read the full Aquilinus
source code for life time :
Neil Schneider, Grant Bagwell and Simon Brown.
If the original author of Aquilinus, Denis Reischl,
discontinues the work on this software, the named persons
automatically inherit all rights to continue this work.

Except where otherwise noted, this work is licensed under :
<http://creativecommons.org/licenses/by-nc-nd/3.0/deed.en_US>.
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
