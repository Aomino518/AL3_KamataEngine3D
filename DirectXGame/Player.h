#pragma once
#include "Matrix.h"
#include <KamataEngine.h>
#include <cassert>

class MapChipField;
class Enemy;
using KamataEngine::Vector3;

// 左右
enum class LRDirection {
	kRight,
	kLeft,
};

class Player {
public:
	/// <summary>
	/// プレイヤーの初期化
	/// </summary>
	/// <param name="model">描画に使うモデル</param>
	/// <param name="camera">ワールド→ビュー変換に使うカメラ</param>
	void Init(KamataEngine::Model* model, KamataEngine::Camera* camera, const Vector3& position);

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 移動処理
	/// </summary>
	void InputMove();

	void SetMapField(MapChipField* field) { mapChipField_ = field; }

	bool GetOnGround() const { return onGround_; }

	bool GetLanding() const { return collisionMapInfo.landing; }

	bool GetHitWall() const { return collisionMapInfo.hitWall; }

	bool GetCeiling() const { return collisionMapInfo.ceiling; }

	KamataEngine::WorldTransform& GetWorldTransform() { return worldTransform_; }

	const Vector3& GetVelocity() const { return velocity_; }

	KamataEngine::WorldTransform& Transform() { return worldTransform_; }
	const KamataEngine::WorldTransform& Transform() const { return worldTransform_; }

	Vector3 GetWorldPosition();
	
	// AABBを取得
	KamataEngine::AABB GetAABB();

	// 衝突応答
	void OnCollision(const Enemy* enemy);

	// デスフラグのgetter
	bool IsDead() const { return isDead_; }

	// 踏みつけ攻撃のフィードバッグ
	void StompBounce(float velocity) { 
		onGround_ = false;
		velocity_.y = velocity;
	}

	int SetAddItemCount(int num);

	int GetItemCount() const { return itemCount_; }

private:
	// ワールド変換（位置・回転・拡大と最終行列）
	KamataEngine::WorldTransform worldTransform_{};
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	// プレイヤーの移動速度
	Vector3 velocity_ = {};
	// 速度増加
	static inline const float kAcceleration = 0.2f;
	// 速度減衰
	static inline const float kAttenuation = 0.05f;
	// 最大移動速度
	static inline const float kLimitRunSpeed = 0.2f;

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	// 接地フラグ
	bool onGround_ = true;
	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.98f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.5f;
	// ジャンプ方向(上方向)
	static inline const float kJumpAcceleration = 10.0f;

	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;
	static inline const float kBlank = 0.001f;

	// マップの当たり判定情報
	struct CollisionMapInfo {
		bool ceiling = false; // 天井衝突フラグ
		bool landing = false; // 着地フラグ
		bool hitWall = false; // 壁接触フラグ
		Vector3 move;         // 移動量
	};

	CollisionMapInfo collisionMapInfo = {};

	void CheckMapCollosion(CollisionMapInfo& info);

	// デスフラグ
	bool isDead_ = false;

	int itemCount_ = 0;

	uint32_t sHSeJump;
	uint32_t vHSeJump;
};