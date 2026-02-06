#include "pch.h"
#include "GlowSystem.hpp"
#include "../GlowComponents.hpp"
#include "System/Conponent/Sprite/SpriteComponent.hpp"
#include "Math/Random/Random.hpp"

void Engine::System::GlowSystem::Update(entt::registry& Reg, float DeltaTime)
{
	Reg.view<GlowComponent, SpriteComponent>()
		.each([&](auto entity, GlowComponent& glow, SpriteComponent& sprite)
			{
				float nextIntensity = glow.BaseIntensity;

				//	íºê¸
				if (LinearShiftTag* liner = Reg.try_get<LinearShiftTag>(entity))
				{
					glow.Timer += DeltaTime;
					float t = std::clamp(glow.Timer / liner->Duration, 0.0f, 1.0f);
					nextIntensity = std::lerp(liner->StartIntensity, liner->EndIntensity, t);

					if (t >= 1.0f && liner->AutoDestroy == true)
					{
						Reg.remove<LinearShiftTag>(entity);
						glow.Timer = 0.0f;
					}
				}
				//	Ç»ÇØÇÍÇŒå€ìÆ
				else if (PulseTag* pulse = Reg.try_get<PulseTag>(entity))
				{
					glow.Timer += DeltaTime;
					float t = (std::sin(glow.Timer * (2.0f * 3.14159f / pulse->Duration)) * 0.5f) + 0.5f;
					nextIntensity = glow.BaseIntensity + (t * pulse->Range);
				}

				sprite.Sprite.SetIntensity(nextIntensity);

				//	âŒâ‘Ç™Ç†Ç¡ÇΩÇÁ
				if (SparkTag* spark = Reg.try_get<SparkTag>(entity))
				{
					if (Random::Range(0.0f, 1.0f) < spark->Chance)
					{
						float current = sprite.Sprite.GetIntensity();
						sprite.Sprite.SetIntensity(current * spark->BoostMultiplier);
					}
				}

			});
}
