struct VertexInput
{
    float3 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
    int4 BoneIndex : BONE_INDEX;
    float4 Weight : WEIGHT;
};

struct VStoPS
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 Normal : NORMAL;
    float4 Color : COLOR;
};

//	モデル用
cbuffer ModelBuffer : register(b0)
{
    //  姿勢行列
    matrix Transform;
	//	ビュー行列
    matrix View;
	//	プロジェクション行列
    matrix Projection;
    //  色
    float4 Color;
    //  ボーン情報
    matrix Bones[256];
    int IsSkinned; // 0:スキニングをしない 1:スキニングをする
    float padding[3];
}