#include "Skydome.h"
#include <cassert>

namespace KamataEngine {
/* Skydome::~Skydome() { 
	delete model_;
	delete camera_;
}*/

/// <summary>
/// 初期化
/// </summary>
void Skydome::Initialize(Model* model, Camera* camera) { 
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	camera_ = camera;
}

/// <summary>
/// 更新
/// </summary>
void Skydome::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

/// <summary>
/// 描画
/// </summary>
void Skydome::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *camera_);
}
} // namespace KamataEngine
