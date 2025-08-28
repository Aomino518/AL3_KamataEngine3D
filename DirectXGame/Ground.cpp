#include "Ground.h"

void Ground::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {
	model_ = model;
	worldTransform_.Initialize();
	camera_ = camera;
}

void Ground::Update() { 
	worldTransform_.TransferMatrix(); 
}

void Ground::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *camera_);
}
