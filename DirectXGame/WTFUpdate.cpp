#include "WTFUpdate.h"
#include "Matrix.h"

namespace KamataEngine {

void WtfUpdate(WorldTransform& worldTransform) {
	// アフィン変換行列で計算
	worldTransform.matWorld_ = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
	// 行列を定数バッファに転送
	worldTransform.TransferMatrix();
}

} // namespace KamataEngine
