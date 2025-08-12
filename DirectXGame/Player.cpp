#include "Player.h"

void Player::Init(Model* model, Camera* camera) { 
	// NULLポインタチェック
	assert(model); 
	model_ = model;
	worldTransform_.Initialize();
	camera_ = camera;
}

void Player::Update() { 
	worldTransform_.TransferMatrix(); 
}

void Player::Draw() { 
	model_->Draw(worldTransform_, *camera_); 
}
