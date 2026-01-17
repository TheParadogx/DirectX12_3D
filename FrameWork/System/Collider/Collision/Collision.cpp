#include "pch.h"
#include "Collision.hpp"

#include"System/Collider/AABB/AABBCollider.hpp"
#include"System/Collider/Sphere/SphereCollider.hpp"

/// <summary>
/// AABBど氏の当たり判定
/// </summary>
/// <param name="Collider1">オブジェクト１</param>
/// <param name="Collider2">オブジェクト２</param>
/// <param name="OutVector">押し戻しの移動ベクトル</param>
/// <returns>true:接触</returns>
bool Engine::System::Collision::ComputeAABBCollision(const AABBCollider* Collider1, const AABBCollider* Collider2, Math::Vector3& OutVector)
{
    if (Collider1 == nullptr || Collider2 == nullptr)
    {
        return false;
    }

    //  初期化
    OutVector = Math::Vector3::Zero;

    //  各軸の範囲取得
    const AABBCollider::Range Range1X = Collider1->GetAxisXRange();
    const AABBCollider::Range Range1Y = Collider1->GetAxisYRange();
    const AABBCollider::Range Range1Z = Collider1->GetAxisZRange();

    const AABBCollider::Range Range2X = Collider2->GetAxisXRange();
    const AABBCollider::Range Range2Y = Collider2->GetAxisYRange();
    const AABBCollider::Range Range2Z = Collider2->GetAxisZRange();

    //  衝突判定
    const bool  isHit = 
        Range1X.Min < Range2X.Max && 
        Range1X.Max > Range2X.Min &&
        Range1Y.Min < Range2Y.Max && 
        Range1Y.Max > Range2Y.Min &&
        Range1Z.Min < Range2Z.Max && 
        Range1Z.Max > Range2Z.Min;

    if (isHit == false)
    {
        return false;
    }

    //  押し戻し量の計算
    const Math::Vector3 Center1 = Collider1->GetCenter();
    const Math::Vector3 Center2 = Collider2->GetCenter();
    const Math::Vector3 Volume1 = Collider1->GetVolume();
    const Math::Vector3 Volume2 = Collider2->GetVolume();

    //  Collider1からCollider2へのベクトル
    const Math::Vector3 toOther = Center2 - Center1;

    //  各軸の最大許容距離から現在の中心間距離を引いて重なっている量を出す
    float overlapX = (Volume1.x * 0.5f + Volume2.x * 0.5f) - fabsf(toOther.x);
    float overlapY = (Volume1.y * 0.5f + Volume2.y * 0.5f) - fabsf(toOther.y);
    float overlapZ = (Volume1.z * 0.5f + Volume2.z * 0.5f) - fabsf(toOther.z);

    //  最小貫通軸の取得
    //  もっとも浅い方向に押し出す。
    //  回転なしの斜めからのめり込みに対して変な方向に飛ばないようにする
    if (overlapX < overlapY && overlapX < overlapZ)
    {
        // X軸で押し戻す（Collider2が右にいるなら、Collider1は左へ）
        OutVector.x = (toOther.x > 0) ? -overlapX : overlapX;
    }
    else if (overlapY < overlapX && overlapY < overlapZ)
    {
        // Y軸で押し戻す
        OutVector.y = (toOther.y > 0) ? -overlapY : overlapY;
    }
    else
    {
        // Z軸で押し戻す
        OutVector.z = (toOther.z > 0) ? -overlapZ : overlapZ;
    }
    return true;
}

/// <summary>
/// 円同士の当たり判定
/// </summary>
/// <param name="Collider1">オブジェクト１</param>
/// <param name="Collider2">オブジェクト２</param>
/// <param name="OutVector">押し戻しの移動ベクトル</param>
/// <returns>true:接触</returns>
bool Engine::System::Collision::ComputeSphepeCollision(const SphereCollider* Collider1, const SphereCollider* Collider2, Math::Vector3& OutVector)
{
    //  初期化
    OutVector = Math::Vector3::Zero;

    //  中心座標間のベクトル
    Math::Vector3 v = Collider1->GetCenter() - Collider2->GetCenter();
    float dis2 = v.SqrLength();
    float radSum = Collider1->GetRadius() + Collider2->GetRadius();

    //  衝突判定
    //  true:衝突
    if (dis2 <= radSum * radSum)
    {
        float dis = sqrtf(dis2);

        //  ほぼ完全に重なっているときは真上に押し出す
        if (dis < 0.0001f)
        {
            OutVector = Math::Vector3(0.0f, 1.0f, 0.0f) * radSum;
            return true;
        }

        //  押し戻し量の計算
        //  押し戻すべき距離＝（半径の合計）ー（現在の距離）
        float overlap = radSum - dis;

        //  押し戻す方向
        //  （方向ベクトル）＊（重なり量）
        //  V / disでベクトルの正規化
        OutVector = (v / dis) * overlap;
        return true;
    }

    return false;
}

bool Engine::System::Collision::ComputeCollision(const AABBCollider* Collider1, const AABBCollider* Collider2, Math::Vector3& OutVector)
{
    if (Collider1 == nullptr || Collider2 == nullptr)
    {
        return false;
    }

    //  初期化
    OutVector = Math::Vector3::Zero;

    //  各軸の範囲取得
    const AABBCollider::Range Range1X = Collider1->GetAxisXRange();
    const AABBCollider::Range Range1Y = Collider1->GetAxisYRange();
    const AABBCollider::Range Range1Z = Collider1->GetAxisZRange();

    const AABBCollider::Range Range2X = Collider2->GetAxisXRange();
    const AABBCollider::Range Range2Y = Collider2->GetAxisYRange();
    const AABBCollider::Range Range2Z = Collider2->GetAxisZRange();

    //  衝突判定
    const bool  isHit =
        Range1X.Min < Range2X.Max &&
        Range1X.Max > Range2X.Min &&
        Range1Y.Min < Range2Y.Max &&
        Range1Y.Max > Range2Y.Min &&
        Range1Z.Min < Range2Z.Max &&
        Range1Z.Max > Range2Z.Min;

    if (isHit == false)
    {
        return false;
    }

    //  押し戻し量の計算
    const Math::Vector3 Center1 = Collider1->GetCenter();
    const Math::Vector3 Center2 = Collider2->GetCenter();
    const Math::Vector3 Volume1 = Collider1->GetVolume();
    const Math::Vector3 Volume2 = Collider2->GetVolume();

    //  Collider1からCollider2へのベクトル
    const Math::Vector3 toOther = Center2 - Center1;

    //  各軸の最大許容距離から現在の中心間距離を引いて重なっている量を出す
    float overlapX = (Volume1.x * 0.5f + Volume2.x * 0.5f) - fabsf(toOther.x);
    float overlapY = (Volume1.y * 0.5f + Volume2.y * 0.5f) - fabsf(toOther.y);
    float overlapZ = (Volume1.z * 0.5f + Volume2.z * 0.5f) - fabsf(toOther.z);

    //  最小貫通軸の取得
    //  もっとも浅い方向に押し出す。
    //  回転なしの斜めからのめり込みに対して変な方向に飛ばないようにする
    if (overlapX < overlapY && overlapX < overlapZ)
    {
        // X軸で押し戻す（Collider2が右にいるなら、Collider1は左へ）
        OutVector.x = (toOther.x > 0) ? -overlapX : overlapX;
    }
    else if (overlapY < overlapX && overlapY < overlapZ)
    {
        // Y軸で押し戻す
        OutVector.y = (toOther.y > 0) ? -overlapY : overlapY;
    }
    else
    {
        // Z軸で押し戻す
        OutVector.z = (toOther.z > 0) ? -overlapZ : overlapZ;
    }
    return true;
}

bool Engine::System::Collision::ComputeCollision(const SphereCollider* Collider1, const SphereCollider* Collider2, Math::Vector3& OutVector)
{
    //  初期化
    OutVector = Math::Vector3::Zero;

    //  中心座標間のベクトル
    Math::Vector3 v = Collider1->GetCenter() - Collider2->GetCenter();
    float dis2 = v.SqrLength();
    float radSum = Collider1->GetRadius() + Collider2->GetRadius();

    //  衝突判定
    //  true:衝突
    if (dis2 <= radSum * radSum)
    {
        float dis = sqrtf(dis2);

        //  ほぼ完全に重なっているときは真上に押し出す
        if (dis < 0.0001f)
        {
            OutVector = Math::Vector3(0.0f, 1.0f, 0.0f) * radSum;
            return true;
        }

        //  押し戻し量の計算
        //  押し戻すべき距離＝（半径の合計）ー（現在の距離）
        float overlap = radSum - dis;

        //  押し戻す方向
        //  （方向ベクトル）＊（重なり量）
        //  V / disでベクトルの正規化
        OutVector = (v / dis) * overlap;
        return true;
    }

    return false;

}
