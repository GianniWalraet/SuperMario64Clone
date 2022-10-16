//DX10 - PERPIXEL SHADER
//Digital Arts & Entertainment
//PerPixelLighting.fx 
//FirstName: Gianni 
//LastName: Walraet
//Class: 2DAE14N


//GLOBAL VARIABLES
//****************
float4x4 gWorldViewProj : WORLDVIEWPROJECTION;
float4x4 gWorld : WORLD;
float3 gEyePosW;

float4 gColorAmbient : COLOR = float4(1.0f, 1.0f, 1.0f, 1.0f);
float4 gColorDiffuse : COLOR = float4(.9f, .9f, .1f, 1.0f);
float4 gColorSpecular : COLOR = float4(1.0f, 1.0f, 1.0f, 1.0f);

int gShininess = 15;

float3 gLightDirection = float3(-0.577f, -0.577f, 0.577f);

// STRUCTS
// ********
struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR0;
	float2 lighting: COLOR1;
};

//STATES
//******
RasterizerState gRS_NoCulling { CullMode = NONE; };


//MAIN VERTEX SHADER
//******************
VS_OUTPUT MainVS(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	//Transformations
	//Step 1
	float4 position = float4(input.position, 1);
	//Step 2
	float4 worldPosition = mul(position, gWorld);
	//Step 3
	float3 tNormal = normalize(mul(input.normal, (float3x3)gWorld));
	//Step 4
	output.position = mul(position, gWorldViewProj);
	float3 viewDirection = normalize(worldPosition.xyz - gEyePosW);
	//Diffuse
	float diffuseStrength = dot(tNormal, -gLightDirection);
	output.lighting.x = saturate(diffuseStrength);
	//Specular
	float3 halfVector = -normalize(viewDirection + gLightDirection);
	float specularStrength = dot(halfVector, tNormal);
	specularStrength = saturate(specularStrength);
	output.lighting.y = pow(specularStrength, gShininess);
	//Color
	output.color = float4(0.9f, 0.9f, 0.9f, 1);
	return output;
}

//MAIN PIXEL SHADER
//*****************
float4 MainPS(VS_OUTPUT input) : SV_TARGET
{
	//Diffuse
	float3 diffColor = input.color.rgb * gColorDiffuse.rgb;
	diffColor *= input.lighting.x;
	//Specular
	float3 specColor = gColorSpecular.xyz * input.lighting.y;
	//Final Color
	float3 finalColor = diffColor + specColor + gColorAmbient.rgb;
	return float4(finalColor, 1);
}


//TECHNIQUES
//**********
technique10 LightTech
{
	pass p0
	{
		SetRasterizerState(gRS_NoCulling);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}