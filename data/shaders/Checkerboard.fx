// Combines two images into one Checkerboard image

sampler2D TexMap0;
sampler2D TexMap1;
uniform int viewWidth;
uniform int viewHeight;

float4 Checkerboard(float2 Tex : TEXCOORD0) : COLOR
{
	float4 tColor;
	int col = int((Tex.x*viewWidth) % 2);
	int row = int((Tex.y*viewHeight) % 2);
	if((col == 0 && row == 0) || (col == 1 && row == 1))
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
        PixelShader  = compile ps_2_0 Checkerboard();
    }
}
