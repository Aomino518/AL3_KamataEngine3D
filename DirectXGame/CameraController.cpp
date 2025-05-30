#include <algorithm>
#include "CameraController.h"
#include "Player.h"
#include "Matrix.h"

void CameraController::Initialize(Camera* camera) { 
	camera_ = camera;
}

void CameraController::Update() {
	// 追従対称のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	const Vector3& targetVelocity = target_->GetVelocity();

	// 追従対称とオフセットからカメラの座標を計算
	goalPoint_ = targetWorldTransform.translation_ + targetOffset_ + targetVelocity * kVelocityBias;
	
	// 座標補間によりゆったり追従
	camera_->translation_ = Lerp(camera_->translation_, goalPoint_, kInterpolationRate);

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
