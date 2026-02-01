#include "Header.hlsli"

// PS_Texture.hlsl
cbuffer SpriteData : register(b0)
{
    float4x4 WVP;
    float4 MaterialColor; // ここにアルファが入る
};

Texture2D MainTexture : register(t0);
SamplerState Sampler : register(s0);

float4 main(VStoPS input) : SV_Target0
{
    float4 texColor = MainTexture.Sample(Sampler, input.UV);
    
    // テクスチャの色に、CPUから送った色（透明度含む）を掛ける
    float4 finalColor = texColor * MaterialColor;

    if (finalColor.a <= 0)
    {
        discard;
    }
    return finalColor;
}