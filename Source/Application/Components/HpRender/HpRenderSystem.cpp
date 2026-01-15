#include "pch.h"
#include "HpRenderSystem.hpp"

#include"Application/Components/Status/StatusComponent.hpp"

/// <summary>
/// 体力コンポーネントからバーの長さと色を設定する
/// </summary>
void Engine::System::HpRenderSystem::PostUpdate(entt::registry& Reg, double DeltaTime)
{
	auto view = Reg.view<HpRenderComponent,StatusComponet>();
	view.each([&](auto entity,auto& render,auto&status) 
	{
			//	今の体力の割合の取得
			auto ratio = status.GetHpRatio();
			//	割合のセット
			render.BarSprite->SetFillRatio(ratio);

			//	割合に応じて色を変更する
			if (render.isColorOverridden == true)
			{

			}
			else
			{
				UpdateColor(render, ratio);
			}


			//	* 頭の上に表示し続けたいときは専用のコンポーネントを
			//	作成してこの下に座標更新ロジックを追加。

	});
}

void Engine::System::HpRenderSystem::Render(entt::registry& Reg)
{
	auto view = Reg.view<HpRenderComponent>();
	view.each([&](auto entity, auto& render)
		{
			if (render.BaseSprite == nullptr || render.BarSprite == nullptr)
			{
				return;
			}

			render.BaseSprite->Render();
			render.BarSprite->Render();
	});

}

/// <summary>
/// 体力の残量で色を変更する
/// </summary>
void Engine::System::HpRenderSystem::UpdateColor(HpRenderComponent& Component, float Ratio)
{
	//	50%より上
	if (Ratio > 0.5f)
	{
		Component.BarSprite->SetColor(Graphics::Color::Green());
	}
	//	20%より上50%以下
	else if (Ratio > 0.2f)
	{
		Component.BarSprite->SetColor(Graphics::Color::Yellow());
	}
	//	20%以下
	else
	{
		Component.BarSprite->SetColor(Graphics::Color::Red());
	}
}

