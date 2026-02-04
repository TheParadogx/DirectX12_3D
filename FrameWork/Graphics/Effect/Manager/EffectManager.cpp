#include "pch.h"
#include "EffectManager.hpp"
#include"Graphics/DirectX/DirectX12.hpp"
#include"System/Camera/Camera.hpp"

/// <summary>
/// 初期化
/// </summary>
/// <returns>true:成功</returns>
bool Engine::Graphics::EffectManager::Initialize()
{
    DirectX* directX = DirectX::GetInstance();
    if (directX == nullptr) return false;

    //  レンダラー作成
    DXGI_FORMAT rtFormat = directX->GetFormat();

    mEfkRenderer = EffekseerRendererDX12::Create(
        directX->GetDevice(),
        directX->GetCommandQueue(),
        directX->NUM_BACK_BAFFER,
        &rtFormat,
        1,
        DXGI_FORMAT_D32_FLOAT,
        false,
        8000
    );

    if (mEfkRenderer == nullptr)
    {
        LOG_ERROR("Failed CreateEfkRenderer.");
        return false;
    }

    mEfkManager = Effekseer::Manager::Create(2000);
    
    // 各種レンダラーとローダーの設定
    mEfkManager->SetSpriteRenderer(mEfkRenderer->CreateSpriteRenderer());
    mEfkManager->SetRibbonRenderer(mEfkRenderer->CreateRibbonRenderer());
    mEfkManager->SetRingRenderer(mEfkRenderer->CreateRingRenderer());
    mEfkManager->SetModelRenderer(mEfkRenderer->CreateModelRenderer());
    mEfkManager->SetTrackRenderer(mEfkRenderer->CreateTrackRenderer());

    mEfkManager->SetTextureLoader(mEfkRenderer->CreateTextureLoader());
    mEfkManager->SetModelLoader(mEfkRenderer->CreateModelLoader());
    mEfkManager->SetMaterialLoader(mEfkRenderer->CreateMaterialLoader());

    mEfkMemoryPool = EffekseerRenderer::CreateSingleFrameMemoryPool(
        mEfkRenderer->GetGraphicsDevice());

    if (mEfkMemoryPool == nullptr)
    {
        LOG_ERROR("Failed CreateSingleFrameMemoryPool.");
        return false;
    }

    //  専用のコマンドリストの作成
    mEfkCmdList = EffekseerRenderer::CreateCommandList(mEfkRenderer->GetGraphicsDevice(), mEfkMemoryPool);

    return true;
}

/// <summary>
/// エフェクトの取得
/// </summary>
/// <param name="FilePath">フォルダパス</param>
/// <returns></returns>
Effekseer::EffectRef Engine::Graphics::EffectManager::GetEffect(const std::filesystem::path& FilePath)
{
    //  パスの正規化
    std::u16string key = std::filesystem::absolute(FilePath).lexically_normal().u16string();

    auto it = mEffectCache.find(key);
    if (it != mEffectCache.end())
    {
        return it->second;
    }

    // なければロードして保存
    auto effect = Effekseer::Effect::Create(
        mEfkManager,
        key.c_str()
    );

    if (effect != nullptr)
    {
        mEffectCache.emplace(key, effect);
    }

    return effect;
}

/// <summary>
/// 更新
/// </summary>
void Engine::Graphics::EffectManager::Update()
{
    if (mEfkManager != nullptr)
    {
        mEfkManager->Update();
    }
}

/// <summary>
/// 描画
/// </summary>
void Engine::Graphics::EffectManager::Draw()
{

    auto cmdList = DirectX::GetInstance()->GetCommandList();
    if (mEfkRenderer == nullptr || mEfkManager == nullptr || cmdList == nullptr) return;

    const auto& view = Engine::System::Camera::Main->GetView();
    const auto& proj = Engine::System::Camera::Main->GetProjection();

    //  変換
    mEfkRenderer->SetCameraMatrix(ToEfkMatrix(view));
    mEfkRenderer->SetProjectionMatrix(ToEfkMatrix(proj));

    EffekseerRendererDX12::BeginCommandList(mEfkCmdList, cmdList);
    mEfkRenderer->SetCommandList(mEfkCmdList);

    mEfkRenderer->BeginRendering();
    mEfkManager->Draw();
    mEfkRenderer->EndRendering();

    EffekseerRendererDX12::EndCommandList(mEfkCmdList);
}

Effekseer::Matrix44 Engine::Graphics::EffectManager::ToEfkMatrix(const Math::Matrix& CustomMat)
{
    Effekseer::Matrix44 result;

    // Mat[4][4] 配列に直接アクセスできる場合はこれが最速です
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.Values[i][j] = CustomMat.Mat[i][j];
        }
    }

    return result;
}
