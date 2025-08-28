#define NOMINMAX
#include "Player.h"
#include <algorithm>
#include <cassert>
#include <numbers>
#include "GameScene.h"
#include "MapChipField.h"
#define _USE_MATH_DEFINES
#include <math.h>

using namespace KamataEngine;

void Player::Init(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position) {
	assert(model && "Player::Init: model is null");
	assert(camera && "Player::Init: camera is null");
	if (!model || !camera) {
		return;
	}

	model_ = model;
	camera_ = camera;

	sHSeJump = Audio::GetInstance()->LoadWave("./Resources/sounds/se_jump_003.wav");

	// 変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = float(M_PI) / 2.0f;
}

void Player::Update() {

	// 移動
	InputMove();

	// 衝突情報を初期化
	collisionMapInfo.move = velocity_;

	// マップ衝突チェック
	CheckMapCollosion(collisionMapInfo);

	// 着地していたら速度をリセット
	if (collisionMapInfo.landing) {
		onGround_ = true;
		velocity_.y = 0.0f;
		collisionMapInfo.landing = false;
	}

	if (collisionMapInfo.ceiling && velocity_.y > 0.0f) {
		velocity_.y = 0.0f;
		collisionMapInfo.ceiling = false;
	}

	if (collisionMapInfo.hitWall) {
		velocity_.x = 0.0f;
	}

	// 移動
	worldTransform_.translation_ += collisionMapInfo.move;

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
				acceleration.x += kAcceleration / 60.0f;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					// 旋回開始時の角度を記録
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を記録する
					turnTimer_ = kTimeTurn;
				}

			} else if (Input::GetInstance()->PushKey(DIK_A)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration / 60.0f;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					// 旋回開始時の角度を記録
					turnFirstRotationY_ = worldTransform_.rotation_.y;
					// 旋回タイマーに時間を記録する
					turnTimer_ = kTimeTurn;
				}

			}

			// 加速と減速
			velocity_ += acceleration;
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (onGround_ && Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			vHSeJump = Audio::GetInstance()->PlayWave(sHSeJump, false, 0.5f);
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration / 15.0f, 0);
		}

		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			// 空中状態へ移行
			onGround_ = false;
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
				acceleration.x += kAcceleration / 60.0f;
			} else if (Input::GetInstance()->PushKey(DIK_A)) {
				// 右移動中の左入力
				if (velocity_.x > 0.0f) {
					velocity_.x *= (1.0f - kAttenuation);
				}
				acceleration.x -= kAcceleration / 60.0f;
			}

			// 加速と減速
			velocity_ += acceleration;
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			velocity_.x *= (1.0f - kAttenuation);
		}

		if (!Input::GetInstance()->PushKey(DIK_SPACE)) {
			// ジャンプ初速
			velocity_ += Vector3(0, -kGravityAcceleration / 40.0f, 0);
		}

		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration / 40.0f, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

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

}

// タイルがソリッドか
static  inline bool IsSolidCell(MapChipField* field, uint32_t tx, uint32_t ty) { 
	return field->GetMapChipTypeByIndex(tx, ty) == MapChipType::kBlock; 
}

// ワールド座標からタイル座標
static  inline MapChipField::IndexSet ToCell(MapChipField* field, const Vector3& pos) { 
	return field->GetMapChipIndexSetByPosition(pos); 
}

void Player::CheckMapCollosion(CollisionMapInfo& info) { 
	if (!mapChipField_) {
		onGround_ = false;
		return;
	}

	const float hw = kWidth * 0.5f; // 横
	const float hh = kHeight * 0.5f; // 縦
	const float kEps = kBlank;
	Vector3 pos = worldTransform_.translation_;

	// ---X軸---
	if (info.move.x != 0.0f) {
		float newX = pos.x + info.move.x;
		// 右か左に出る側の列だけを見る
		if (info.move.x > 0.0f) {
			// 右上と右下の2点をチェック
			Vector3 pTopRight = {newX + hw, pos.y + hh - kEps, 0};
			Vector3 pBottomRight = {newX + hw, pos.y - hh + kEps, 0};
			auto cTR = ToCell(mapChipField_, pTopRight);
			auto cBR = ToCell(mapChipField_, pBottomRight);

			if (IsSolidCell(mapChipField_, cTR.xIndex, cTR.yIndex) || 
				IsSolidCell(mapChipField_, cBR.xIndex, cBR.yIndex)) {
				// 右タイルの左端にピッタリ揃える
				MapChipField::Rect rect = mapChipField_->GetRectByIndex(cTR.xIndex, cTR.yIndex);
				newX = rect.left - hw - kEps;
				info.move.x = newX - pos.x;
				info.hitWall = true;
			} else {
				info.hitWall = false;
			}

		} else {
			// 左上と左下
			Vector3 pTopLeft = {newX - hw, pos.y + hh - kEps, 0};
			Vector3 pBottomLeft = {newX - hw, pos.y - hh + kEps, 0};
			auto cTL = ToCell(mapChipField_, pTopLeft);
			auto cBL = ToCell(mapChipField_, pBottomLeft);
			if (IsSolidCell(mapChipField_, cTL.xIndex, cTL.yIndex) || 
				IsSolidCell(mapChipField_, cBL.xIndex, cBL.yIndex)) {
				// 左タイルの右端にピッタリ揃える
				MapChipField::Rect rect = mapChipField_->GetRectByIndex(cTL.xIndex, cTL.yIndex);
				newX = rect.right + hw + kEps;
				info.move.x = newX - pos.x;
				info.hitWall = true;
			} else {
				info.hitWall = false;
			}
		}
		pos.x += info.move.x;
	}

	// ---Y軸---
	if (info.move.y != 0.0f) {
		float newY = pos.y + info.move.y;
		if (info.move.y > 0.0f) {
			// 天井
			Vector3 pTopLeft = {pos.x - hw + kEps, newY + hh, 0};
			Vector3 pTopRight = {pos.x + hw - kEps, newY + hh, 0};
			auto cTL = ToCell(mapChipField_, pTopLeft);
			auto cTR = ToCell(mapChipField_, pTopRight);

			if (IsSolidCell(mapChipField_, cTL.xIndex, cTL.yIndex) || 
				IsSolidCell(mapChipField_, cTR.xIndex, cTR.yIndex)) {
				MapChipField::Rect rect = mapChipField_->GetRectByIndex(cTL.xIndex, cTL.yIndex);
				newY = rect.bottom - hh - kEps;
				info.move.y = newY - pos.y;
				info.ceiling = true;
			} else {
				info.ceiling = false;
			}

		} else {
			// 床
			Vector3 pBL = {pos.x - hw + kEps, newY - hh, 0};
			Vector3 pBR = {pos.x + hw - kEps, newY - hh, 0};
			auto cBL = ToCell(mapChipField_, pBL);
			auto cBR = ToCell(mapChipField_, pBR);
			if (IsSolidCell(mapChipField_, cBL.xIndex, cBL.yIndex) || 
				IsSolidCell(mapChipField_, cBR.xIndex, cBR.yIndex)) {
				MapChipField::Rect rect = mapChipField_->GetRectByIndex(cBL.xIndex, cBL.yIndex);
				newY = rect.top + hh;
				info.move.y = newY - pos.y;
				info.landing = true;
			}
		}
		pos.y += info.move.y;
	} else {
		const float probe = kBlank;
		const float footY = pos.y - hh;
		Vector3 pL = {pos.x - hw + kEps, footY - probe, 0};
		Vector3 pR = {pos.x + hw - kEps, footY - probe, 0};
		auto cL = ToCell(mapChipField_, pL);
		auto cR = ToCell(mapChipField_, pR);
		bool hasSupport = IsSolidCell(mapChipField_, cL.xIndex, cL.yIndex) || IsSolidCell(mapChipField_, cR.xIndex, cR.yIndex);

		if (!hasSupport) {
			onGround_ = false;
			if (info.move.y == 0.0f) {
				info.move.y = -probe;
				pos.y += info.move.y;
			}
		}
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

AABB Player::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	const float eps = 0.2f;

	aabb.min = {(worldPos.x - kWidth / 2.0f) - eps, (worldPos.y - kHeight / 2.0f) - eps, (worldPos.z - kWidth / 2.0f) - eps};
	aabb.max = {(worldPos.x + kWidth / 2.0f) - eps, (worldPos.y + kHeight / 2.0f) - eps, (worldPos.z + kWidth / 2.0f) - eps};

	return aabb;
}

void Player::OnCollision(const Enemy* enemy) { 
	(void)enemy; 
	isDead_ = true;
}

int Player::SetAddItemCount(int num) { 
	return itemCount_ = itemCount_+ num; 
}
