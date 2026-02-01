#pragma once

namespace Engine::System
{
	struct InteractableComponent
	{
		float InteractRange = 10.0f;	//	インタラクトできる距離
		bool Talkable = true;			//	離せる状態かどうか
	};

}