#include "Item.h"
#include "Player.h"

void Item::Init(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position, ItemStatus itemStatus) { 
	model_ = model;
	camera_ = camera;
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	status_ = itemStatus;

	sHItemGet = KamataEngine::Audio::GetInstance()->LoadWave("./Resources/sounds/se_itemget_001.wav");
}

void Item::Update() {
	switch (status_) { 
	case ItemStatus::Key: 

		// 行列の更新
		WtfUpdate(worldTransform_);
		break;
	}
}

void Item::Draw() { 
	if (isGet) {
		return;
	}
		
	model_->Draw(worldTransform_, *camera_);
}

Vector3 Item::GetWorldPosition() const {
	Vector3 worldPos;

	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

KamataEngine::AABB Item::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	KamataEngine::AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}


void Item::OnCollision(Player* player) { 
	(void)player;
	vHItemGet = KamataEngine::Audio::GetInstance()->PlayWave(sHItemGet, false, 0.5f);
	player->SetAddItemCount(1);
	isGet = true;
}


