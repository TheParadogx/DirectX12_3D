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

		/// <summary>
		/// エフェクトの再生
		/// </summary>
		/// <param name="effect"></param>
		void Play(Effekseer::EffectRef Effect, const Math::Vector3& Position,bool AutoDelete = false);

		//	情報更新
		void SetLocation(const Math::Vector3& Position);
		void SetRotation(const Math::Vector3& Rotation);
		void SetScale(const Math::Vector3& Scale);
		void SetSpeed(float Speed);

		/// <summary>
		/// ハンドルの解放
		/// </summary>
		void Stop();
		/// <summary>
		/// 再生中かどうかの判定
		/// </summary>
		/// <returns>true:存在している</returns>
		bool IsPlaying() const;
		/// <summary>
		/// 自動削除の条件判定
		/// </summary>
		/// <returns></returns>
		bool ShouldDestroy() const;

		/// <summary>
		/// ハンドルの取得
		/// </summary>
		/// <returns></returns>
		Effekseer::Handle GetHandle()const;
	protected:

		/// <summary>
		/// 再生したインスタンスのハンドル
		/// </summary>
		Effekseer::Handle mHandle = -1;

		/// <summary>
		/// エフェクトの再生速度
		/// </summary>
		float mSpeed = 1.0f;

		/// <summary>
		/// エフェクト終了時に自動削除をするかどうか
		/// </summary>
		bool mIsAutoDelete = false;
	};
}