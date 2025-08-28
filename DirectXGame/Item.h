#pragma once
#include <KamataEngine.h>
#include <cstdint>
#include "Matrix.h"

using KamataEngine::Vector3;

class Player;

class Item {
public:
	// アイテムの種類
	enum class ItemStatus {
		Key,
		HealItem,
	};

	void Init(KamataEngine::Model* model, 
		KamataEngine::Camera* camera, 
		const Vector3& position, 
		ItemStatus itemStatus);

	void Update();

	void Draw();

	Vector3 GetWorldPosition() const;

	KamataEngine::AABB GetAABB();

	void OnCollision(Player* player);

	bool GetterIsGet() const { return isGet; }

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;

	ItemStatus status_;
	bool isGet = false;

	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;

	uint32_t sHItemGet;
	uint32_t vHItemGet;
};
