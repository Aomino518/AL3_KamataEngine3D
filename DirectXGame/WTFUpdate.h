#pragma once
#include <KamataEngine.h>

namespace KamataEngine {
/// <summary>
/// 行列を計算・転送する
/// </summary>
/// <param name="worldTransform">ワールドトランスフォーム</param>
void WtfUpdate(WorldTransform& worldTransform);
}