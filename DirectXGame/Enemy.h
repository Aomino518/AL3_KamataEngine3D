#pragma once
#include <KamataEngine.h>
#include "Matrix.h"

using namespace KamataEngine;

class Player;

/// <summary>
/// 敵
/// </summary>
class Enemy {
public:
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	void Update();

	void Draw();

	Vector3 GetWorldPosition();

	AABB GetAABB();

	// 衝突応答
	void OnCollision(const Player* player);

private:
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;

	static inline const float kWalkSpeed = 0.02f;

	Vector3 velocity_ = {};

	static inline const float kWalkMotionAngleStart = 0.0f;

	static inline const float kWalkMotionAngleEnd = 30.0f;

	static inline const float kWalkMotionTime = 1.0f;

	float walkTimer = 0.0f;

	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
};
