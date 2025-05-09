#include "Player.h"
#include <cassert>
#include "Matrix.h"
#include "WTFUpdate.h"

namespace KamataEngine {

/// <summary>
/// 初期化
/// </summary>
void Player::Initialize(Model* model, Camera* camera) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	camera_ = camera;
}

/// <summary>
/// 更新
/// </summary>
void Player::Update() { 
	WtfUpdate(worldTransform_); 
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *camera_);
}

} // namespace KamataEngine