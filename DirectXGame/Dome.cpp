#include "Dome.h"

void Dome::Initialize(KamataEngine::Model* model, KamataEngine::Camera* camera) {
	model_ = model;
	worldTransform_.Initialize();
	camera_ = camera;
}

void Dome::Update() { 
	worldTransform_.TransferMatrix(); 
}

void Dome::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *camera_);
}
