#include "pch.h"
#include "EffectManager.hpp"
#include"Graphics/DirectX/DirectX12.hpp"
#include"System/Camera/Camera.hpp"

#include"System/Window/Window.hpp"

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
void Engine::Graphics::EffectManager::Update(float DeltaTime)
{
    if (mEfkManager != nullptr)
    {
        mEfkManager->Update(DeltaTime * 60.0f);
    }

    if (mEfkMemoryPool != nullptr)
    {
        mEfkMemoryPool->NewFrame();
    }
}

/// <summary>
/// 描画
/// </summary>
void Engine::Graphics::EffectManager::Draw()
{

    auto cmdList = DirectX::GetInstance()->GetCommandList();
    auto mainCamera = System::Camera::Main;
    if (mEfkRenderer == nullptr || mEfkManager == nullptr || cmdList == nullptr) return;
    //  カメラある時
    if (mainCamera != nullptr)
    {
        const auto& view = mainCamera->GetView();
        const auto& proj = mainCamera->GetProjection();

        //  変換
        mEfkRenderer->SetCameraMatrix(ToEfkMatrix(view));
        mEfkRenderer->SetProjectionMatrix(ToEfkMatrix(proj));

    }
    //  カメラないとき
    else
    {
        // カメラがない場合：2D用プロジェクションを設定
        Effekseer::Matrix44 viewMat;
        Effekseer::Matrix44 projMat;
        viewMat.Indentity();

        // 画面サイズを取得
        float screenW = static_cast<float>(System::Window::GetInstance()->GetWidth());
        float screenH = static_cast<float>(System::Window::GetInstance()->GetHeight());

        // 平行投影行列を作成（左上(0,0), 右下(W, H)）
        projMat.OrthographicLH(screenW, screenH, 0.0f, 100.0f);

        mEfkRenderer->SetCameraMatrix(viewMat);
        mEfkRenderer->SetProjectionMatrix(projMat);
    }


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

/// <summary>
/// マネージャーの取得
/// </summary>
/// <returns></returns>
Effekseer::ManagerRef Engine::Graphics::EffectManager::GetManager()
{
    return mEfkManager;
}
