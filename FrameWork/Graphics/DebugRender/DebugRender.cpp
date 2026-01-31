#include "pch.h"
#include "DebugRender.hpp"
#include "Graphics/Renderer/Renderer.hpp"
#include "Graphics/Graphics.hpp"

void Engine::Graphics::DebugRender::DrawDebugCircle(const Math::Vector3& Center, float Radius, const Graphics::Color& Color, const int Segments)
{
    std::vector<LineVertex> vertices(Segments);
    std::vector<uint16_t> indices;

    for (int i = 0; i < Segments; ++i) {
        float theta = (2.0f * 3.14159265f * i) / Segments;
        vertices[i].Position = {
            Center.x + Radius * cosf(theta),
            Center.y, 
            Center.z + Radius * sinf(theta)
        };
        vertices[i].color = Color;

        // インデックスを張る (i番目とi+1番目をつなぐ)
        indices.push_back((uint16_t)i);
        indices.push_back((uint16_t)((i + 1) % Segments));
    }

    //  描画
    Renderer::GetInstance()->DrawLines(
        vertices.data(), vertices.size(),
        indices.data(), indices.size()
    );
}

void Engine::Graphics::DebugRender::DrawDebugQuad(const Math::Matrix& World, float Width, float Height, const Graphics::Color& Color)
{
    // ローカル座標での4隅 (ピボット中央想定、必要に応じて修正してください)
    float halfW = Width * 0.5f;
    float halfH = Height * 0.5f;

    Math::Vector3 localPos[4] = {
        { -halfW, -halfH, 0.0f }, // 左下
        { -halfW,  halfH, 0.0f }, // 左上
        {  halfW,  halfH, 0.0f }, // 右上
        {  halfW, -halfH, 0.0f }  // 右下
    };

    std::vector<LineVertex> vertices(4);
    for (int i = 0; i < 4; ++i) {
        // 行列を使ってワールド座標へ変換
        // Vector3::TransformCoord (あるいは相当する行列乗算) を使用
        vertices[i].Position = Math::Vector3::TransformCoord(localPos[i], World);
        vertices[i].color = Color;
    }

    // 4つの点をつなぐインデックス (0-1, 1-2, 2-3, 3-0)
    uint16_t indices[] = { 0, 1, 1, 2, 2, 3, 3, 0 };

    Renderer::GetInstance()->DrawLines(
        vertices.data(), (UINT)vertices.size(),
        indices, _countof(indices)
    );
}
