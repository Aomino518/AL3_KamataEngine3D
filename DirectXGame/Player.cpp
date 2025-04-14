#include "Player.h"
#include "cassert"

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
/// <param name="textureHndle">テクスチャハンドル</param>
/// <param name="camera">カメラ</param>
void Player::Initialize(KamataEngine::Model* model, uint32_t textureHandle, KamataEngine::Camera* camera) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;
	worldTransform_.Initialize();
	camera_ = camera;
}

/// <summary>
/// 更新
/// </summary>
void Player::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

/// <summary>
/// 描画
/// </summary>
void Player::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *camera_, textureHandle_);
}