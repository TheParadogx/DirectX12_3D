#include "Header.hlsli"

//	メインテクスチャ
Texture2D MainTexture : register(t0);
SamplerState Sampler : register(s0);

float4 main(VStoPS input) : SV_Target0
{
    float4 Color = MainTexture.Sample(Sampler, input.UV);
    if (Color.a <= 0)
    {
        discard;
    }
    return Color;
}