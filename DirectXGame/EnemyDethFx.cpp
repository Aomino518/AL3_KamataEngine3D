#define NOMINMAX
#include "EnemyDethFx.h"
#include "Matrix.h"

void EnemyDethFx::Init(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::WorldTransform& worldTransform) { 
	model_ = model;
	camera_ = camera;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = worldTransform.translation_;
	worldTransform_.rotation_ = worldTransform.rotation_;
	worldTransform_.scale_ = worldTransform.scale_;
}

void EnemyDethFx::Update() {
	if (isFinished_) {
		return;
	}

	worldTransform_.scale_.y = std::max(worldTransform_.scale_.y - 0.01f, finishScale);

	 if (worldTransform_.scale_.y <= finishScale) {
		worldTransform_.scale_.y = finishScale;
		isFinished_ = true;
	 }

	WtfUpdate(worldTransform_);
}

void EnemyDethFx::Draw() { 
	if (isFinished_) {
		return;
	}

	model_->Draw(worldTransform_, *camera_); 
}
