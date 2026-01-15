#pragma once

namespace Engine::Graphics 
{
	struct Color
	{
		Color() noexcept : r(1.0f), g(1.0f), b(1.0f), a(1.0f) {}
		Color(float r, float g, float b) noexcept : r(r), g(g), b(b), a(1.0f) {}
		Color(float r, float g, float b, float a) noexcept : r(r), g(g), b(b), a(a) {}
		Color(const Color& color) noexcept : r(color.r), g(color.g), b(color.b), a(color.a) {}

		//	赤
		static const Color Red();
		//	青
		static const Color Blue();
		//	緑
		static const Color Green();
		//	黒
		static const Color Black();
		//	白
		static const Color White();
		//	シアン
		static const Color Cyan();
		//	マゼンタ
		static const Color Magenta();
		//	黄
		static const Color Yellow();
		//	灰
		static const Color Gray();



		//	GPU や SIMD 用データなどで使用するのでunion
		union
		{
			struct
			{
				float r, g, b, a;
			};
			float value[4];
		};

		inline float& operator[](int index)
		{
			return value[index];
		}
		inline float operator[](int index)const
		{
			return value[index];
		}

	};
}