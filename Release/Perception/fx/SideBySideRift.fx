// Combines two images into one warped Side-by-Side image

sampler2D TexMap0;
sampler2D TexMap1;

// Apply warp to texture coordinate look up
float2 Warp(float2 Tex : TEXCOORD0)		// Texture coordinate is in a 0.0 to 1.0 range in each direction
{
  float2 newPos = Tex;
  float c = -81.0f/10.0f;				// Distortion coefficient of some sort
  float u = Tex.x*2.0f - 1.0f;			// Texture coordinates converted to -1.0 to 1.0 range
  float v = Tex.y*2.0f - 1.0f;			
  newPos.x = c*u/(pow(v, 2) + c);		// Distortion
  newPos.y = c*v/(pow(u, 2) + c);		
  newPos.x = (newPos.x + 1.0f)*0.5f;	// Convert range back to 0.0 to 1.0 (roughly, more like -1/14 to +15/14) which is why you get the clamped
  newPos.y = (newPos.y + 1.0f)*0.5f;	// textures repeating to the edge where you're out of the texture coord range of 0.0 to 1.0
  return newPos;
}

float4 SBSRift(float2 Tex : TEXCOORD0) : COLOR	// Executed for every pixel on the screen
{
	float4 tColor;
	float2 newPos = Tex;
	if(newPos.x < 0.5)							// Pixel in the left half of the screen
	{
		newPos.x = newPos.x * 2.0f;				// Extend the x texture coordinate to cover the whole input texture, effectively maps full size image to left half of screen (squashed in x)
		tColor = tex2D(TexMap0, Warp(newPos));	// Sample the left image input using the warped texture coordinate
	}
	else										// Pixel in the right half of screen
	{
		newPos.x = (newPos.x - 0.5f) * 2.0f;	// Same as above, obviously different adjustment (map 0.5 to 1.0 range to 0.0 to 1.0 range instead of from the 0.0 to 0.5 range of the left side)
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
