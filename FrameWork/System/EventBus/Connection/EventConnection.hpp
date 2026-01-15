#pragma once
#include<functional>
#include<Utility>


namespace Engine::System
{
	class EventConnection
	{
	public:
		EventConnection() = default;

		/// <summary>
		/// マネージャーから渡される解除用関数を保持
		/// </summary>
		explicit EventConnection(std::function<void()>unbinder)
			:mUnbinder(std::move(unbinder))
		{

		}

		/// <summary>
		/// デストラクタで自動的に登録解除
		/// </summary>
		virtual ~EventConnection()
		{
			this->Disconnect();
		}

		//	コピーは禁止・二重で解除されないため
		EventConnection(const EventConnection&) = delete;
		EventConnection& operator=(const EventConnection&) = delete;

		//	関数の戻り値で渡せるようにムーブは許可する
		EventConnection(EventConnection&& rhs) noexcept : mUnbinder(std::move(rhs.mUnbinder)) {
			rhs.mUnbinder = nullptr;
		}
		EventConnection& operator=(EventConnection&& rhs) noexcept {
			if (this != &rhs) {
				Disconnect();
				mUnbinder = std::move(rhs.mUnbinder);
				rhs.mUnbinder = nullptr;
			}
			return *this;
		}

		/// <summary>
		/// 管理クラスの登録解除の関数の呼び出し
		/// </summary>
		void Disconnect() {
			if (mUnbinder != nullptr) {
				mUnbinder();
				mUnbinder = nullptr;
			}
		}

	private:
		/// <summary>
		/// 解除関数保持
		/// </summary>
		std::function<void()> mUnbinder;
	};
}

