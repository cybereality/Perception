// Combines two images into one warped Side-by-Side image

sampler2D TexMap0;
sampler2D TexMap1;

float2 LensCenter;
float2 Scale;
float2 ScaleIn;
float4 HmdWarpParam;

// Warp operates on left view, for right, mirror x texture coord before and after calling.
float2 HmdWarp(float2 in01)
{
   float2 theta = (in01 - LensCenter) * ScaleIn;
   float  rSq= theta.x * theta.x + theta.y * theta.y;
   float2 theta1 = theta * (HmdWarpParam.x + HmdWarpParam.y * rSq +
                   HmdWarpParam.z * rSq * rSq + HmdWarpParam.w * rSq * rSq * rSq);
   theta1 = (Scale * theta1) + LensCenter;

   // We're sampling from the full texture not a half texture like in libovr
   theta1.x *= 2.0f;
   return theta1;
}

float4 SBSRift(float2 Tex : TEXCOORD0) : COLOR
{
	float2 newPos = Tex;
	float2 tc;
	float4 tColor;
	float2 theta;

	if(newPos.x < 0.5f) {
		tc = HmdWarp(newPos);
		tColor = tex2D(TexMap0, tc);
	} 
	else {
		newPos.x = (1.0f - newPos.x);	// mirror
		tc = HmdWarp(newPos);
		tc.x = 1.0f - tc.x;		// unmirror
		
		tColor = tex2D(TexMap1, tc);
	}

	if (any(clamp(tc, float2(0.0,0.0), float2(1.0, 1.0)) - tc))
		return 0;

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
