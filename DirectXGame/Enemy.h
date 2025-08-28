#pragma once
#include <KamataEngine.h>
#include "Matrix.h"

using KamataEngine::Vector3;

class Player;

class Enemy {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="camera">カメラ</param>
	/// <param name="position">位置</param>
	void Init(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3 position);

	// 更新
	void Update();

	// 描画
	void Draw();

	Vector3 GetWorldPosition() const;

	Vector3 GetScale() const { return worldTransform_.scale_; }

	const KamataEngine::WorldTransform& GetWorldTransform() const { return worldTransform_; }

	KamataEngine::AABB GetAABB();

	// 衝突応答
	void OnCollision(const Player* player);

	bool IsDead() const { return isDead_; }

	void Kill() { isDead_ = true; }

	bool HasSpawnedDeathFx() const { return spawnedDeathFx_; }

	void MarkSpawnedDeathFx() { spawnedDeathFx_ = true; }

private:
	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_{};
	// モデル
	KamataEngine::Model* model_ = nullptr;
	// カメラ
	KamataEngine::Camera* camera_ = nullptr;

	// 移動スピード
	float velocity_ = 0.05f;
	// 初期位置からの移動距離	
	float move_ = 0.0f;

	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;

	bool isDead_ = false;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	LRDirection lrDirection_ = LRDirection::kLeft;

	bool spawnedDeathFx_ = false;

	static inline const float kWalkMotionAngleStart = 0.0f;

	static inline const float kWalkMotionAngleEnd = 30.0f;

	static inline const float kWalkMotionTime = 1.0f;

	float walkTimer = 0.0f;

	uint32_t sHStomp;
	uint32_t vHStomp;
};
