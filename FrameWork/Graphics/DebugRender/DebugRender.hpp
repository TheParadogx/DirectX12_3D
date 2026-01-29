#pragma once
#include"Math/Vector3/Vector3.h"
#include"Graphics/Color/Color.hpp"

namespace Engine::Graphics
{
	class DebugRender
	{
	public:		
		/// <summary>
		/// デバック用の円の描画
		/// </summary>
		/// <param name="Center">中心座標</param>
		/// <param name="Radius">半径</param>
		/// <param name="Color">色</param>
		/// <param name="Segments">頂点数</param>
		static void DrawDebugCircle(const Math::Vector3& Center, float Radius, const Graphics::Color& Color,const int Segments = 32);

		static void DrawDebugQuad(const Math::Matrix& World, float Width, float Height, const Graphics::Color& Color);
	};
}

