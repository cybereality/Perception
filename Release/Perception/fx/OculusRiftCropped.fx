// Combines two images into one warped Side-by-Side image

sampler2D TexMap0;
sampler2D TexMap1;

float2 LensCenter;
float2 ScreenCenter;
float2 Scale;
float2 ScaleIn;
float4 HmdWarpParam;

float2 HmdWarp(float2 in01)
{
   float2 theta = (in01 - LensCenter) * ScaleIn;
   float  rSq= theta.x * theta.x + theta.y * theta.y;
   float2 theta1 = theta * (HmdWarpParam.x + HmdWarpParam.y * rSq +
                   HmdWarpParam.z * rSq * rSq + HmdWarpParam.w * rSq * rSq * rSq);
   return LensCenter + Scale * theta1;
}

float4 SBSRift(float2 Tex : TEXCOORD0) : COLOR
{
	float2 newPos = Tex;
	float2 tc;
	float4 tColor;

	if(newPos.x < 0.5f) {
		tc = HmdWarp(newPos);

		tc.x = tc.x  + 0.25f - (LensCenter.x+LensShift.x -0.25f);  //offset for ipd

		tColor = tex2D(TexMap0,tc);
	} else {
		newPos.x = (1.0f - newPos.x);	// mirror
		tc = HmdWarp(newPos);

		tc.x = tc.x  + 0.25f - (LensCenter.x+LensShift.x -0.25f);  //offset for ipd

		tc.x = 1.0f -tc.x;		// unmirror
		tColor = tex2D(TexMap1,tc);
	}

	tc.x = tc.x / 2.0f;

	if (any(clamp(tc, ScreenCenter-float2(0.25,0.5), ScreenCenter+float2(0.25, 0.5)) - tc)){
		return 0;
//		tColor[0] = 1.0f;
//		tColor[1] = 1.0f;
//		tColor[2] = 1.0f;
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
