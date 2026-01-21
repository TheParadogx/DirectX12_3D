#pragma once
#include"System/Singleton/Singleton.hpp"
#include"System/Entity/System/Base/ISystem.hpp"

#include<memory>
#include<vector>
#include<concepts>

namespace Engine::System
{

	template<typename T>
	concept IsSystem = std::derived_from<T, ISystem>;

	/// <summary>
	/// システムの管理
	/// </summary>
	class SystemManager : public Singleton<SystemManager>
	{
		GENERATE_SINGLETON_BODY(SystemManager);
	public:

		/// <summary>
		/// 可変更新
		/// </summary>
		/// <param name="Reg">ecs管理</param>
		/// <param name="DeltaTime">デルタタイム</param>
		void PreUpdate(entt::registry& Reg, double DeltaTime);

		/// <summary>
		/// 固定更新
		/// </summary>
		/// <param name="Reg">ecs管理</param>
		/// <param name="DeltaTime">デルタタイム</param>
		void MainUpdate(entt::registry& Reg, double DeltaTime);

		/// <summary>
		/// 事後更新
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		void PostUpdate(entt::registry& Reg, double DeltaTime);

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="Alpha"></param>
		void Render(entt::registry& Reg);

		/// <summary>
		/// システムの追加
		/// </summary>
		/// <param name="System">システムのポインタ</param>
		template<IsSystem T,typename... Args>
		void AddSystem(Args&&... args)
		{
			mSystems.push_back(std::make_unique<T>(std::forward<Args>(args)...));
		}

		/// <summary>
		/// システムの削除
		/// </summary>
		/// <typeparam name="T"></typeparam>
		template<IsSystem T>
		void RemoveSystem()
		{
			std::erase_if(mSystems,
				[](const std::unique_ptr<ISystem>& sys)
				{
					// sys.get() を閉じてから nullptr と比較する
					if (dynamic_cast<T*>(sys.get()) != nullptr)
					{
						sys->Release();
						return true;
					}
					return false;
				});
		}

		/// <summary>
		/// システムの全削除
		/// </summary>
		void AllClearSystem();

	private:
		/// <summary>
		/// ユーザー定義のシステム
		/// </summary>
		std::vector<std::unique_ptr<ISystem>> mSystems;
	};
}

