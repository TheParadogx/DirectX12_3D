#pragma once
#include"Effekseer.h"
#include"Math/Vector3/Vector3.h"

namespace Engine::System
{
	class EffectFactory
	{
	public:
        /// <summary>
        /// 設置型のオブジェクト
        /// </summary>
        /// <param name="Reg"></param>
        /// <param name="Asset">リソース</param>
        /// <param name="Position">座標</param>
        /// <param name="Scale">スケール</param>
        /// <returns></returns>
        static entt::entity CreateAtLocation(
            Effekseer::EffectRef Asset,
            const Math::Vector3& Position,
            const Math::Vector3& Scale = { 1,1,1 },
            bool IsLoop = false
        );

        /// <summary>
        /// 追従型（親エンティティにくっつける）
        /// </summary>
        /// <param name="IsLoop">true:ループ</param>
        /// <returns></returns>
        static entt::entity CreateAttached(
            Effekseer::EffectRef Asset,
            entt::entity Parent,
            const Math::Vector3& Offset = { 0,0,0 },
            bool IsLoop = false
        );
	};
}


