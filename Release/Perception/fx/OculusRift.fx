// Combines two images into one warped Side-by-Side image

sampler2D TexMap0;
sampler2D TexMap1;

float ViewportXOffset;
float ViewportYOffset;
float2 LensCenter;
float2 Scale;
float2 ScaleIn;
float4 Chroma;
float2 Resolution;
float4 HmdWarpParam;

// Warp operates on left view, for right, mirror x texture coord
// before and after calling.  in02 contains the chromatic aberration
// correction coefficients.
float2 HmdWarp(float2 in01, float2 in02)
{
  float2 theta = (in01 - LensCenter) * ScaleIn;
  float  rSq= theta.x * theta.x + theta.y * theta.y;
  float2 theta1 = theta

                  * ((1.0f + in02.x) + in02.y * rSq) // correct chromatic aberr.

                  * (HmdWarpParam.x +       // correct lens distortion
                     HmdWarpParam.y * rSq +
                     HmdWarpParam.z * rSq * rSq +
                     HmdWarpParam.w * rSq * rSq * rSq);

  theta1 = (Scale * theta1) + LensCenter;
  theta1.x = theta1.x - (LensCenter.x-0.25f);
  theta1.y = theta1.y - (LensCenter.y-0.5f);

  // We're sampling from the full texture not a half texture like in libovr
  theta1.x *= 2.0f;

  return theta1;
}

float4 HorizSuperSampledWarp(float2 in01, float2 in02)
{
  float2 output_tc1 = HmdWarp(in01 + float2(-.0666666/Resolution.x, 0.0f), in02);
  float2 output_tc2 = HmdWarp(in01 + float2( .0666666/Resolution.x, 0.0f), in02);

  return float4(output_tc1.x, output_tc1.y, output_tc2.x, output_tc2.y);
}

float4 SBSRift(float2 Tex : TEXCOORD0) : COLOR
{
  float2 newPos = Tex;
  float4 tcRed0;
  float4 tcGreen0;
  float4 tcBlue0;
  float4 tcRed1;
  float4 tcGreen1;
  float4 tcBlue1;

  float subpixelShiftR = -0.33333/Resolution.x;
  float subpixelShiftB = 0.33333/Resolution.x;

  float3 outColor;

  if (Tex.x > 0.5f) {
    // mirror to get the right-eye distortion
    newPos.x = 1.0f - newPos.x;
	// subpixel alignment isn't symmetric under mirroring
    subpixelShiftR = 0.33333/Resolution.x;
    subpixelShiftB = -0.33333/Resolution.x;
  }  

  // Chromatic Aberation Correction using coefs from SDK.
  tcBlue0 = HorizSuperSampledWarp(newPos + float2(subpixelShiftB, -0.066666/Resolution.y), float2(Chroma.z, Chroma.w));
  tcBlue1 = HorizSuperSampledWarp(newPos + float2(subpixelShiftB,  0.066666/Resolution.y), float2(Chroma.z, Chroma.w));

  // Clamp on blue, because we expand the blue channel outward the most.
  // Symmetry makes this ok to do before any unmirroring.
  if (any(clamp(tcBlue0.xy, float2(0.0,0.0), float2(1.0, 1.0)) - tcBlue0.xy))
    return 0;

  tcRed0   = HorizSuperSampledWarp(newPos + float2(subpixelShiftR, -0.25f/Resolution.y), float2(Chroma.x, Chroma.y));
  tcRed1   = HorizSuperSampledWarp(newPos + float2(subpixelShiftR,  0.25f/Resolution.y), float2(Chroma.x, Chroma.y));
  tcGreen0 = HorizSuperSampledWarp(newPos + float2(0.0, -0.25f/Resolution.y), float2(0.0f, 0.0f));
  tcGreen1 = HorizSuperSampledWarp(newPos + float2(0.0,  0.25f/Resolution.y), float2(0.0f, 0.0f));
  
  if (Tex.x > 0.5f) {
    // unmirror the right-eye coords
    tcRed0.x = 1 - tcRed0.x;
    tcRed0.z = 1 - tcRed0.z;
    tcGreen0.x = 1 - tcGreen0.x;
    tcGreen0.z = 1 - tcGreen0.z;
    tcBlue0.x = 1 - tcBlue0.x;
    tcBlue0.z = 1 - tcBlue0.z;

    tcRed1.x = 1 - tcRed1.x;
    tcRed1.z = 1 - tcRed1.z;
    tcGreen1.x = 1 - tcGreen1.x;
    tcGreen1.z = 1 - tcGreen1.z;
    tcBlue1.x = 1 - tcBlue1.x;
    tcBlue1.z = 1 - tcBlue1.z;

	tcRed0.x = tcRed0.x - ViewportXOffset;
	tcGreen0.x = tcGreen0.x - ViewportXOffset;
	tcBlue0.x = tcBlue0.x - ViewportXOffset;
	tcRed1.x = tcRed1.x - ViewportXOffset;
	tcGreen1.x = tcGreen1.x - ViewportXOffset;
	tcBlue1.x = tcBlue1.x - ViewportXOffset;	
	tcRed0.z = tcRed0.z - ViewportXOffset;
	tcGreen0.z = tcGreen0.z - ViewportXOffset;
	tcBlue0.z = tcBlue0.z - ViewportXOffset;
	tcRed1.z = tcRed1.z - ViewportXOffset;
	tcGreen1.z = tcGreen1.z - ViewportXOffset;
	tcBlue1.z = tcBlue1.z - ViewportXOffset;
	
	tcRed0.y = tcRed0.y - ViewportYOffset;
	tcGreen0.y = tcGreen0.y - ViewportYOffset;
	tcBlue0.y = tcBlue0.y - ViewportYOffset;
	tcRed1.y = tcRed1.y - ViewportYOffset;
	tcGreen1.y = tcGreen1.y - ViewportYOffset;
	tcBlue1.y = tcBlue1.y - ViewportYOffset;	
	tcRed0.w = tcRed0.w - ViewportYOffset;
	tcGreen0.w = tcGreen0.w - ViewportYOffset;
	tcBlue0.w = tcBlue0.w - ViewportYOffset;
	tcRed1.w = tcRed1.w - ViewportYOffset;
	tcGreen1.w = tcGreen1.w - ViewportYOffset;
	tcBlue1.w = tcBlue1.w - ViewportYOffset;

	if(tcRed0.x >= 1.0f || tcRed0.y <= 0.0f || tcRed0.x <= 0.0f || tcRed0.y >= 1.0f)
		return 0;

    outColor.r =  (tex2D(TexMap1,   tcRed0.xy).r + tex2D(TexMap1,   tcRed0.zw).r);
    outColor.r += (tex2D(TexMap1,   tcRed1.xy).r + tex2D(TexMap1,   tcRed1.zw).r);
    outColor.r /= 4.0f; // 4 samples
    outColor.g =  (tex2D(TexMap1, tcGreen0.xy).g + tex2D(TexMap1, tcGreen0.zw).g);
    outColor.g += (tex2D(TexMap1, tcGreen1.xy).g + tex2D(TexMap1, tcGreen1.zw).g);
    outColor.g /= 4.0f; // 4 samples
    outColor.b =  (tex2D(TexMap1,  tcBlue0.xy).b + tex2D(TexMap1,  tcBlue0.zw).b);
    outColor.b +=  (tex2D(TexMap1,  tcBlue1.xy).b + tex2D(TexMap1,  tcBlue1.zw).b);
    outColor.b /= 4.0f; // 4 samples
  }
  else {
	
	tcRed0.x = tcRed0.x - ViewportXOffset;
	tcGreen0.x = tcGreen0.x - ViewportXOffset;
	tcBlue0.x = tcBlue0.x - ViewportXOffset;
	tcRed1.x = tcRed1.x - ViewportXOffset;
	tcGreen1.x = tcGreen1.x - ViewportXOffset;
	tcBlue1.x = tcBlue1.x - ViewportXOffset;
	
	tcRed0.z = tcRed0.z - ViewportXOffset;
	tcGreen0.z = tcGreen0.z - ViewportXOffset;
	tcBlue0.z = tcBlue0.z - ViewportXOffset;
	tcRed1.z = tcRed1.z - ViewportXOffset;
	tcGreen1.z = tcGreen1.z - ViewportXOffset;
	tcBlue1.z = tcBlue1.z - ViewportXOffset;

	tcRed0.y = tcRed0.y - ViewportYOffset;
	tcGreen0.y = tcGreen0.y - ViewportYOffset;
	tcBlue0.y = tcBlue0.y - ViewportYOffset;
	tcRed1.y = tcRed1.y - ViewportYOffset;
	tcGreen1.y = tcGreen1.y - ViewportYOffset;
	tcBlue1.y = tcBlue1.y - ViewportYOffset;	
	tcRed0.w = tcRed0.w - ViewportYOffset;
	tcGreen0.w = tcGreen0.w - ViewportYOffset;
	tcBlue0.w = tcBlue0.w - ViewportYOffset;
	tcRed1.w = tcRed1.w - ViewportYOffset;
	tcGreen1.w = tcGreen1.w - ViewportYOffset;
	tcBlue1.w = tcBlue1.w - ViewportYOffset;

	if(tcRed0.x >= 1.0f || tcRed0.y <= 0.0f || tcRed0.x <= 0.0f || tcRed0.y >= 1.0f)
		return 0;

	outColor.r =  (tex2D(TexMap0,   tcRed0.xy).r + tex2D(TexMap0,   tcRed0.zw).r);
    outColor.r += (tex2D(TexMap0,   tcRed1.xy).r + tex2D(TexMap0,   tcRed1.zw).r);
    outColor.r /= 4.0f; // 4 samples
    outColor.g =  (tex2D(TexMap0, tcGreen0.xy).g + tex2D(TexMap0, tcGreen0.zw).g);
    outColor.g += (tex2D(TexMap0, tcGreen1.xy).g + tex2D(TexMap0, tcGreen1.zw).g);
    outColor.g /= 4.0f; // 4 samples
    outColor.b =  (tex2D(TexMap0,  tcBlue0.xy).b + tex2D(TexMap0,  tcBlue0.zw).b);
    outColor.b +=  (tex2D(TexMap0,  tcBlue1.xy).b + tex2D(TexMap0,  tcBlue1.zw).b);
    outColor.b /= 4.0f; // 4 samples
  }

  return float4(outColor.r, outColor.g, outColor.b, 1.0f);
}

technique ViewShader
{
  pass P0
  {
    VertexShader = null;
    PixelShader  = compile ps_3_0 SBSRift();
  }
}
