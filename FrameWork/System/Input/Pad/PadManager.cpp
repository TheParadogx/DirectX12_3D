#include "pch.h"
#include "PadManager.hpp"

#include<winrt/Windows.Foundation.Collections.h>

/// <summary>
/// インデックスが範囲内か判定するメソッド
/// </summary>
/// <param name="index"></param>
/// <returns>true:範囲内</returns>
bool Engine::Input::PadManager::IsValidIndex(const uint8_t index) const
{
	//	サイズ以上の時
	if (static_cast<size_t>(index) >= mGamepads.size())
	{
		return false;
	}

	//	接続最大数以上の時
	else if (index >= static_cast<size_t>(ePadIndex::Count))
	{
		return false;
	}

	return true;
}

/// <summary>
/// 渡されたボタンが有効かどうかの判定
/// </summary>
/// <param name="Button"></param>
/// <returns></returns>
bool Engine::Input::PadManager::IsValidButtonCode(ePadButton Button)const
{
	if (Button == ePadButton::Count) return false;
	return true;
}

/// <summary>
/// 初期化
/// </summary>
Engine::Input::PadManager::PadManager()
{
	using namespace winrt::Windows;
	//	Pad取得
	Foundation::Collections::IVectorView<Gaming::Input::Gamepad> pads = Gaming::Input::Gamepad::Gamepads();
	uint32_t count = pads.Size();
	for (uint32_t i = 0; i < count; i++)
	{
		//	インスタンス生成
		mGamepads.emplace_back(pads.GetAt(i));
	}

	//	Pad追加イベントをバインド
	Gaming::Input::Gamepad::GamepadAdded([this](auto const&, Gaming::Input::Gamepad const& Pad)
	{
			Engine::Input::Pad entry(Pad);
			mGamepads.push_back(std::move(entry));
	});

	//	Pad削除イベントをバインド
	Gaming::Input::Gamepad::GamepadRemoved([this](auto const&, Gaming::Input::Gamepad const& Pad)
	{
			std::erase_if(mGamepads, [&](auto& e)
				{return e.GetGamepad() == Pad; });
	}); 
}

/// <summary>
/// 各Padの状態更新
/// </summary>
void Engine::Input::PadManager::Update()
{
	//	状態更新
	for (auto& gamepad : mGamepads)
	{
		gamepad.Update();
	}
}

/// <summary>
/// 押した瞬間
/// </summary>
/// <param name="button">ボタン種別</param>
/// <returns>true:押した瞬間</returns>
bool Engine::Input::PadManager::IsPressed(const ePadButton Button, const ePadIndex Index) const
{
	if (this->IsValidButtonCode(Button) == false) return false;

	const auto index = static_cast<uint8_t>(Index);
	if (this->IsValidIndex(index) == false)
	{
		return false;
	}
	return mGamepads[index].IsPressed(Button);
}

/// <summary>
/// 押している間
/// </summary>
/// <param name="button"></param>
/// <returns>true:押されている</returns>
bool Engine::Input::PadManager::IsHeld(const ePadButton Button, const ePadIndex Index) const
{
	if (this->IsValidButtonCode(Button) == false) return false;

	const auto index = static_cast<uint8_t>(Index);
	if (this->IsValidIndex(index) == false)
	{
		return false;
	}
	return mGamepads[index].IsHeld(Button);
}

/// <summary>
/// 離した瞬間
/// </summary>
/// <param name="button"></param>
/// <returns>true:離した瞬間</returns>
bool Engine::Input::PadManager::IsReleased(const ePadButton Button, const ePadIndex Index) const
{
	if (this->IsValidButtonCode(Button) == false) return false;

	const auto index = static_cast<uint8_t>(Index);
	if (this->IsValidIndex(index) == false)
	{
		return false;
	}
	return mGamepads[index].IsReleased(Button);
}


/// <summary>
/// 3D用の左スティック取得
/// </summary>
/// <returns></returns>
Math::Vector2 Engine::Input::PadManager::GetLeftStick3D(const ePadIndex Index) const
{
	const auto index = static_cast<uint8_t>(Index);
	if (this->IsValidIndex(index) == false)
	{
		return Math::Vector2();
	}
	return mGamepads[index].GetLeftStick3D();
}

/// <summary>
/// 3D用の右スティック取得
/// </summary>
/// <returns></returns>
Math::Vector2 Engine::Input::PadManager::GetRightStick3D(const ePadIndex Index) const
{
	const auto index = static_cast<uint8_t>(Index);
	if (this->IsValidIndex(index) == false)
	{
		return Math::Vector2();
	}
	return mGamepads[index].GetRightStick3D();
}

/// <summary>
/// 2D用の左スティック取得
/// </summary>
/// <returns></returns>
Math::Vector2 Engine::Input::PadManager::GetLeftStick2D(const ePadIndex Index) const
{
	const auto index = static_cast<uint8_t>(Index);
	if (this->IsValidIndex(index) == false)
	{
		return Math::Vector2();
	}
	return mGamepads[index].GetLeftStick2D();
}

/// <summary>
/// 右スティック取得
/// </summary>
/// <returns></returns>
Math::Vector2 Engine::Input::PadManager::GetRightStick2D(const ePadIndex Index) const
{
	const auto index = static_cast<uint8_t>(Index);
	if (this->IsValidIndex(index) == false)
	{
		return Math::Vector2();
	}
	return mGamepads[index].GetRightStick2D();
}

/// <summary>
/// 仕様可能なPadのIndexの一覧を返す
/// </summary>
/// <returns></returns>
std::vector<Engine::Input::ePadIndex> Engine::Input::PadManager::GetAvailableIndices() const
{
	std::vector<ePadIndex> result;
	const size_t count = std::min(mGamepads.size(), static_cast<size_t>(ePadIndex::Count));
	for (int i = 0; i < count; i++)
	{
		result.push_back(static_cast<ePadIndex>(i));
	}
	return result;
}

/// <summary>
/// ImGuiの更新
/// </summary>
void Engine::Input::PadManager::ImGuiUpdate()
{
	ImGui::Begin("Gamepad Monitor");
	ImGui::Text("Connected Pads: %zu", mGamepads.size());
	ImGui::Separator();
	int id = 0;
	
	for (auto& gamepad : mGamepads)
	{
		ImGui::Text("Gamepad %d", id);

		gamepad.ImGuiUpdate();

		id++;
	}


	ImGui::End();

}
