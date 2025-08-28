#include "CameraController.h"
#include <algorithm>
#include "Player.h"
#include "Matrix.h"

void CameraController::Initialize(Camera* camera) { camera_ = camera; }

void CameraController::Update() {
	// 追従対称のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	const Vector3& targetVelocity = target_->GetVelocity();

	Vector3 target = targetWorldTransform.translation_ + targetOffset_;
	Vector3 lookAhead = {targetVelocity.x * kVelocityBiasX, 
		(targetVelocity.y > 0.0f ? targetVelocity.y * kVelocityBiasYUp : targetVelocity.y * kVelocityBiasYDown), 
		0.0f};

	goalPoint_ = target + lookAhead;

	camera_->translation_.x += (goalPoint_.x - camera_->translation_.x) * kInterpolationRateX;
	camera_->translation_.y += (goalPoint_.y - camera_->translation_.y) * kInterpolationRateY;

	// 移動範囲制限
	camera_->translation_.x = max(camera_->translation_.x, goalPoint_.x + targetMargin.left);
	camera_->translation_.x = min(camera_->translation_.x, goalPoint_.x + targetMargin.right);
	camera_->translation_.y = max(camera_->translation_.y, goalPoint_.y + targetMargin.bottom);
	camera_->translation_.y = min(camera_->translation_.y, goalPoint_.y + targetMargin.top);

	camera_->translation_.x = max(camera_->translation_.x, movableArea_.left);
	camera_->translation_.x = min(camera_->translation_.x, movableArea_.right);
	camera_->translation_.y = max(camera_->translation_.y, movableArea_.bottom);
	camera_->translation_.y = min(camera_->translation_.y, movableArea_.top);

	// 行列を更新する
	camera_->UpdateMatrix();
}

void CameraController::Reset() {
	// 追従対称のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	// 追従対称とオフセットからカメラの座標を計算
	camera_->translation_ = targetWorldTransform.translation_ + targetOffset_;
}