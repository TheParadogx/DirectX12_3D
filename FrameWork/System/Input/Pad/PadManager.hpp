#pragma once
#include"Pad.hpp"

#include<vector>
#include<span>

namespace Engine::Input
{
	class PadManager
	{
		/// <summary>
		/// インデックスが範囲内か判定するメソッド
		/// </summary>
		/// <param name="index"></param>
		/// <returns>true:範囲内</returns>
		bool IsValidIndex(const uint8_t Index) const;

		/// <summary>
		/// 渡されたボタンが有効かどうかの判定
		/// </summary>
		/// <param name="Button"></param>
		/// <returns></returns>
		bool IsValidButtonCode(ePadButton Button)const;
	public:
		/// <summary>
		/// 初期化
		/// </summary>
		PadManager();
		virtual ~PadManager() = default;

		/// <summary>
		/// 各Padの状態更新
		/// </summary>
		void Update();

		/// <summary>
		/// 押した瞬間
		/// </summary>
		/// <param name="button">ボタン種別</param>
		/// <returns>true:押した瞬間</returns>
		bool IsPressed(const ePadButton Button,const ePadIndex Index = ePadIndex::Pad1) const;

		/// <summary>
		/// 押している間
		/// </summary>
		/// <param name="button"></param>
		/// <returns>true:押されている</returns>
		bool IsHeld(const ePadButton Button,const ePadIndex Index = ePadIndex::Pad1) const;

		/// <summary>
		/// 離した瞬間
		/// </summary>
		/// <param name="button"></param>
		/// <returns>true:離した瞬間</returns>
		bool IsReleased(const ePadButton Button, const ePadIndex Index = ePadIndex::Pad1) const;

		/// <summary>
		/// 3D用の左スティック取得
		/// </summary>
		/// <returns></returns>
		Math::Vector2 GetLeftStick3D(const ePadIndex Index = ePadIndex::Pad1)const;

		/// <summary>
		/// 3D用の右スティック取得
		/// </summary>
		/// <returns></returns>
		Math::Vector2 GetRightStick3D(const ePadIndex Index = ePadIndex::Pad1)const;

		/// <summary>
		/// 2D用の左スティック取得
		/// </summary>
		/// <returns></returns>
		Math::Vector2 GetLeftStick2D(const ePadIndex Index = ePadIndex::Pad1)const;

		/// <summary>
		/// 右スティック取得
		/// </summary>
		/// <returns></returns>
		Math::Vector2 GetRightStick2D(const ePadIndex Index = ePadIndex::Pad1)const;


		/// <summary>
		/// 仕様可能なPadのIndexの一覧を返す
		/// </summary>
		/// <returns></returns>
		std::vector<ePadIndex> GetAvailableIndices()const;

		/// <summary>
		/// ImGuiの更新
		/// </summary>
		void ImGuiUpdate();

	private:
		/// <summary>
		/// Padのコレクション
		/// </summary>
		std::vector<Pad> mGamepads;
	};
}

