/********************************************************************
Vireio Perception: Open-Source Stereoscopic 3D Driver
Copyright (C) 2012 Andres Hernandez

Vireio Cinema - Virtual Reality Cinema Node Plugin 
Copyright (C) 2014 Denis Reischl

File <Shader_D3D9.h> and all Shaders :
Copyright (C) 2015 Denis Reischl

 



Vireio Perception Version History:
v1.0.0 2012 by Andres Hernandez
v1.0.X 2013 by John Hicks, Neil Schneider
v1.1.x 2013 by Primary Coding Author: Chris Drain
Team Support: John Hicks, Phil Larkson, Neil Schneider
v2.0.x 2013 by Denis Reischl, Neil Schneider, Joshua Brown
v2.0.4 onwards 2014 by Grant Bagwell, Simon Brown and Neil Schneider
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

/**
* Textured, colored vertex.
***/
struct TexColoredVertex
{ 
	D3DXVECTOR3 Pos;
	D3DCOLOR    Col;
	float       U, V;
};

/**
* Simplest vertex shader. vs1.1.
***/
static const char* VSSimplest =
	"struct VertexToPixel\n"
	"{\n"
	"float4 Position     : POSITION;\n"
	"float2 TexCoords    : TEXCOORD0;\n"
	"};\n"

	"float4x4 xViewProjection;\n"

	"VertexToPixel SimplestVertexShader( float4 inPos : POSITION, float2 inTexCoords : TEXCOORD0)\n"
	"{\n"
	"VertexToPixel Output = (VertexToPixel)0;\n"

	"Output.Position = mul(inPos, xViewProjection);\n"
	"Output.TexCoords = inTexCoords;\n"

	"return Output;  \n"  
	"}\n";

/**
* Simplest pixel shader. ps1.1. ?? use ps3_0 to compile
***/
static const char* PSSimplest =
	"struct VertexToPixel\n"
	"{\n"
	"float4 Position     : POSITION;\n"
	"float2 TexCoords    : TEXCOORD0;\n"
	"};\n"

	"struct PixelToFrame\n"
	"{\n"
	"   float4 Color : COLOR0;\n"
	"};\n"

	"Texture xColoredTexture;\n"

	"sampler ColoredTextureSampler = sampler_state { texture = <xColoredTexture> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = wrap; AddressV = wrap;};\n"

	"PixelToFrame OurFirstPixelShader(VertexToPixel PSIn)\n"
	"{\n"
	"PixelToFrame Output = (PixelToFrame)0;\n"

	"Output.Color = tex2D(ColoredTextureSampler, PSIn.TexCoords);\n"

	"return Output;\n"
	"}\n";

/**
* Texture + Color vertex shader. vs1.1.
***/
static const char* VSTexColor =
	"struct VertexToPixel\n"
	"{\n"
	"float4 Position     : POSITION;\n"
	"float4 Color        : COLOR;\n"
	"float2 TexCoords    : TEXCOORD0;\n"
	"};\n"

	"float4x4 xViewProjection;\n"

	"VertexToPixel TexColorVertexShader( float4 inPos : POSITION, float4 inColor : COLOR, float2 inTexCoords : TEXCOORD0)\n"
	"{\n"
	"VertexToPixel Output = (VertexToPixel)0;\n"

	"Output.Position = mul(inPos, xViewProjection);\n"
	"Output.Color = inColor;\n"
	"Output.TexCoords = inTexCoords;\n"

	"return Output;  \n"  
	"}\n";

/**
* Simplest pixel shader. ps1.1. ?? use ps3_0 to compile
***/
static const char* PSTexColor =
	"struct VertexToPixel\n"
	"{\n"
	"float4 Position     : POSITION;\n"
	"float4 Color        : COLOR;\n"
	"float2 TexCoords    : TEXCOORD0;\n"
	"};\n"

	"struct PixelToFrame\n"
	"{\n"
	"   float4 Color : COLOR0;\n"
	"};\n"

	"Texture xColoredTexture;\n"

	"sampler ColoredTextureSampler = sampler_state { texture = <xColoredTexture> ; magfilter = LINEAR; minfilter = LINEAR; mipfilter=LINEAR; AddressU = wrap; AddressV = wrap;};\n"

	"PixelToFrame TexColorPixelShader(VertexToPixel PSIn)\n"
	"{\n"
	"PixelToFrame Output = (PixelToFrame)0;\n"

	"Output.Color = PSIn.Color * tex2D(ColoredTextureSampler, PSIn.TexCoords);\n"

	"return Output;\n"
	"}\n";

/**
* >OculusRoomTiny< fields. To be deleted.
***/
int                    numVertices, numIndices;
TexColoredVertex       Vertices[2000]; // Note fixed maximum
UINT16                 Indices[2000];
D3DCOLOR               tex_pixels[4][256*256];
void AddVertex(const TexColoredVertex& v) { if (numVertices < 2000) Vertices[numVertices++] = v; }
void AddIndex(UINT16 a)                   { if (numIndices < 2000) Indices[numIndices++] = a; }

/**
* Method taken from the LibOVR sample >OculusRommTiny<.
***/
void AddSolidColorBox(float x1, float y1, float z1, float x2, float y2, float z2, D3DXCOLOR c)
{
	D3DXVECTOR3 Vert[][2] =
	{   
		D3DXVECTOR3(x1, y2, z1), D3DXVECTOR3(z1, x1, 0.0f),  D3DXVECTOR3(x2, y2, z1), D3DXVECTOR3(z1, x2, 0.0f),
		D3DXVECTOR3(x2, y2, z2), D3DXVECTOR3(z2, x2, 0.0f),  D3DXVECTOR3(x1, y2, z2), D3DXVECTOR3(z2, x1, 0.0f),
		D3DXVECTOR3(x1, y1, z1), D3DXVECTOR3(z1, x1, 0.0f),  D3DXVECTOR3(x2, y1, z1), D3DXVECTOR3(z1, x2, 0.0f),
		D3DXVECTOR3(x2, y1, z2), D3DXVECTOR3(z2, x2, 0.0f),  D3DXVECTOR3(x1, y1, z2), D3DXVECTOR3(z2, x1, 0.0f),
		D3DXVECTOR3(x1, y1, z2), D3DXVECTOR3(z2, y1, 0.0f),  D3DXVECTOR3(x1, y1, z1), D3DXVECTOR3(z1, y1, 0.0f),
		D3DXVECTOR3(x1, y2, z1), D3DXVECTOR3(z1, y2, 0.0f),  D3DXVECTOR3(x1, y2, z2), D3DXVECTOR3(z2, y2, 0.0f),
		D3DXVECTOR3(x2, y1, z2), D3DXVECTOR3(z2, y1, 0.0f),  D3DXVECTOR3(x2, y1, z1), D3DXVECTOR3(z1, y1, 0.0f),
		D3DXVECTOR3(x2, y2, z1), D3DXVECTOR3(z1, y2, 0.0f),  D3DXVECTOR3(x2, y2, z2), D3DXVECTOR3(z2, y2, 0.0f),
		D3DXVECTOR3(x1, y1, z1), D3DXVECTOR3(x1, y1, 0.0f),  D3DXVECTOR3(x2, y1, z1), D3DXVECTOR3(x2, y1, 0.0f),
		D3DXVECTOR3(x2, y2, z1), D3DXVECTOR3(x2, y2, 0.0f),  D3DXVECTOR3(x1, y2, z1), D3DXVECTOR3(x1, y2, 0.0f),
		D3DXVECTOR3(x1, y1, z2), D3DXVECTOR3(x1, y1, 0.0f),  D3DXVECTOR3(x2, y1, z2), D3DXVECTOR3(x2, y1, 0.0f),
		D3DXVECTOR3(x2, y2, z2), D3DXVECTOR3(x2, y2, 0.0f),  D3DXVECTOR3(x1, y2, z2), D3DXVECTOR3(x1, y2, 0.0f), 
	};

	UINT16 CubeIndices[] = {0, 1, 3,     3, 1, 2,     5, 4, 6,     6, 4, 7,
		8, 9, 11,    11, 9, 10,   13, 12, 14,  14, 12, 15,
		16, 17, 19,  19, 17, 18,  21, 20, 22,  22, 20, 23 };

	for(int i = 0; i < 36; i++)
		AddIndex(CubeIndices[i] + (UINT16) numVertices);

	for(int v = 0; v < 24; v++)
	{
		TexColoredVertex vvv; vvv.Pos = Vert[v][0];  vvv.U = Vert[v][1].x; vvv.V = Vert[v][1].y;
		float dist1 = D3DXVec3Length(&(vvv.Pos - D3DXVECTOR3(-2,4,-2)));
		float dist2 = D3DXVec3Length(&(vvv.Pos - D3DXVECTOR3(3,4,-3)));
		float dist3 = D3DXVec3Length(&(vvv.Pos - D3DXVECTOR3(-4,3,25)));
		int   bri   = rand() % 160;
		float RRR   = (c.r * 255.0f) * (bri + 192.0f*(0.65f + 8/dist1 + 1/dist2 + 4/dist3)) / 255.0f;
		float GGG   = (c.g * 255.0f) * (bri + 192.0f*(0.65f + 8/dist1 + 1/dist2 + 4/dist3)) / 255.0f;
		float BBB   = (c.b * 255.0f) * (bri + 192.0f*(0.65f + 8/dist1 + 1/dist2 + 4/dist3)) / 255.0f;
		int r = RRR > 255 ? 255: (int) RRR;
		int g = GGG > 255 ? 255: (int) GGG;
		int b = BBB > 255 ? 255: (int) BBB;
		vvv.Col = D3DCOLOR_ARGB(255, r, g, b);
		AddVertex(vvv);
	}
}

/**
* Generates the >OculusRoomTiny< textures.
***/
void GenerateOculusRoomTinyTextures()
{
	// Construct textures
	for (int k=0;k<4;k++)
	{
		for (int j = 0; j < 256; j++)
			for (int i = 0; i < 256; i++)
			{
				if (k==0) tex_pixels[0][j*256+i] = 
					(((i >> 7) ^ (j >> 7)) & 1) ? 
					D3DCOLOR_ARGB(180,180,180,255) : D3DCOLOR_ARGB(80,80,80,255);// floor
				if (k==1) tex_pixels[1][j*256+i] = 
					(((j/4 & 15) == 0) || (((i/4 & 15) == 0) && ((((i/4 & 31) == 0) ^ ((j/4 >> 4) & 1)) == 0))) ?
					D3DCOLOR_ARGB(60,60,60,255) : D3DCOLOR_ARGB(180,180,180,255); //wall
				if (k==2) tex_pixels[2][j*256+i] = 
					(i/4 == 0 || j/4 == 0) ? 
					D3DCOLOR_ARGB(80,80,80,255) : D3DCOLOR_ARGB(180,180,180,255);// ceiling
				if (k==3) tex_pixels[3][j*256+i] = 
					D3DCOLOR_ARGB(128,128,128,255);// blank
			}
	}
}

/**
* Generates the >OculusRoomTiny< mesh.
***/
void GenerateOculusRoomTinyMesh()
{
	// Construct geometry
	AddSolidColorBox( 0, 0, 0,  +1.0f,  +1.0f, 1.0f,  D3DCOLOR_ARGB(255,64,64,64)); 

	AddSolidColorBox( -10.1f,   0.0f,  -20.0f, -10.0f,  4.0f,  20.0f, D3DCOLOR_ARGB(255,192,128,128)); // Left Wall
	AddSolidColorBox( -10.0f,  -0.1f,  -20.1f,  10.0f,  4.0f, -20.0f, D3DCOLOR_ARGB(255,128,192,128)); // Back Wall
	AddSolidColorBox(  10.0f,  -0.1f,  -20.0f,  10.1f,  4.0f,  20.0f, D3DCOLOR_ARGB(255,128,128,192));  // Right Wall

	AddSolidColorBox( -10.0f,  -0.1f,  -20.0f,  10.0f,  0.0f, 20.1f,  D3DCOLOR_ARGB(255,192,128,192)); // Main floor
	AddSolidColorBox( -15.0f,  -6.1f,   18.0f,  15.0f, -6.0f, 30.0f,  D3DCOLOR_ARGB(255,128,192,128) );// Bottom floor

	AddSolidColorBox( -10.0f,  4.0f,  -20.0f,  10.0f,  4.1f, 20.1f,  D3DCOLOR_ARGB(255,128,128,128)); 

	AddSolidColorBox(   9.5f,   0.75f,  3.0f,  10.1f,  2.5f,   3.1f,  D3DCOLOR_ARGB(255,0,96,96) );   // Right side shelf// Verticals
	AddSolidColorBox(   9.5f,   0.95f,  3.7f,  10.1f,  2.75f,  3.8f,  D3DCOLOR_ARGB(255,0,96,96) );   // Right side shelf
	AddSolidColorBox(   9.55f,  1.20f,  2.5f,  10.1f,  1.30f,  3.75f,  D3DCOLOR_ARGB(255,96,96,0) ); // Right side shelf// Horizontals
	AddSolidColorBox(   9.55f,  2.00f,  3.05f,  10.1f,  2.10f,  4.2f,  D3DCOLOR_ARGB(255,96,96,0) ); // Right side shelf
	AddSolidColorBox(   5.0f,   1.1f,   20.0f,  10.0f,  1.2f,  20.1f, D3DCOLOR_ARGB(255,96,0,96) );   // Right railing   
	AddSolidColorBox(  -10.0f,  1.1f, 20.0f,   -5.0f,   1.2f, 20.1f, D3DCOLOR_ARGB(255,96,0,96) );   // Left railing  
	for (float f=5.0f;f<=9.0f;f+=1.0f)
	{
		AddSolidColorBox(   f,   0.0f,   20.0f,   f+0.1f,  1.1f,  20.1f, D3DCOLOR_ARGB(255,128,128,128) );// Left Bars
		AddSolidColorBox(  -f,   1.1f,   20.0f,  -f-0.1f,  0.0f,  20.1f, D3DCOLOR_ARGB(255,128,128,128) );// Right Bars
	}
	AddSolidColorBox( -1.8f, 0.8f, 1.0f,   0.0f,  0.7f,  0.0f,   D3DCOLOR_ARGB(255,128,128,0)); // Table
	AddSolidColorBox( -1.8f, 0.0f, 0.0f,  -1.7f,  0.7f,  0.1f,   D3DCOLOR_ARGB(255,128,128,0)); // Table Leg 
	AddSolidColorBox( -1.8f, 0.7f, 1.0f,  -1.7f,  0.0f,  0.9f,   D3DCOLOR_ARGB(255,128,128,0)); // Table Leg 
	AddSolidColorBox(  0.0f, 0.0f, 1.0f,  -0.1f,  0.7f,  0.9f,   D3DCOLOR_ARGB(255,128,128,0)); // Table Leg 
	AddSolidColorBox(  0.0f, 0.7f, 0.0f,  -0.1f,  0.0f,  0.1f,   D3DCOLOR_ARGB(255,128,128,0)); // Table Leg 
	AddSolidColorBox( -1.4f, 0.5f, -1.1f, -0.8f,  0.55f, -0.5f,  D3DCOLOR_ARGB(255,44,44,128) ); // Chair Set
	AddSolidColorBox( -1.4f, 0.0f, -1.1f, -1.34f, 1.0f,  -1.04f, D3DCOLOR_ARGB(255,44,44,128) ); // Chair Leg 1
	AddSolidColorBox( -1.4f, 0.5f, -0.5f, -1.34f, 0.0f,  -0.56f, D3DCOLOR_ARGB(255,44,44,128) ); // Chair Leg 2
	AddSolidColorBox( -0.8f, 0.0f, -0.5f, -0.86f, 0.5f,  -0.56f, D3DCOLOR_ARGB(255,44,44,128) ); // Chair Leg 2
	AddSolidColorBox( -0.8f, 1.0f, -1.1f, -0.86f, 0.0f,  -1.04f, D3DCOLOR_ARGB(255,44,44,128) ); // Chair Leg 2
	AddSolidColorBox( -1.4f, 0.97f,-1.05f,-0.8f,  0.92f, -1.10f, D3DCOLOR_ARGB(255,44,44,128) ); // Chair Back high bar

	for (float f=3.0f;f<=6.6f;f+=0.4f)
		AddSolidColorBox( -3,  0.0f, f,   -2.9f, 1.3f, f+0.1f, D3DCOLOR_ARGB(255,64,64,64) );//Posts
}
