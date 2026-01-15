#include "Header3D.hlsli"

VStoPS main(VertexInput input)
{
    VStoPS output = (VStoPS) 0;
    
    float4 localPos = float4(input.Position, 1.0f);
    float4 localNormal = float4(input.Normal, 0.0f);

    // フラグを見て「ボーン計算をするか」決める
    if (IsSkinned != 0)
    {
        // プレイヤーなどのスキニングありモデルの処理
        float4x4 skinMatrix =
            Bones[input.BoneIndex.x] * (float) input.Weight.x +
            Bones[input.BoneIndex.y] * (float) input.Weight.y +
            Bones[input.BoneIndex.z] * (float) input.Weight.z +
            Bones[input.BoneIndex.w] * (float) input.Weight.w;

        localPos = mul(localPos, skinMatrix);
        localNormal = mul(localNormal, skinMatrix);
    }
    // IsSkinned が 0 (Cubeなど) の場合は、localPos は元の座標のまま進む
    
    // 頂点座標の変換
    float4 worldPos = mul(localPos, Transform); // ここで Transform (World行列) がかかる
    float4 viewPos = mul(worldPos, View);
    output.Position = mul(viewPos, Projection);
    
    // 法線の変換
    float4 worldNormal = mul(localNormal, Transform);
    output.Normal = normalize(worldNormal.xyz);
    
    output.UV = input.UV;
    output.Color = Color;
    return output;

}