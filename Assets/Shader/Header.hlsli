struct VertexInput
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD;
};

struct VStoPS
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
};