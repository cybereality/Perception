// Combines two images into one warped Side-by-Side image

sampler2D TexMap0;
sampler2D TexMap1;
sampler2D TexMap2;
sampler2D TexMap3;

float ViewportXOffset;
float ViewportYOffset;
float2 LensCenter;
float2 Scale;
float2 ScaleIn;
float4 Chroma;
float2 Resolution;
float4 HmdWarpParam;
float3 Vignette;
float Rotation;
float SmearCorrection;
bool bAverageFrame;

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

float2 rotatePoint(float angle, float2 coord)
{
	if (angle == 0.0f)
		return coord;
	
	float aspect = Resolution.x / Resolution.y;

	float2 newPos = coord;
	
	float s = sin(angle);
	float c = cos(angle);
	

	//New Elliptical Rotation
	// translate point back to origin:
	newPos.x -= 0.5f;
	newPos.y -= 1.0f - LensCenter.y;
	float tempX = newPos.x;

	newPos.y /= aspect;
	newPos.x = newPos.x * c - newPos.y * s;
	newPos.y = tempX * s + newPos.y * c;
	newPos.y *= aspect;
		
	// translate point back:
	newPos.x += 0.5f;
	newPos.y += 1.0f - LensCenter.y; 
	
	return newPos;
}


float4 SBSRift(float2 Tex : TEXCOORD0) : COLOR
{
	float2 newPos = Tex;
	float2 tcRed;
	float2 tcGreen;
	float2 tcBlue;
	float angle = Rotation;
	float3 outColor;
	float3 outColorAverage;


	if (Tex.x > 0.5f) {
		// mirror to get the right-eye distortion
		newPos.x = 1.0f - newPos.x;
		angle = -Rotation;
	}  

	// Chromatic Aberation Correction using coefs from SDK.
	tcBlue = HmdWarp(newPos, float2(Chroma.z, Chroma.w));
	tcBlue = rotatePoint(angle, tcBlue);

	// Clamp on blue, because we expand the blue channel outward the most.
	// Symmetry makes this ok to do before any unmirroring.
	if (any(clamp(tcBlue.xy, float2(0.0,0.0), float2(1.0, 1.0)) - tcBlue.xy))
		return 0;

	tcRed = HmdWarp(newPos, float2(Chroma.x, Chroma.y));
	tcRed = rotatePoint(angle, tcRed);

	tcGreen = HmdWarp(newPos, float2(0.0f, 0.0f));
	tcGreen = rotatePoint(angle, tcGreen);

	if (Tex.x > 0.5f)
	{
		// unmirror the right-eye coords
		tcRed.x = 1 - tcRed.x;
		tcGreen.x = 1 - tcGreen.x;
		tcBlue.x = 1 - tcBlue.x;
	}

	tcRed.x = tcRed.x - ViewportXOffset;
	tcGreen.x = tcGreen.x - ViewportXOffset;
	tcBlue.x = tcBlue.x - ViewportXOffset;

	tcRed.y = tcRed.y - ViewportYOffset;
	tcGreen.y = tcGreen.y - ViewportYOffset;
	tcBlue.y = tcBlue.y - ViewportYOffset;

	if (any(clamp(tcBlue.xy, float2(0.0,0.0), float2(1.0, 1.0)) - tcBlue.xy))
		return 0;

	if(bAverageFrame)
	{
		float fade = 0.75f;
		if (Tex.x > 0.5f)
		{
			outColor.r =  tex2D(TexMap1,   tcRed.xy).r;
			outColor.g =  tex2D(TexMap1,   tcRed.xy).g;
			outColor.b =  tex2D(TexMap1,   tcRed.xy).b;
			outColorAverage.r =  tex2D(TexMap3,   tcRed.xy).r;
			outColorAverage.g =  tex2D(TexMap3,   tcRed.xy).g;
			outColorAverage.b =  tex2D(TexMap3,   tcRed.xy).b;
			outColor.r = lerp(outColor.r, outColorAverage.r, fade);
			outColor.g = lerp(outColor.g, outColorAverage.g, fade);
			outColor.b = lerp(outColor.b, outColorAverage.b, fade);			
		}
		else
		{
			outColor.r =  tex2D(TexMap0,   tcRed.xy).r;
			outColor.g =  tex2D(TexMap0,   tcRed.xy).g;
			outColor.b =  tex2D(TexMap0,   tcRed.xy).b;
			outColorAverage.r =  tex2D(TexMap2,   tcRed.xy).r;
			outColorAverage.g =  tex2D(TexMap2,   tcRed.xy).g;
			outColorAverage.b =  tex2D(TexMap2,   tcRed.xy).b;
			outColor.r = lerp(outColor.r, outColorAverage.r, fade);
			outColor.g = lerp(outColor.g, outColorAverage.g, fade);
			outColor.b = lerp(outColor.b, outColorAverage.b, fade);			
			//outColor.r =  ((tex2D(TexMap2,   tcRed.xy).r) + tex2D(TexMap0,   tcRed.xy).r) / 2;
			//outColor.g =  ((tex2D(TexMap2, tcGreen.xy).g) + tex2D(TexMap0, tcGreen.xy).g) / 2;
			//outColor.b =  ((tex2D(TexMap2, tcBlue.xy).b)  + tex2D(TexMap0, tcBlue.xy).b) / 2;			
		}
	}
	else
	{
		if (Tex.x > 0.5f)
		{
			outColor.r =  tex2D(TexMap1,   tcRed.xy).r;
			outColor.g =  tex2D(TexMap1, tcGreen.xy).g;
			outColor.b =  tex2D(TexMap1,  tcBlue.xy).b;
		}
		else
		{
			outColor.r =  tex2D(TexMap0,   tcRed.xy).r;
			outColor.g =  tex2D(TexMap0, tcGreen.xy).g;
			outColor.b =  tex2D(TexMap0,  tcBlue.xy).b;
		}
	}

	//Are we applying vignette filter?
	float vignetteScaler = 1.0f;

	//values used in the following manner:
	//  x - Where the vignette starts (0.0 -> 1.0, 1.0 is edge of screen)
	//  y - Vignette distance, where fade ends (0.0f for no vignette effect at the edge, ideally x + y == 1.0f)
	//  z - aspect multiplier (should be small number), pushes the vignette towards corners of the screen (rather 
	//      than being simply oval or circular like VorpX
	if (Vignette.x > 0 && Vignette.x < 1.0f)
	{
		//x is halved by subtracting 0.5 if greater than 0.5, y needs to be halved by dividing by 2
		float2 newPos = float2(Tex.x, Tex.y / 2);
			if (Tex.x > 0.5)
				newPos.x = newPos.x - 0.5;
		newPos = float2(abs(newPos.x - 0.25f), abs(newPos.y - 0.25f));

		//Get the position on a scale of 0 to 1 for x and y
		newPos *= 4.0f;

		float aspectScale = 0.0f;
		if (newPos.x == 0.0f && newPos.y == 0.0f)
			aspectScale = 0.0f;
		else if (newPos.x > newPos.y)
			aspectScale = (newPos.y / newPos.x) * Vignette.z;
		else
			aspectScale = (newPos.x / newPos.y) * Vignette.z;

		float dist = sqrt((newPos.x * newPos.x) + (newPos.y * newPos.y)) / (1.0f + aspectScale);
		if (dist > Vignette.x)
		{
			vignetteScaler = 1.0f - ((dist - Vignette.x) / Vignette.y);
			if (vignetteScaler < 0.0f)
				vignetteScaler = 0.0f;
		}
	}

	float4 returnColour = float4(outColor.r * vignetteScaler, outColor.g * vignetteScaler, outColor.b * vignetteScaler, 1.0f);
	if (SmearCorrection != 0.0f)
	{
		returnColour.r = (returnColour.r * (1.0f - (2.0f * SmearCorrection))) + SmearCorrection;
		returnColour.g = (returnColour.g * (1.0f - (2.0f * SmearCorrection))) + SmearCorrection;
		returnColour.b = (returnColour.b * (1.0f - (2.0f * SmearCorrection))) + SmearCorrection;
	}

	return returnColour;
}

technique ViewShader
{
	pass P0
	{
		VertexShader = null;
		PixelShader  = compile ps_3_0 SBSRift();
	}
}