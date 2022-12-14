//INCOMPLETE!

float4x4 gTransform : WorldViewProjection;
Texture2D gSpriteTexture;
float2 gTextureSize;

SamplerState samPoint
{
    Filter = MIN_MAG_MIP_POINT;
    AddressU = WRAP;
    AddressV = WRAP;
};

BlendState EnableBlending
{
    BlendEnable[0] = TRUE;
    SrcBlend = SRC_ALPHA;
    DestBlend = INV_SRC_ALPHA;
};

DepthStencilState NoDepth
{
    DepthEnable = FALSE;
};

RasterizerState BackCulling
{
    CullMode = BACK;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
    uint TextureId : TEXCOORD0;
    float4 TransformData : POSITION; //PosX, PosY, Depth (PosZ), Rotation
    float4 TransformData2 : POSITION1; //PivotX, PivotY, ScaleX, ScaleY
    float4 Color : COLOR;
};

struct GS_DATA
{
    float4 Position : SV_POSITION;
    float4 Color : COLOR;
    float2 TexCoord : TEXCOORD0;
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
    return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, float rotation, float2 rotCosSin, float2 offset, float2 pivotOffset)
{   
    if (rotation != 0)
    {
        //Do rotation calculations
        pos.xy -= pivotOffset;
        //Transform to origin
        pos.xy -= offset;
        //Rotate
        float2 origPos = pos.xy;
        pos.x = (origPos.x * rotCosSin.x) - (origPos.y * rotCosSin.y);
        pos.y = (origPos.y * rotCosSin.x) + (origPos.x * rotCosSin.y);
        //Retransform to initial position
        pos.xy += offset;
    }
    else
    {
        //No rotation calculations (no need to do the rotation calculations if there is no rotation applied > redundant operations)
        //Just apply the pivot offset
        pos.xy -= pivotOffset;
    }

    //Geometry Vertex Output
    GS_DATA geomData = (GS_DATA)0;
    geomData.Position = mul(float4(pos, 1.0f), gTransform);
    geomData.Color = col;
    geomData.TexCoord = texCoord;
    triStream.Append(geomData);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
    //Given Data (Vertex Data)
    VS_DATA v = vertex[0];
    float3 position = v.TransformData.xyz; //Extract the position data from the VS_DATA vertex struct
    float2 offset = v.TransformData.xy; //Extract the offset data from the VS_DATA vertex struct (initial X and Y position)
    float rotation = v.TransformData.w; //Extract the rotation data from the VS_DATA vertex struct
    float2 pivot = v.TransformData2.xy; //Extract the pivot data from the VS_DATA vertex struct
    float2 scale = v.TransformData2.zw; //Extract the scale data from the VS_DATA vertex struct
    float2 texCoord = float2(0, 0); //Initial Texture Coordinate

    // LT----------RT //TringleStrip (LT > RT > LB, LB > RB > RT)
    // |          / |
    // |       /    |
    // |    /       |
    // | /          |
    // LB----------RB
     
    // rotation
    float2 rotCosSin;
    if (rotation == 0) rotCosSin = float2(0, 0);
    else rotCosSin = float2(cos(rotation), sin(rotation));

    //VERTEX 1 [LT]
    float2 pivotOffset = float2(pivot.x * gTextureSize.x * scale.x, pivot.y * gTextureSize.y * scale.y);
    CreateVertex(triStream, position, v.Color, texCoord, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!

    //VERTEX 2 [RT]
    texCoord = float2(1, 0);
    position.x = offset.x + gTextureSize.x * scale.x;
    CreateVertex(triStream, position, v.Color, texCoord, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!

    //VERTEX 3 [LB]
    texCoord = float2(0, 1);
    position.x = offset.x;
    position.y = offset.y + gTextureSize.y * scale.y;
    CreateVertex(triStream, position, v.Color, texCoord, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!

    //VERTEX 4 [RB]
    texCoord = float2(1, 1);
    position.x = offset.x + gTextureSize.x * scale.x;
    CreateVertex(triStream, position, v.Color, texCoord, rotation, rotCosSin, offset, pivotOffset); //Change the color data too!
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET
{
    float4 sampled = gSpriteTexture.Sample(samPoint, input.TexCoord);
    if (sampled.a < 1.f) discard;
   
    return sampled * input.Color;
}

// Default Technique
technique11 Default
{
    pass p0
    {
        SetRasterizerState(BackCulling);
        SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
        //SetDepthStencilState(NoDepth,0);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0, MainGS()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}