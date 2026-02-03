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

				//	’¼ü
				if (LinearShiftTag* liner = Reg.try_get<LinearShiftTag>(entity))
				{
					glow.Timer += DeltaTime;
					float t = std::clamp(glow.Timer / liner->Duration, 0.0f, 1.0f);
					nextIntensity = std::lerp(liner->StartIntensity, liner->EndIntensity, t);
				}
				//	‚È‚¯‚ê‚ÎŒÛ“®
				else if (PulseTag* pulse = Reg.try_get<PulseTag>(entity))
				{
					glow.Timer += DeltaTime;
					float t = (std::sin(glow.Timer * (2.0f * 3.14159f / pulse->Duration)) * 0.5f) + 0.5f;
					nextIntensity = glow.BaseIntensity + (t * pulse->Range);
				}

				sprite.Sprite.SetIntensity(nextIntensity);

				//	‰Î‰Ô‚ª‚ ‚Á‚½‚ç
				if (SparkTag* spark = Reg.try_get<SparkTag>(entity))
				{
					if (Math::Random::Range(0.0f, 1.0f) < spark->Chance)
					{
						float current = sprite.Sprite.GetIntensity();
						sprite.Sprite.SetIntensity(current * spark->BoostMultiplier);
					}
				}

			});
}
