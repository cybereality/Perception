// Combines two images into an Anaglyph Green/Magenta image

sampler2D TexMap0;
sampler2D TexMap1;

float4 GreenMagenta(float2 Tex : TEXCOORD0) : COLOR
{
	float4 tColor = float4(1.0, 1.0, 1.0, 1.0);
	tColor.g = tex2D(TexMap0, Tex).g;	
	tColor.rb = tex2D(TexMap1, Tex).rb;
		
	return tColor;
}

technique ViewShader
{
	pass P0
    {
		VertexShader = null;
        PixelShader  = compile ps_2_0 GreenMagenta();
    }
}
