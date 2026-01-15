#include"pch.h"


#if _DEBUG
#define _CRTDBG_MAP_ALLOC 
#include <crtdbg.h>
#endif // _DEBUG

#include "Application/Application.hpp"

int APIENTRY WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd)
{
	Engine::App::Application::Create();
	Engine::App::Application* app = Engine::App::Application::GetInstance();
	app->Initialize();
	app->Run();
	app->Shutdown();
	return 0;
}