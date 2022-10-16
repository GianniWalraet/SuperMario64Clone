float4x4 gWorldViewProj : WORLDVIEWPROJECTION;

TextureCube gTexture : CubeMap;

SamplerState gTextureSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
	AddressW = WRAP;
};
struct VS_INPUT 
{
	float3 pos : POSITION;
};
struct VS_OUTPUT 
{
	float4 pos : SV_POSITION;
	float3 texCoord : TEXCOORD;
};

DepthStencilState EnableDepth
{
	DepthEnable = TRUE;
	DepthWriteMask = ALL;
	DepthFunc = LESS_EQUAL;
};
RasterizerState NoCulling
{
	CullMode = NONE;
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT input) 
{
	VS_OUTPUT output;

	output.pos = mul(float4(input.pos, 0.f), gWorldViewProj).xyww;
	output.texCoord = input.pos;

	return output;
}

//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
float4 PS(VS_OUTPUT input) : SV_TARGET
{
	return gTexture.Sample(gTextureSampler, input.texCoord);
}

//--------------------------------------------------------------------------------------
// Technique
//--------------------------------------------------------------------------------------
technique11 Default
{
	pass P0
	{
		SetRasterizerState(NoCulling);
		SetDepthStencilState(EnableDepth, 0);
		SetVertexShader(CompileShader(vs_4_0, VS()));
		SetGeometryShader(NULL);
		SetPixelShader(CompileShader(ps_4_0, PS()));
	}
}