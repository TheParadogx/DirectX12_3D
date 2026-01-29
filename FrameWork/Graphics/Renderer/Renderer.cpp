#include "pch.h"
#include "Renderer.hpp"

#include"Debug/spdlog/Logger.hpp"
#include"Graphics/DirectX/DirectX12.hpp"
#include"System/Camera/Camera.hpp"
#include"Math/Quaternion/Quaternion.h"

/// <summary>
/// インスタンス生成時の処理
/// </summary>
void Engine::Graphics::Renderer::OnCreate()
{
	bool ret = false;

	//	Vertex
	mVertexBuffer = std::make_unique<VertexBuffer>();
	//ret = mVertexBuffer->Create(
	//	PerMaxVertexBufferSize * 2,
	//	sizeof(LineVertex));
	ret = mVertexBuffer->Create(
		PerMaxVertexBufferSize * 2,
		1);
	if (ret == false)
	{
		LOG_CRITICAL("Failed Create RendererVertexBuffer.");
	}

	//	Index
	mIndexBuffer = std::make_unique<IndexBuffer>();
	ret = mIndexBuffer->Create(
		PerMaxIndexBufferSize * 2);
	if (ret == false)
	{
		LOG_CRITICAL("Failed Create RendererIndexBuffer.");
	}

	//	Constant
	mConstantBuffer = std::make_unique<ConstantBuffer>();
	ret = mConstantBuffer->Create(sizeof(CameraBuffer));
	if (ret == false)
	{
		LOG_CRITICAL("Failed Create RendererConstantBuffer.");
	}

	mVfxConstantBuffer = std::make_unique<ConstantBuffer>();
	ret = mVfxConstantBuffer->Create(sizeof(VfxConstantBuffer));
	if (ret == false)
	{
		LOG_CRITICAL("Failed Create VfxConstantBuffer.");
	}

	//	Pipeline
	ret = mSpritePipeline.Create();
	if (ret == false)
	{
		LOG_CRITICAL("Failed Create SpritePipeline.");
	}

	ret = mFbxPipeline.Create();
	if (ret == false)
	{
		LOG_CRITICAL("Failed Create FbxPipeline.");
	}

	ret = mLinePipeline.Create();
	if (ret == false)
	{
		LOG_CRITICAL("Failed Create LinePipeline.");
	}

	ret = mUISpritePipeline.Create();
	if (ret == false)
	{
		LOG_CRITICAL("Failed Create UISpritePipeline.");
	}

	ret = mSkyBoxPipeline.Create();
	if (ret == false)
	{
		LOG_CRITICAL("Failed Create SkyBoxPipeline.");
	}

	ret = mVfxPipeline.Create();
	if (ret == false)
	{
		LOG_CRITICAL("Failed Create VfxPipeline.");
	}



	//	CmdList
	mCmdList = Graphics::DirectX::GetInstance()->GetCommandList();

}

/// <summary>
/// インスタンスの解放時の処理
/// </summary>
void Engine::Graphics::Renderer::OnDestory()
{
	mVertexBuffer.reset();
	mIndexBuffer.reset();
	mConstantBuffer.reset();
}

/// <summary>
/// 新規描画フレームの開始
/// </summary>
void Engine::Graphics::Renderer::NewRenderingFrame()
{
	Graphics::DirectX* directX = Graphics::DirectX::GetInstance();
	const size_t frameIndex = static_cast<size_t>(directX->GetFrameIndex());

	mVertexBufferOffset = PerMaxVertexBufferSize * frameIndex;
	mIndexBufferOffset = PerMaxIndexBufferSize * frameIndex;

	mUsingVertexBufferSize = 0;
	mUsingIndexBufferSize = 0;

	mCurrentPipeline.RootSignature = nullptr;
	mCurrentPipeline.PipelineState = nullptr;
	mCurrentPipeline.Topology = D3D_PRIMITIVE_TOPOLOGY_UNDEFINED;

}

/// <summary>
/// 2D用スプライトの描画
/// </summary>
/// <param name="spriteVertices">頂点配列の先頭アドレス</param>
/// <param name="vertexCount">頂点配列数</param>
/// <param name="spriteIndices">インデックス配列の先頭アドレス</param>
/// <param name="indexCount">インデックス配列数</param>
void Engine::Graphics::Renderer::DrawSprite(const SpriteVertex* spriteVertices, const size_t vertexCount, const uint16_t* spriteIndices, const size_t indexCount)
{
	Draw(
		spriteVertices, sizeof(SpriteVertex), vertexCount,
		spriteIndices, eIndexBufferFormat::Uint16, indexCount);
}

/// <summary>
/// 基本描画関数
/// </summary>
void Engine::Graphics::Renderer::Draw(const void* VertexData, const size_t VertexStride, const size_t VertexCount, const void* IndexData, const eIndexBufferFormat IndexFormat, const size_t IndexCount)
{
	// --- 【修正】頂点バッファのアライメント調整 ---
		// 次のデータの開始地点が Stride の倍数になるように繰り上げる
	if (VertexStride > 0) {
		mVertexBufferOffset = (mVertexBufferOffset + (VertexStride - 1)) & ~(VertexStride - 1);
	}

	const size_t vertexSize = VertexStride * VertexCount;
	if (mUsingVertexBufferSize + vertexSize > PerMaxVertexBufferSize) {
		LOG_CRITICAL("Not Enough Renderer VertexBuffer.");
	}

	// データの更新とセット
	mVertexBuffer->Update(VertexData, mVertexBufferOffset, vertexSize);
	mVertexBuffer->Set(mVertexBufferOffset, VertexStride, vertexSize);

	// --- 【修正】インデックスのアライメント調整 ---
	size_t indexStride = (IndexFormat == eIndexBufferFormat::Uint32) ? 4 : 2;
	mIndexBufferOffset = (mIndexBufferOffset + (indexStride - 1)) & ~(indexStride - 1);

	const size_t indexSize = indexStride * IndexCount;
	if (mUsingIndexBufferSize + indexSize > PerMaxIndexBufferSize) {
		LOG_CRITICAL("Not Enough Renderer IndexBuffer.");
	}

	mIndexBuffer->Update(IndexData, mIndexBufferOffset, indexSize);
	mIndexBuffer->Set(IndexFormat, mIndexBufferOffset, indexSize);


	// 描画実行
	mCmdList->DrawIndexedInstanced(
		static_cast<UINT>(IndexCount),
		1,
		0, // すでに IndexBuffer->Set でオフセットを指定済みの場合は 0
		0, // すでに VertexBuffer->Set でオフセットを指定済みの場合は 0
		0
	);


	// 使用量の更新
	mUsingVertexBufferSize += vertexSize;
	mUsingIndexBufferSize += indexSize;

	// オフセットを更新
	mVertexBufferOffset += vertexSize;
	mIndexBufferOffset += indexSize;
}

//	RootSignature
void Engine::Graphics::Renderer::SetRootSignature(ID3D12RootSignature* RootSignature)
{

	//	別のやつなら変更する
	if (mCurrentPipeline.RootSignature != RootSignature)
	{
		mCmdList->SetGraphicsRootSignature(RootSignature);
		mCurrentPipeline.RootSignature = RootSignature;
	}
}
// PipelineState
void Engine::Graphics::Renderer::SetPipelineState(ID3D12PipelineState* PipelineState)
{

	if (mCurrentPipeline.PipelineState != PipelineState)
	{
		mCmdList->SetPipelineState(PipelineState);
		mCurrentPipeline.PipelineState = PipelineState;
	}
}
// Topology
void Engine::Graphics::Renderer::SetTopology(D3D_PRIMITIVE_TOPOLOGY Topology)
{
	if (mCurrentPipeline.Topology != Topology)
	{
		mCmdList->IASetPrimitiveTopology(Topology);
		mCurrentPipeline.Topology = Topology;
	}
}

/// <summary>
/// FbxMeshの描画
/// </summary>
/// <param name="VBv"></param>
/// <param name="IBv"></param>
/// <param name="IndexCount"></param>
/// <param name="SrvHandle"></param>
void Engine::Graphics::Renderer::DrawMesh(const D3D12_VERTEX_BUFFER_VIEW& VBv, const D3D12_INDEX_BUFFER_VIEW& IBv, UINT IndexCount, UINT StartIndex)
{
	mCmdList->IASetVertexBuffers(0, 1, &VBv);
	mCmdList->IASetIndexBuffer(&IBv);

	mCmdList->DrawIndexedInstanced(IndexCount, 1, StartIndex, 0, 0);
}

/// <summary>
/// SkyBoxの描画
/// </summary>
/// <param name="VBv">スカイボックスの頂点バッファビュー</param>
/// <param name="vertexCount">頂点数（通常36）</param>
void Engine::Graphics::Renderer::DrawSkyBox(const D3D12_VERTEX_BUFFER_VIEW& VBv, UINT vertexCount)
{
	// スカイボックスはインデックスバッファを使わない
	mCmdList->IASetVertexBuffers(0, 1, &VBv);

	// インデックスなしの描画命令
	mCmdList->DrawInstanced(vertexCount, 1, 0, 0);
}

/// <summary>
/// Vfxの描画
/// </summary>
/// <param name="v"></param>
/// <param name="vNum"></param>
/// <param name="i"></param>
/// <param name="iNum"></param>
void Engine::Graphics::Renderer::DrawVfx(const VfxVertex* v, size_t vNum, const uint16_t* i, size_t iNum)
{
	// 既存の Draw メソッドが VfxVertex のサイズも許容するように呼び出す
	Draw(v, sizeof(VfxVertex), vNum, i, eIndexBufferFormat::Uint16, iNum);
}

/// <summary>
/// ワールド空間へのライン描画
/// </summary>
/// <param name="lineVertices">頂点配列の先頭アドレス</param>
/// <param name="vertexCount">頂点配列数</param>
/// <param name="lineIndices">インデックス配列の先頭アドレス</param>
/// <param name="indexCount">インデックス配列の先頭アドレス</param>
void Engine::Graphics::Renderer::DrawLines(const LineVertex* LineVertices, const size_t VertexCount, const uint16_t* LineIndices, const size_t IndexCount)
{
	this->SetLinePipeline();

	//	カメラ用バッファの更新
	const CameraBuffer cameraBuffer =
	{
		Math::Matrix::Transpose(System::Camera::Main->GetView()),
		Math::Matrix::Transpose(System::Camera::Main->GetProjection())
	};
	mConstantBuffer->Update(&cameraBuffer);
	mConstantBuffer->Set(0);

	Draw(
		LineVertices, sizeof(LineVertex), VertexCount,
		LineIndices, eIndexBufferFormat::Uint16, IndexCount);
}

/// <summary>
/// デバック用の点と回転の表示
/// </summary>
/// <param name="pos"></param>
/// <param name="rot"></param>
/// <param name="length"></param>
void Engine::Graphics::Renderer::DrawGizmo(Math::Vector3 pos, Math::Quaternion rot, float length)
{
	auto RotateVec = [](const Math::Vector3& v, const Math::Quaternion& q) {
		Math::Vector3 qV(q.x, q.y, q.z);
		Math::Vector3 t = Math::Vector3::Cross(qV, v) * 2.0f;
		return v + (t * q.w) + Math::Vector3::Cross(qV, t);
		};

	Math::Vector3 dirX = RotateVec({ length, 0, 0 }, rot);
	Math::Vector3 dirY = RotateVec({ 0, length, 0 }, rot);
	Math::Vector3 dirZ = RotateVec({ 0, 0, length }, rot);

	Graphics::LineVertex vertices[6];

	// X軸 (赤)
	vertices[0].Position = pos;
	vertices[0].color = Graphics::Color::Red();
	vertices[1].Position = pos + dirX;
	vertices[1].color = Graphics::Color::Red();

	// Y軸 (緑)
	vertices[2].Position = pos;
	vertices[2].color = Graphics::Color::Green();
	vertices[3].Position = pos + dirY;
	vertices[3].color = Graphics::Color::Green();

	// Z軸 (青)
	vertices[4].Position = pos;
	vertices[4].color = Graphics::Color::Blue();
	vertices[5].Position = pos + dirZ;
	vertices[5].color = Graphics::Color::Blue();

	const uint16_t indices[6] = { 0, 1, 2, 3, 4, 5 };

	// すでに実績のある DrawLines を呼ぶ
	this->DrawLines(vertices, 6, indices, 6);
}

/// <summary>
/// スプライトのパイプラインのセット
/// </summary>
void Engine::Graphics::Renderer::SetSpritePipeline()
{
	this->SetRootSignature(mSpritePipeline.GetRootSignature());
	this->SetPipelineState(mSpritePipeline.GetPipelineState());
	this->SetTopology(mSpritePipeline.GetTopology());
}

/// <summary>
/// Fbxのパイプラインのセット
/// </summary>
void Engine::Graphics::Renderer::SetFbxPipeline()
{
	this->SetRootSignature(mFbxPipeline.GetRootSignature());
	this->SetPipelineState(mFbxPipeline.GetPipelineState());
	this->SetTopology(mFbxPipeline.GetTopology());
}

/// <summary>
/// 線描画用のパイプラインのセット
/// </summary>
void Engine::Graphics::Renderer::SetLinePipeline()
{
	this->SetRootSignature(mLinePipeline.GetRootSignature());
	this->SetPipelineState(mLinePipeline.GetPipelineState());
	this->SetTopology(mLinePipeline.GetTopology());

}

/// <summary>
/// UI用のspriteパイプラインのセット
/// </summary>
void Engine::Graphics::Renderer::SetUISpritePipeline()
{
	this->SetRootSignature(mUISpritePipeline.GetRootSignature());
	this->SetPipelineState(mUISpritePipeline.GetPipelineState());
	this->SetTopology(mUISpritePipeline.GetTopology());
}

/// <summary>
/// skybox pipeline
/// </summary>
void Engine::Graphics::Renderer::SetSkyBoxPipeline()
{
	this->SetRootSignature(mSkyBoxPipeline.GetRootSignature());
	this->SetPipelineState(mSkyBoxPipeline.GetPipelineState());
	this->SetTopology(mSkyBoxPipeline.GetTopology());
}

/// <summary>
/// VFX用のパイプライン
/// </summary>
void Engine::Graphics::Renderer::SetVfxPipeline()
{
	this->SetRootSignature(mVfxPipeline.GetRootSignature());
	this->SetPipelineState(mVfxPipeline.GetPipelineState());
	this->SetTopology(mVfxPipeline.GetTopology());
}

/// <summary>
/// VFX用の定数バッファをセットする
/// </summary>
void Engine::Graphics::Renderer::SetVfxConstantBuffer(uint32_t rootIndex, const void* data, size_t size)
{
	mVfxConstantBuffer->Update(data); // VFX専用バッファを更新
	mVfxConstantBuffer->Set(rootIndex); // こちらをバインド
}
