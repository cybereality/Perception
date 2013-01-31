// Combines two images into one warped Side-by-Side image

sampler2D TexMap0;
sampler2D TexMap1;

float2 Warp(float2 Tex : TEXCOORD0)
{
  float2 newPos = Tex;
  float c = -81.0f/10.0f;
  float u = Tex.x*2.0f - 1.0f;
  float v = Tex.y*2.0f - 1.0f;
  newPos.x = c*u/(pow(v, 2) + c);
  newPos.y = c*v/(pow(u, 2) + c);
  newPos.x = (newPos.x + 1.0f)*0.5f;
  newPos.y = (newPos.y + 1.0f)*0.5f;
  return newPos;
}

float4 SBSRift(float2 Tex : TEXCOORD0) : COLOR
{
	float4 tColor;
	float2 newPos = Tex;
	if(newPos.x < 0.5)
	{
		newPos.x = newPos.x * 2.0f;
		tColor = tex2D(TexMap0, Warp(newPos));	
	}
	else 
	{
		newPos.x = (newPos.x - 0.5f) * 2.0f;
		tColor = tex2D(TexMap1, Warp(newPos));
	}
	return tColor;
}

technique ViewShader
{
	pass P0
    {
		VertexShader = null;
        PixelShader  = compile ps_2_0 SBSRift();
    }
}
