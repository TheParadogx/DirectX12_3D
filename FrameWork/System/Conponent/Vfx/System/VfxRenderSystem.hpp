#pragma once
namespace Engine::System
{
	class VfxRenderSystem
	{
	public:
		/// <summary>
		/// アニメーションの状態更新
		/// </summary>
		/// <param name="Reg"></param>
		/// <param name="DeltaTime"></param>
		static void Update(entt::registry& Reg);

		/// <summary>
		/// 描画
		/// </summary>
		/// <param name="Reg">enttレジストリ</param>
		/// <param name="Alpha">補間係数</param>
		static void Render(entt::registry& Reg);

	private:

	};

}

