#include "pch.h"
#include "HpRenderSystem.hpp"

#include"Application/Components/Status/StatusComponent.hpp"

/// <summary>
/// 体力コンポーネントからバーの長さと色を設定する
/// </summary>
void Engine::System::HpRenderSystem::PostUpdate(entt::registry& Reg, double DeltaTime)
{
	auto view = Reg.view<HpRenderComponent,StatusComponet>();
	view.each([&](auto entity, HpRenderComponent& render, StatusComponet&status)
	{
			//	今の体力の割合の取得
			auto ratio = status.GetHpRatio();


			//	黄色の実際のHPに近づける
			if (render.VisualRatio > ratio)
			{
				//	
				render.VisualRatio -= (render.VisualRatio - ratio) * (float)DeltaTime * render.ShrinkSpeed;
				if (render.VisualRatio < ratio) render.VisualRatio = ratio;
			}
			else
			{
				render.VisualRatio = ratio;
			}

			render.BarSprite->SetFillRatio(ratio);
			render.BarSpriteYellow->SetFillRatio(render.VisualRatio);

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

	OnGui(Reg);

}

void Engine::System::HpRenderSystem::Render(entt::registry& Reg)
{
	auto view = Reg.view<HpRenderComponent>();
	view.each([&](auto entity, HpRenderComponent& render)
		{
			if (render.BaseSprite == nullptr || render.BarSprite == nullptr)
			{
				return;
			}

			render.BaseSprite->Render();
			render.BarSpriteYellow->Render();
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

void Engine::System::HpRenderSystem::OnGui(entt::registry& Reg)
{
	if (ImGui::Begin("Entitys"))
	{

		auto view = Reg.view<HpRenderComponent>();
		view.each([&](auto entity, HpRenderComponent& render)
			{
				std::string label = "Entity: " + std::to_string((uint32_t)entity);
				if (ImGui::CollapsingHeader(label.c_str()))
				{
					ImGui::PushID((int)entity);

					//	座標
					float pos[2] = { render.Position.x, render.Position.y };
					if (ImGui::DragFloat2("Position", pos, 1.0f))
					{
						render.SetPosition({ pos[0], pos[1] });
					}

					// サイズ
					float size[2] = { render.Size.x, render.Size.y };
					if (ImGui::DragFloat2("Size", size, 1.0f))
					{
						render.SetSize({ size[0], size[1] });
					}

					//	スピード
					ImGui::SliderFloat("Shrink Speed", &render.ShrinkSpeed, 0.1f, 10.0f);
					ImGui::PopID();
				}
			});

	}

	ImGui::End();
}
