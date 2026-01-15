#pragma once

namespace Engine::System
{
	//	後から加算補正値やバフ数値などを追加できるようにしときます。
	struct StatusValue
	{
		float Base = 0.0f;		//	基準値

		//	現在数値の取得（後から個々だけ変更したらいいように）
		float GetCurrent()const noexcept
		{
			return Base;
		}
	};

	/// <summary>
	/// ステータスを管理するコンポーネント
	/// </summary>
	struct StatusComponet
	{
		StatusValue MaxHp = {1.0f};
		StatusValue Attack = { 1.0f };
		float Hp = 1.0f;
		//	他のステータスは必要になったら

		//	残りの体力の割合
		float GetHpRatio()
		{
			if (Hp <= 0 || MaxHp.Base <= 0)
			{
				return 0.0f;
			}

			return Hp / MaxHp.GetCurrent();

		}
	};
}