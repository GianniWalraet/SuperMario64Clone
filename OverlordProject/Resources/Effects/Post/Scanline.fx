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
    float width, height;
    gTexture.GetDimensions(width, height);

    int pixel = height * input.TexCoord.y;
    if (pixel % 2 == 0)
        return float4(0.f, 0.f, 0.f, 1.f);

    float3 color = gTexture.Sample(samPoint, input.TexCoord).rgb;

    return float4(color, 1.0f);
}


//TECHNIQUE
//---------
technique11 Scanline
{
    pass P0
    {
        // Set states...
        SetVertexShader(CompileShader(vs_4_0, VS()));
        SetGeometryShader(NULL);
        SetPixelShader(CompileShader(ps_4_0, PS()));
    }
}