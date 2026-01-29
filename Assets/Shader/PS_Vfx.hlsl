#include"VfxShader.hlsli"

Texture2D tex : register(t0);
SamplerState sam : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
    // テクスチャの色をサンプリング
    float4 texColor = tex.Sample(sam, input.UV);
    
    // テクスチャの色に、プログラムで設定した色（m_color）を乗算
    float4 finalColor = texColor * input.Color;
    
    // アルファ値が0に近い部分は描画をスキップ（テクスチャの抜き）
    clip(finalColor.a - 0.01f);
    
    return finalColor;
}