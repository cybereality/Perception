// Combines two images into one Over/Under image

sampler2D TexMap0;
sampler2D TexMap1;

float4 OverUnder(float2 Tex : TEXCOORD0) : COLOR
{
	float4 tColor;
	float2 newPos = Tex;
	if(newPos.y < 0.5)
	{
		newPos.y = newPos.y * 2.0f;
		tColor = tex2D(TexMap0, newPos);	
	}
	else 
	{
		newPos.y = (newPos.y - 0.5f) * 2.0f;
		tColor = tex2D(TexMap1, newPos);
	}
	return tColor;
}

technique ViewShader
{
	pass P0
    {
		VertexShader = null;
        PixelShader  = compile ps_2_0 OverUnder();
    }
}
