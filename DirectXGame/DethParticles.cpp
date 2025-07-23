#include "DethParticles.h"

void DethParticles::Initialize(Model* model, Camera* camera, const Vector3& position) {
	model_ = model;
	camera_ = camera;
	
	// ワールド変換の初期化
	for (WorldTransform& worldTransform : worldTransform_) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
}

void DethParticles::Update() {
	// ワールド変換の更新
	for (auto& worldTransform : worldTransform_) {
		WtfUpdate(worldTransform);
	}
}

void DethParticles::Draw() {
	for (auto& worldTransform : worldTransform_) {
		model_->Draw(worldTransform, *camera_);
	}
}
