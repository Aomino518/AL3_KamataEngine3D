#pragma once
#include "Matrix.h"
#include <KamataEngine.h>
#include <cassert>

using KamataEngine::Vector3;
class Player;

class Goal {
public:
	// 初期化
	void Init(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position);
	
	// 更新
	void Update();

	// 描画
	void Draw();

	// ワールド座標取得
	Vector3 GetWorldPosition() const;

	// AABB取得
	KamataEngine::AABB GetAABB();

	// 衝突関数
	void OnCollision(const Player* player);

	bool GetGoal() const { return isGoal; }

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;
	KamataEngine::WorldTransform worldTransform_;

	bool isGoal = false;
	int goalItem_ = 3;

	static inline const float kWidth = 1.5f;
	static inline const float kHeight = 1.5f;
};
