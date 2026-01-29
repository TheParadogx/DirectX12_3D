#include "VfxShader.hlsli"

PS_INPUT main(VS_INPUT input)
{
    PS_INPUT output = (PS_INPUT) 0;

    float4 localPos = float4(input.Pos, 1.0f);

    // FBXのシェーダーと「完全に」同じ順序にする
    float4 worldPos = mul(localPos, World);
    float4 viewPos = mul(worldPos, View);
    output.Pos = mul(viewPos, Projection);

    output.UV = input.UV;
    output.Color = Color;

    return output;
}

