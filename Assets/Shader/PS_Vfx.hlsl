#include"VfxShader.hlsli"

Texture2D tex : register(t0);
SamplerState sam : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャの色をサンプリング
    float4 texColor = tex.Sample(sam, input.UV);
    
    // テクスチャの色に、プログラムで設定した色を乗算
    float4 finalColor = texColor * input.Color;
    
    finalColor.rgb *= finalColor.a;
    
    return finalColor;
}