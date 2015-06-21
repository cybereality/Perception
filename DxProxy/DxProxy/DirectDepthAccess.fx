//--------------------------------------------------------------------------------------
// File: DirectDepthAccess.fx
//
// The effect file for the DirectDepthAccess sample.  
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Global variables
//--------------------------------------------------------------------------------------
texture DepthTargetTexture;

sampler DepthSampler = 
sampler_state
{
    Texture = <DepthTargetTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;

    AddressU = Clamp;
    AddressV = Clamp;
};


float4 RenderUnmodified( in float2 OriginalUV : TEXCOORD0 ) : COLOR 
{
    return tex2D(DepthSampler, OriginalUV);
}

technique ShowUnmodified
{
    pass P0
    {        
        PixelShader = compile ps_2_0 RenderUnmodified();
    }
}

// See http://developer.download.nvidia.com/GPU_Programming_Guide/GPU_Programming_Guide_G80.pdf
#define MORE_ACCURATE 1
float4 RenderUnmodifiedRAWZ( in float2 OriginalUV : TEXCOORD0 ) : COLOR 
{
#ifdef MORE_ACCURATE
	float3 rawval = floor( 255.0 * tex2D(DepthSampler, OriginalUV).arg + 0.5); 
	float z = dot( rawval, float3(0.996093809371817670572857294849, 
		0.0038909914428586627756752238080039, 
		1.5199185323666651467481343000015e-5) / 255.0);
#else
	float z = dot(tex2D(DepthSampler, OriginalUV).arg, 
		float3(0.996093809371817670572857294849, 
		0.0038909914428586627756752238080039, 
		1.5199185323666651467481343000015e-5));
#endif

    return z;
}

technique ShowUnmodifiedRAWZ
{
    pass P0
    {        
        PixelShader = compile ps_2_0 RenderUnmodified();
    }
}