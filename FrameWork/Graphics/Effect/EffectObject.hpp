#pragma once
#include<Effekseer.h>
#include"Math/Vector3/Vector3.h"

namespace Engine::Graphics
{
	class EffectObject
	{
	public:
		EffectObject() = default;
		virtual ~EffectObject() { Stop(); }

		void Play(Effekseer::EffectRef effect, const Math::Vector3& position);

		// 更新系（内部でマネージャーを自動取得）
		void SetLocation(const Math::Vector3& position);
		void SetRotation(const Math::Vector3& rotation);
		void SetScale(const Math::Vector3& scale);

		void Stop();
		bool IsPlaying() const;

	protected:
		/// <summary>
		/// 再生したインスタンスのハンドル
		/// </summary>
		Effekseer::Handle mHandle = -1;



	};
}