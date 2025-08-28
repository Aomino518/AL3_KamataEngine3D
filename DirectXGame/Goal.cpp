#include "Goal.h"
#include "Player.h"

void Goal::Init(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position) { 
	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
}

void Goal::Update() { 
	WtfUpdate(worldTransform_); 
}

void Goal::Draw() { 
	model_->Draw(worldTransform_, *camera_); 
}

Vector3 Goal::GetWorldPosition() const {
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

KamataEngine::AABB Goal::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	KamataEngine::AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Goal::OnCollision(const Player* player) {
	(void)player;
	if (player->GetItemCount() == goalItem_) {
		isGoal = true;
	}
}
