#pragma once

namespace Engine::System
{
	struct WindowSetting
	{
		int width = 1280;      //	ウィンドウの縦幅
		int height = 720;      //	ウィンドウの横幅
		bool isFullscreen = false;	//	true:フルスクリーン false:ウィンドウ
	};
}