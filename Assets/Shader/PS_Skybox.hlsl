TextureCube g_Skybox : register(t0);
SamplerState g_Sampler : register(s0);

struct VS_OUTPUT
{
    float4 Position : SV_POSITION;
    float3 TexCoord : TEXCOORD;
};

float4 main(VS_OUTPUT input) : SV_TARGET
{
    // float3の方向ベクトルを使ってCubeMapをサンプリング
    return g_Skybox.Sample(g_Sampler, input.TexCoord);
}