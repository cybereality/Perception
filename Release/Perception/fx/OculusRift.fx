// Combines two images into one warped Side-by-Side image

sampler2D TexMap0;
sampler2D TexMap1;

float2 LensCenter;
float2 Scale;
float2 ScaleIn;
float4 HmdWarpParam;

// Warp operates on left view, for right, mirror x texture coord
// before and after calling.  in02 contains the chromatic aberration
// correction coefficients.
float2 HmdWarp(float2 in01, float2 in02)
{
  float2 theta = (in01 - LensCenter) * ScaleIn;
  float  rSq= theta.x * theta.x + theta.y * theta.y;
  float2 theta1 = theta

                  * (in02.x + in02.y * rSq) // correct chromatic aberr.

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

float4 SBSRift(float2 Tex : TEXCOORD0) : COLOR
{
  float2 newPos = Tex;
  float2 tcRed;
  float2 tcGreen;
  float2 tcBlue;
  float3 outColor;

  if (Tex.x > 0.5f) {
    // mirror to get the right-eye distortion
    newPos.x = 1.0f - newPos;
  }


  // TODO chromaberr params hardcoded for DK1 with A-cup lenses; need to pass in
  // from SDK.
  tcBlue = HmdWarp(newPos, float2(1.014, 0.0f));

  // Clamp on blue, because we expand the blue channel outward the most.
  // Symmetry makes this ok to do before any unmirroring.
  if (any(clamp(tcBlue, float2(0.0,0.0), float2(1.0, 1.0)) - tcBlue))
    return 0;

  tcRed = HmdWarp(newPos, float2(0.996f, -.004f));
  tcGreen = HmdWarp(newPos, float2(1.0f, 0.0f));


  if (Tex.x > 0.5f) {
    // unmirror the right-eye coords
    tcRed.x = 1 - tcRed.x;
    tcGreen.x = 1 - tcGreen.x;
    tcBlue.x = 1 - tcBlue.x;

    outColor.r = tex2D(TexMap1, tcRed).r;
    outColor.g = tex2D(TexMap1, tcGreen).g;
    outColor.b = tex2D(TexMap1, tcBlue).b;
  }
  else {
    outColor.r = tex2D(TexMap0, tcRed).r;
    outColor.g = tex2D(TexMap0, tcGreen).g;
    outColor.b = tex2D(TexMap0, tcBlue).b;
  }

  return float4(outColor.r, outColor.g, outColor.b, 1.0f);
}

technique ViewShader
{
  pass P0
  {
    VertexShader = null;
    PixelShader  = compile ps_2_0 SBSRift();
  }
}