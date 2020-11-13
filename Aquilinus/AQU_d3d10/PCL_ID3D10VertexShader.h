/***************************************************************
Aquilinus - 3D Modification Studio
Copyright © 2014 Denis Reischl

File <PCL_ID3D10VertexShader.h> and
Class <PCL_ID3D10VertexShader> :
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

#ifndef PCL_PCL_ID3D10VERTEXSHADER_CLASS
#define PCL_PCL_ID3D10VERTEXSHADER_CLASS

#include "PCL_ID3D10DeviceChild.h"

/**
* Wrapped ID3D10VertexShader class, or proxy class.
* Derives from PCL_ID3D10DeviceChild, as ID3D10VertexShader derives from ID3D10DeviceChild.
***/
class PCL_ID3D10VertexShader : public PCL_ID3D10DeviceChild
{
public:
	PCL_ID3D10VertexShader(ID3D10VertexShader* pID3D10VertexShader);
	virtual ~PCL_ID3D10VertexShader(void);

private:
	/**
	* The actual D3D10 vertex shader.
	***/
	ID3D10VertexShader* m_pID3D10VertexShader;
};

#endif