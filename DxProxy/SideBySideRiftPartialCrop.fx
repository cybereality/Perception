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
	float2 warpedTexCoord;
	
	// This is all pretty lazy, I haven't generalised for a variable vertical percentage so it's fixed at 80%.
	//float screenRatio = 1.6f // 16:10 screen, use 1.777777777f for 16:9 screen
	//float visibleRatio = 1f; // 16:10 screen, use 0.9f for 16:9 screen. These numbers would need to change 
	// for a different vertical %. I've left all those calculations out
	// These numbers are constants for the screen size and a given vertical usage, so rather than calculating them for every pixel
	// I'm just putting the number directly into xRangeToUse. Normally this would be done outside the shader
	// and passed in.
	
	float xRangeToUse = 0.625f; //visibleRatio / screenRatio;
	float xHalfOfRangeNottoUse = 0.1875f; // (1f - xRangeToUse) * 0.5f;
	newPos.y = newPos.y * 1.25f - 0.125f; // 80% of vertical. 
		
	if(newPos.x < 0.5)
	{
		newPos.x = (newPos.x * 2.0f * xRangeToUse) + xHalfOfRangeNottoUse;
		warpedTexCoord = Warp(newPos);
		tColor = tex2D(TexMap0, warpedTexCoord);	
	}
	else 
	{
		newPos.x = (((newPos.x - 0.5f) * 2.0f)* xRangeToUse) + xHalfOfRangeNottoUse;
		warpedTexCoord = Warp(newPos);
		tColor = tex2D(TexMap1, warpedTexCoord);	
	}
	
	// remove everything between here and the next comment except "return tColor;" if you want the clamped texture around the edge instead of black
	if ((warpedTexCoord.y > 1.0f) || (warpedTexCoord.y < 0.0f)) {  
		return 0;
	}
	else {
		return tColor;
	}
	// This is the next comment
}

technique ViewShader
{
	pass P0
    {
		VertexShader = null;
        PixelShader  = compile ps_2_0 SBSRift();
    }
}
