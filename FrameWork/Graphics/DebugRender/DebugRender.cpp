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
