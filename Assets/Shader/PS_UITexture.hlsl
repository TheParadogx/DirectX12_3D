#include "Header.hlsli"

// ルート定数（b1）から ratio を受け取る
cbuffer UIBarConfig : register(b1)
{
    float4 g_color; //  set1-4
    float g_ratio; // 0.0f ～ 1.0f set0
};

Texture2D MainTexture : register(t0);
SamplerState Sampler : register(s0);

float4 main(VStoPS input) : SV_Target0
{
    // UVのx座標が ratio を超えていたら描画をスキップ
    // これにより画像が「削れる」表現になる
    if (input.UV.x > g_ratio)
    {
        discard;
    }

    float4 Color = MainTexture.Sample(Sampler, input.UV);
    
    // アルファテスト（既存スプライトと同じ挙動）
    if (Color.a <= 0)
    {
        discard;
    }


    return Color * g_color;
}