// Combines two images into one horizontal Interleaved image

sampler2D TexMap0;
sampler2D TexMap1;
uniform int viewWidth;
uniform int viewHeight;

float4 InterleaveHorz(float2 Tex : TEXCOORD0) : COLOR
{
	float4 tColor;
	if(int((Tex.y*viewHeight) % 2) == 1)
	{
		tColor = tex2D(TexMap0, Tex);	
	}
	else 
	{
		tColor = tex2D(TexMap1, Tex);
	}
	return tColor;
}

technique ViewShader
{
	pass P0
    {
		VertexShader = null;
        PixelShader  = compile ps_2_0 InterleaveHorz();
    }
}
