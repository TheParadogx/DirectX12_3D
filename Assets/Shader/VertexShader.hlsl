struct VertexInput
{
    float3 Position : POSITION;
};

struct VertexOutput
{
    float4 Position : SV_Position;
};

VertexOutput main(VertexInput input)
{
    VertexOutput output;
    output.Position = float4(input.Position, 1.0f);
    return output;
}