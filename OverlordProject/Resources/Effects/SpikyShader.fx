//************
// VARIABLES *
//************
float4x4 m_MatrixWorldViewProj : WORLDVIEWPROJECTION;
float4x4 m_MatrixWorld : WORLD;
float3 m_LightDirection : DIRECTION
<
    string Object = "TargetLight";
> = float3(-0.577f, 0.577f, -0.577f);

float3 gColorDiffuse : COLOR = float3(1, 1, 1);

float gSpikeLength
<
    string UIWidget = "Slider";
    float UIMin = 0.0f;
    float UIMax = 0.5f;
    float UIStep = 0.0001f;
> = 0.2f;

RasterizerState FrontCulling
{
    CullMode = FRONT;
};

//**********
// STRUCTS *
//**********
struct VS_DATA
{
    float3 Position : POSITION;
    float3 Normal : NORMAL;
};

struct GS_DATA
{
    float4 Position : SV_POSITION;
    float3 Normal : NORMAL;
};

//****************
// VERTEX SHADER *
//****************
VS_DATA MainVS(VS_DATA vsData)
{
    //Step 1.
    //Delete this transformation code and just return the VS_DATA parameter (vsData)
    //Don't forget to change the return type!

    //temp.Position = mul(float4(vsData.Position, 1), m_MatrixWorldViewProj);
    //temp.Normal = mul(vsData.Normal, (float3x3) m_MatrixWorld);

    return vsData;
}

//******************
// GEOMETRY SHADER *
//******************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float3 normal)
{
    //Step 1. Create a GS_DATA object
    GS_DATA data = (GS_DATA)0;
    //Step 2. Transform the position using the WVP Matrix and assign it to (GS_DATA object).Position (Keep in mind: float3 -> float4)
    data.Position = mul(float4(pos, 1), m_MatrixWorldViewProj);
    //Step 3. Transform the normal using the World Matrix and assign it to (GS_DATA object).Normal (Only Rotation, No translation!)
    data.Normal = mul(normal, (float3x3)m_MatrixWorld);
    //Step 4. Append (GS_DATA object) to the TriangleStream parameter (TriangleStream::Append(...))
    triStream.Append(data);
}

[maxvertexcount(9)]
void SpikeGenerator(triangle VS_DATA vertices[3], inout TriangleStream<GS_DATA> triStream)
{
    //Use these variable names
    float3 basePoint, top, left, right, spikeNormal;

    top = vertices[0].Position;
    left = vertices[1].Position;
    right = vertices[2].Position;

    //Step 1. Calculate CENTER_POINT
    basePoint = (top + left + right) / 3;
    //Step 2. Calculate Face Normal (Original Triangle)
    spikeNormal = (vertices[0].Normal + vertices[1].Normal + vertices[2].Normal) / 3;
    //Step 3. Offset CENTER_POINT (use gSpikeLength)
    basePoint = basePoint + (gSpikeLength * spikeNormal);
    //Step 4 + 5. Calculate Individual Face Normals (Cross Product of Face Edges) & Create Vertices for every face
    //FACE 1 (top, right, basePoint)
    float3 p0 = right - top;
    float3 p1 = right - basePoint;
    float3 faceNormal = normalize(cross(p0, p1));
    CreateVertex(triStream, top, faceNormal);
    CreateVertex(triStream, basePoint, faceNormal);
    CreateVertex(triStream, right, faceNormal);
    //Restart Strip! >> We want to start a new triangle (= interrupt trianglestrip)
    triStream.RestartStrip();

    //FACE 2 (right, left, basePoint)
    p0 = left - right;
    p1 = left - basePoint;
    faceNormal = normalize(cross(p0, p1));
    CreateVertex(triStream, right, faceNormal);
    CreateVertex(triStream, basePoint, faceNormal);
    CreateVertex(triStream, left, faceNormal);

    triStream.RestartStrip();

    //Face 3 (left, top, basePoint)
    p0 = top - left;
    p1 = top - basePoint;
    faceNormal = normalize(cross(p0, p1));
    CreateVertex(triStream, left, faceNormal);
    CreateVertex(triStream, basePoint, faceNormal);
    CreateVertex(triStream, top, faceNormal);

    triStream.RestartStrip();

    //Step 6. Complete code in CreateVertex(...)
    //Step 7. Bind this Geometry Shader function to the effect pass (See Technique Struct)
}

//***************
// PIXEL SHADER *
//***************
float4 MainPS(GS_DATA input) : SV_TARGET
{
    input.Normal = normalize(input.Normal);
    float diffuseStrength = max(dot(normalize(m_LightDirection), -input.Normal), 0.2f);
    return float4(gColorDiffuse * diffuseStrength, 1.0f);
}

//*************
// TECHNIQUES *
//*************
technique11 Default //FXComposer >> Rename to "technique10 Default"
{
    pass p0
    {
        SetRasterizerState(FrontCulling);
        SetVertexShader(CompileShader(vs_4_0, MainVS()));
        SetGeometryShader(CompileShader(gs_4_0, SpikeGenerator()));
        SetPixelShader(CompileShader(ps_4_0, MainPS()));
    }
}