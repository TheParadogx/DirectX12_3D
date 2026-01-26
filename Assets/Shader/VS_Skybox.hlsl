// 定数バッファ（RootSignatureのb0）
cbuffer SceneBuffer : register(b0)
{
    matrix World;
    matrix View;
    matrix Projection;
};

struct VS_INPUT
{
    float3 Position : POSITION;
};

struct VS_OUTPUT
{
    float4 SV_Position : SV_POSITION;
    float3 TexCoord    : TEXCOORD; // キューブマップサンプリング用のベクトル
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output;

    // 1. 頂点の位置を計算
    // スカイボックスは常にカメラ（View行列の移動成分）に追従させる
    float4 worldPos = mul(float4(input.Position, 1.0f), World);
    float4 viewPos  = mul(worldPos, View);
    output.SV_Position = mul(viewPos, Projection);

    // 2. 【重要】深度値を 1.0 (一番奥) に固定するトリック
    // 出力座標の z を w と同じにすることで、除算後に z/w = 1.0 になる
    output.SV_Position.z = output.SV_Position.w;

    // 3. ローカル座標をそのままサンプリング用ベクトルとして渡す
    // 中心(0,0,0)から立方体の各頂点への方向が、そのまま空の色を取り出す方向になる
    output.TexCoord = input.Position;

    return output;
}