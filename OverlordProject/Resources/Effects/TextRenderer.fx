float4x4 gTransform : WORLDVIEWPROJECTION;
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

RasterizerState BackCulling
{
	CullMode = BACK;
};

//SHADER STRUCTS
//**************
struct VS_DATA
{
	int Channel : TEXCOORD2; //Texture Channel
	float3 Position : POSITION; //Left-Top Character Quad Starting Position
	float4 Color: COLOR; //Color of the vertex
	float2 TexCoord: TEXCOORD0; //Left-Top Character Texture Coordinate on Texture
	float2 CharSize: TEXCOORD1; //Size of the character (in screenspace)
};

struct GS_DATA
{
	float4 Position : SV_POSITION; //HOMOGENEOUS clipping space position
	float4 Color: COLOR; //Color of the vertex
	float2 TexCoord: TEXCOORD0; //Texcoord of the vertex
	int Channel : TEXCOORD1; //Channel of the vertex
};

//VERTEX SHADER
//*************
VS_DATA MainVS(VS_DATA input)
{
	return input;
}

//GEOMETRY SHADER
//***************
void CreateVertex(inout TriangleStream<GS_DATA> triStream, float3 pos, float4 col, float2 texCoord, int channel)
{
	//Create a new GS_DATA object
	GS_DATA data = (GS_DATA)0;
	//Fill in all the fields
	data.Position = mul(float4(pos, 1.f), gTransform);
	data.Color = col;
	data.TexCoord = texCoord;
	data.Channel = channel;
	//Append it to the TriangleStream
	triStream.Append(data);
}

[maxvertexcount(4)]
void MainGS(point VS_DATA vertex[1], inout TriangleStream<GS_DATA> triStream)
{
	//REMOVE THIS >
	//GS_DATA dummyData = (GS_DATA)0; //Just some dummy data
	//triStream.Append(dummyData); //The geometry shader needs to emit something, see what happens if it doesn't emit anything.
	//< STOP REMOVING

	//Create a Quad using the character information of the given vertex
	//Note that the Vertex.CharSize is in screenspace, TextureCoordinates aren't ;) [Range 0 > 1]
	VS_DATA v = vertex[0];
	float3 pos = v.Position;
	float2 size = v.CharSize / gTextureSize;
	//1. Vertex Left-Top
	CreateVertex(triStream, float3(pos.x, pos.y, pos.z), v.Color, v.TexCoord, v.Channel);

	//2. Vertex Right-Top
	CreateVertex(triStream, float3(pos.x + v.CharSize.x, pos.y, pos.z), v.Color, float2(v.TexCoord.x + size.x, v.TexCoord.y), v.Channel);

	//3. Vertex Left-Bottom
	CreateVertex(triStream, float3(pos.x, pos.y + v.CharSize.y, pos.z), v.Color, float2(v.TexCoord.x, v.TexCoord.y + size.y), v.Channel);

	//4. Vertex Right-Bottom
	CreateVertex(triStream, float3(pos.x + v.CharSize.x, pos.y + v.CharSize.y, pos.z), v.Color, float2(v.TexCoord.x + size.x, v.TexCoord.y + size.y), v.Channel);
}

//PIXEL SHADER
//************
float4 MainPS(GS_DATA input) : SV_TARGET
{

	//Sample the texture and return the correct channel [Vertex.Channel]
	float channel = gSpriteTexture.Sample(samPoint, input.TexCoord)[input.Channel];
	//You can iterate a float4 just like an array, using the index operator
	//Also, don't forget to colorize ;) [Vertex.Color]

	return (input.Color * channel);
}

// Default Technique
technique10 Default 
{

	pass p0 
	{
		SetRasterizerState(BackCulling);
		SetBlendState(EnableBlending, float4(0.0f, 0.0f, 0.0f, 0.0f), 0xFFFFFFFF);
		SetVertexShader(CompileShader(vs_4_0, MainVS()));
		SetGeometryShader(CompileShader(gs_4_0, MainGS()));
		SetPixelShader(CompileShader(ps_4_0, MainPS()));
	}
}
