// Combines two images into an Anaglyph Red/Cyan grayscale image

sampler2D TexMap0;
sampler2D TexMap1;

float redLum = 0.2225f;
float greenLum = 0.7169f;
float blueLum = 0.0606;

float4 getGrayscale(float4 inColor)
{
	float gray = inColor.r * redLum + inColor.g * greenLum + inColor.b * blueLum;
	return float4(gray, gray, gray, 1.0);
}

float4 RedCyanGray(float2 Tex : TEXCOORD0) : COLOR
{
	float4 tColor = float4(1.0, 1.0, 1.0, 1.0);
	float4 tColor0 = tex2D(TexMap0, Tex);
	float4 tColor1 = tex2D(TexMap1, Tex);
	
	float4 gray0 = getGrayscale(tColor0);
	float4 gray1 = getGrayscale(tColor1);
	
	tColor.r = gray0.r;	
	tColor.bg = gray1.bg;
		
	return tColor;
}

technique ViewShader
{
	pass P0
    {
		VertexShader = null;
        PixelShader  = compile ps_2_0 RedCyanGray();
    }
}
