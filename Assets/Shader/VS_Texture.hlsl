#include "Header.hlsli"

VStoPS main(VertexInput input)
{
    VStoPS output = (VStoPS)0;
    output.Position = float4(input.Position, 1.0f);
    output.UV = input.UV;
    return output;
}