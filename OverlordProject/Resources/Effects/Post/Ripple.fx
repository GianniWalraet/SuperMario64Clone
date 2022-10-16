//=============================================================================
//// Shader uses position and texture
//=============================================================================
SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = Mirror;
    AddressV = Mirror;
};

Texture2D gTexture;
float gTotalTime;
float gTimeSinceEnabled;

bool gEnabled = false;
bool gFadeOut = false;

/// Create Depth Stencil State (ENABLE DEPTH WRITING)
/// Create Rasterizer State (Backface culling) 


//IN/OUT STRUCTS
//--------------
struct VS_INPUT
{
    float3 Position : POSITION;
    float2 TexCoord : TEXCOORD0;

};

struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float2 TexCoord : TEXCOORD1;
};


//VERTEX SHADER
//-------------
PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT)0;
    // Set the Position
    output.Position = float4(input.Position, 1.f);
    // Set the TexCoord
    output.TexCoord = input.TexCoord;

    return output;
}


//PIXEL SHADER
//------------
float4 PS(PS_INPUT input) : SV_Target
{
    if (!gEnabled) return float4(gTexture.Sample(samPoint, input.TexCoord).rgb, 1.f);
    float2 uv = input.TexCoord;
    float w = 0.5f - uv.x;
    float h = 0.5f - uv.y ;
    float distanceFromCenter = sqrt(w * w + h * h);
    float sinArg = distanceFromCenter * 10.f - gTotalTime * 10.f;
    float slope = sin(sinArg);
    float3 color = gTexture.Sample(samPoint, uv + normalize(float2(w, h)) * slope * 0.05f).rgb;

    if (gFadeOut)
        color += float3(gTimeSinceEnabled / 3.f, gTimeSinceEnabled / 3.f, gTimeSinceEnabled / 3.f);

    return float4(color, 1.f);
}


//TECHNIQUE
//---------
technique11 Ripple
{
    pass P0
    {
        // Set states...
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}
