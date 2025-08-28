#include "Enemy.h"
#include <cassert>
#include <numbers>

void Enemy::Init(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3 position) { 
	assert(model); 

	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	sHStomp = KamataEngine::Audio::GetInstance()->LoadWave("./Resources/sounds/se_Stomp.mp3");
}

void Enemy::Update() {
	if (isDead_) {
		return;
	}

	move_ += velocity_;

	if (move_ >= 4.0f) {
		velocity_ *= -1.0f;
		if (lrDirection_ != LRDirection::kRight) {
			lrDirection_ = LRDirection::kRight;
			// 旋回開始時の角度を記録
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			// 旋回タイマーに時間を記録する
			turnTimer_ = kTimeTurn;
		}
	}

	if (move_ <= -4.0f) {
		velocity_ *= -1.0f;
		if (lrDirection_ != LRDirection::kLeft) {
			lrDirection_ = LRDirection::kLeft;
			// 旋回開始時の角度を記録
			turnFirstRotationY_ = worldTransform_.rotation_.y;
			// 旋回タイマーに時間を記録する
			turnTimer_ = kTimeTurn;
		}
	}

	worldTransform_.translation_.x -= velocity_;

	if (turnTimer_ > 0.0f) {
		turnTimer_ -= static_cast<float>(1) / 60;
		float time_ = 1.0f - (turnTimer_ / kTimeTurn);

		// 左右の自キャラ角度テーブル
		float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};

		// 状態応じた角度を取得する
		float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
		// 自キャラの角度を設定する
		worldTransform_.rotation_.y = turnFirstRotationY_ + (destinationRotationY - turnFirstRotationY_) * time_;
	}

	walkTimer += 1.0f / 60.0f;

	float param = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer / kWalkMotionTime);

	float degree = 45.0f + (-45.0f) * (param + 1.0f) / 2.0f;

	worldTransform_.rotation_.x = degree * (std::numbers::pi_v<float> / 180.0f);

	// 行列の更新
	WtfUpdate(worldTransform_);
}

void Enemy::Draw() { 
	if (isDead_) {
		return;
	}

	model_->Draw(worldTransform_, *camera_); 
}

Vector3 Enemy::GetWorldPosition() const {
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

KamataEngine::AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	KamataEngine::AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) { 
	(void)player; 
	vHStomp = KamataEngine::Audio::GetInstance()->PlayWave(sHStomp, false, 1.0f);
}
