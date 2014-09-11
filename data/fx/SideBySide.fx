// Combines two images into one Side-by-Side image

sampler2D TexMap0;
sampler2D TexMap1;
float2 LensCenter;
float2 Scale;

float4 SBS(float2 Tex : TEXCOORD0) : COLOR {

	float2 newPos = Tex;
	
	newPos.y = (Tex.y + LensCenter.y - 1.0f) * Scale[0]/0.146928f + 0.5f;
	
	if( newPos.y < 0.0f || newPos.y > 1.0f ){
		return float4( 0.0f , 0.0f , 0.0f , 1 );
	}
	
	if( newPos.x < 0.5 ){
		newPos.x = Tex.x * 2.0f - LensCenter.x + 0.286325f;
		
		if( newPos.x < 0.0f || newPos.x > 1.0f ){
			return float4( 0.0f , 0.0f , 0.0f , 1 );
		}else{
			return tex2D(TexMap0, newPos);	
		}
	}else{
		newPos.x = (Tex.x - 0.5f) * 2.0f + LensCenter.x - 0.286325f;
		
		if( newPos.x < 0.0f || newPos.x > 1.0f ){
			return float4( 0.0f , 0.0f , 0.0f , 1 );
		}else{
			return tex2D( TexMap1 , newPos );
		}
	}
}

technique ViewShader
{
	pass P0
    {
		VertexShader = null;
        PixelShader  = compile ps_2_0 SBS();
    }
}
