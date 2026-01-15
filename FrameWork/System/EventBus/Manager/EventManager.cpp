#include "pch.h"
#include "EventManager.hpp"

/// <summary>
/// 遅延型イベントの処理
/// フレーム末で処理をします。
/// </summary>
void Engine::System::EventManager::ProcessEvents()
{
	if (mEventQueue.empty())
	{
		return;
	}

	// 処理中に新しいイベントが追加されてもいいように一時バッファに移動
	std::vector<std::function<void()>> processingQueue;
	processingQueue.swap(mEventQueue);
	for (auto& eventTask : processingQueue) {
		eventTask();
	}

}
