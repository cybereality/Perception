// Combines two images into an Anaglyph Red/Cyan image

sampler2D TexMap0;
sampler2D TexMap1;

float4 RedCyan(float2 Tex : TEXCOORD0) : COLOR
{
	float4 tColor = float4(1.0, 1.0, 1.0, 1.0);
	tColor.r = tex2D(TexMap0, Tex).r;	
	tColor.bg = tex2D(TexMap1, Tex).bg;
		
	return tColor;
}

technique ViewShader
{
	pass P0
    {
		VertexShader = null;
        PixelShader  = compile ps_2_0 RedCyan();
    }
}
