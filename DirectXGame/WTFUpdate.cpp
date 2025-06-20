#include "WTFUpdate.h"
#include "Matrix.h"

namespace KamataEngine {

void WtfUpdate(WorldTransform& worldTransform) {

	Matrix4x4 affin_mat = MakeAffineMatrix(worldTransform.scale_, worldTransform.rotation_, worldTransform.translation_);
	// アフィン変換行列で計算
	worldTransform.matWorld_ = affin_mat;
	// 行列を定数バッファに転送
	worldTransform.TransferMatrix();
}

} // namespace KamataEngine
