#include "Header3D.hlsli"

//	メインテクスチャ
Texture2D MainTexture : register(t0);
SamplerState Sampler : register(s0);

float ToonShadow(float DotValue)
{
    if (DotValue >= 0.5f)
    {
        return 1.0f;
    }
    else
    {
        return 0.5f;
    }
}

// 今回は光源位置を固定としてライティングの作成（制作展終了後リファクタリング）
float4 main(VStoPS input) : SV_Target0
{
    //  色
    float4 texColor = MainTexture.SampleLevel(Sampler, input.UV, 0);
    float4 baseColor = texColor * input.Color;

    // らいと
    float3 LightDir = normalize(float3(1.0f, 1.0f, 1.0f));
    
    float3 N = normalize(input.Normal);
    float d = dot(N, LightDir) * 0.5f + 0.5f;

    //  境界のアンチエリアス処理
    float delta = fwidth(d);
    //  少しアニメ調のほうがよさそう
    delta *= 0.01f;
    float shadow = smoothstep(0.5f - delta, 0.5f + delta, d) * 0.7f + 0.3f;
    
    //  環境光も今回は固定（空の明るさ考慮）
    float3 ambient = float3(0.05f, 0.05f, 0.1f) + (N.y * 0.05f);

    // 最終色の合成
    float3 finalRGB = baseColor.rgb * (shadow + ambient);
    
    return float4(finalRGB, baseColor.a);
}
