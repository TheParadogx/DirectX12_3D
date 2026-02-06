#include"Color.hpp"

using namespace Engine::Graphics;







//	赤
const Color Engine::Graphics::Color::Red()
{
	static const Color red = { 1.0f, 0.0f, 0.0f, 1.0f };
	return red;
}
//	青
const Color Engine::Graphics::Color::Blue()
{
	static const Color blue = { 0.0f, 0.0f, 1.0f, 1.0f };
	return blue;
}
//	緑
const Color Engine::Graphics::Color::Green()
{
	static const Color green = { 0.0f, 1.0f, 0.0f, 1.0f };
	return green;
}
//	黒
const Color Engine::Graphics::Color::Black()
{
	static const Color black = { 0.0f, 0.0f, 0.0f, 1.0f };
	return black;
}
//	白
const Color Engine::Graphics::Color::White()
{
	//	白
	static const Color white = {1.0f, 1.0f, 1.0f, 1.0f};
	return white;
}
//	シアン(水色)
const Color Engine::Graphics::Color::Cyan()
{
	//	シアン
	static const Color cyan = { 0.0f, 1.0f, 1.0f, 1.0f };
	return cyan;
}
//	マゼンタ(ピンク)
const Color Engine::Graphics::Color::Magenta()
{
	//	マゼンタ
	static const Color magenta = { 1.0f, 0.0f, 1.0f, 1.0f };
	return magenta;
}
//	黄色
const Color Engine::Graphics::Color::Yellow()
{
	//	黄
	static const Color yellow = { 1.0f, 0.92f, 0.016f, 1.0f };
	return yellow;
}
//	灰色
const Color Engine::Graphics::Color::Gray()
{
	//	灰
	static const Color gray = { 0.5f, 0.5f, 0.5f, 1.0f };
	return gray;
}

const Color Engine::Graphics::Color::CoralRed()
{
	static const Color CoralRed = { 0.95, 0.42, 0.45, 1.0f };
	return CoralRed;
}

const Color Engine::Graphics::Color::Apricot()
{
	static const Color Apricot = { 1.00, 0.8, 0.5, 1.0f };
	return Apricot;
}

const Color Engine::Graphics::Color::Lime()
{
	static const Color Lime = { 0.45, 0.7, 0.10, 1.0f };
	return Lime;
}

