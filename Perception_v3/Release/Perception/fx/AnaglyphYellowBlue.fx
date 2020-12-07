// Combines two images into an Anaglyph Yellow/Blue image

sampler2D TexMap0;
sampler2D TexMap1;

float4 YellowBlue(float2 Tex : TEXCOORD0) : COLOR
{
	float4 tColor = float4(1.0, 1.0, 1.0, 1.0);
	tColor.rg = tex2D(TexMap0, Tex).rg;	
	tColor.b = tex2D(TexMap1, Tex).b;
		
	return tColor;
}

technique ViewShader
{
	pass P0
    {
		VertexShader = null;
        PixelShader  = compile ps_2_0 YellowBlue();
    }
}
