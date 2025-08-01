#pragma once
#include <KamataEngine.h>
#include "Matrix.h"

// 前方宣言
class MapChipField;
class Enemy;
using namespace KamataEngine;

class Player {
public:
	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	enum Corner {
		kRightBottom,
		kLeftBottom,
		kRightTop,
		kLeftTop,
		kNumCorner
	};

	// 振る舞い
	enum class Behavior { 
		kRoot, // 通常状態
		kAttack, // 攻撃中
		kUnknown,
	};

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="camera">カメラ</param>
	void Initialize(Model* model, Camera* camera, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	WorldTransform& GetWorldTransform() { return worldTransform_; }

	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	void InputMove();

	Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();

	// 衝突応答
	void OnCollision(const Enemy* enemy);

	// デスフラグのgetter
	bool IsDead() const { return isDead_; }

	// 通常行動更新
	void BehaviorRootUpdate();

	// 攻撃行動更新
	void BehaviorAttackUpdate();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;
	// playerの速度
	Vector3 velocity_ = {};
	static inline const float kAcceleration = 0.1f;
	static inline const float kAttenuation = 0.05f;
	static inline const float kLimitRunSpeed = 0.3f;

	LRDirection lrDirection_ = LRDirection::kRight;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;
	// 接地状態のフラグ
	bool onGround_ = true;
	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.98f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.5f;
	// ジャンプ初速(上方向)
	static inline const float kJumpAcceleration = 20.0f;
	//　マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;
	static inline const float kBlank = 0.04f;

	// マップとの当たり判定情報
	struct CollisionMapInfo {
		bool ceiling = false; // 天井衝突フラグ
		bool landing = false; // 着地フラグ
		bool hitWall = false; // 壁接触フラグ
		Vector3 move; // 移動量
	};

	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);
	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);
	// 接地状態の切り替え処理
	void UpdateOnGround(const CollisionMapInfo& info);
	void UpdateOnWall(const CollisionMapInfo& info);

    static inline const float kAttenuationLanding = 0.1f;
    static inline const float kGroundSearchHeight = 0.1f;
	static inline const float kAttenuationWall = 0.1f;

	// デスフラグ
	bool isDead_ = false;

	// 振る舞い
	Behavior behavior_ = Behavior::kRoot;

	// 次の振る舞いをリクエスト
	Behavior behaviorRequest_ = Behavior::kUnknown;
};
