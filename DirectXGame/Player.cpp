#define NOMINMAX
#include "Player.h"
#include <algorithm>
#include <cassert>
#include <numbers>

using namespace KamataEngine;

void Player::Init(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position) {
	assert(model && "Player::Init: model is null");
	assert(camera && "Player::Init: camera is null");
	if (!model || !camera) {
		return;
	}

	model_ = model;
	camera_ = camera;

	// 変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Player::Update() {

	// 移動
	InputMove();

	// 着地フラグ
	bool landing = false;

	// 地面との当たり判定
	// 下降中か
	if (velocity_.y < 0.0f) {
		if (worldTransform_.translation_.y <= 2.0f) {
			landing = true;
		}
	}

	// 設置判定
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態へ移行
			onGround_ = false;
		}
	} else {
		// 着地
		if (landing) {
			// めり込み排斥
			worldTransform_.translation_.y = 2.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}

	// 行列の更新
	WtfUpdate(worldTransform_);
}

void Player::Draw() {
	// ヌル防御：どちらか欠けたら描画しない
	if (!model_ || !camera_) {
		return;
	}

	// モデル描画
	model_->Draw(worldTransform_, *camera_);
}

void Player::InputMove() {
	// 接地状態
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A)) {
			Vector3 acceleration = {};
			// 左右加速
			if (Input::GetInstance()->PushKey(DIK_D)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
			} else if (Input::GetInstance()->PushKey(DIK_A)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
			}

			// 加速と減速
			velocity_ += acceleration;
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (onGround_ && Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration / 15.0f, 0);
		}
		// 空中にいるとき
	} else {
		if (Input::GetInstance()->PushKey(DIK_D) || Input::GetInstance()->PushKey(DIK_A)) {
			Vector3 acceleration = {};
			// 左右加速
			if (Input::GetInstance()->PushKey(DIK_D)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x += kAcceleration;
			} else if (Input::GetInstance()->PushKey(DIK_A)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration;
			}

			// 加速と減速
			velocity_ += acceleration;
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (!onGround_ && !Input::GetInstance()->PushKey(DIK_SPACE)) {
			velocity_.y = -kJumpAcceleration / 30.0f;
		}

		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration / 40.0f, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// 移動
	worldTransform_.translation_ += velocity_;
}

