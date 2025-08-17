#pragma once
#include <KamataEngine.h>
#include <cassert>
#include "Matrix.h"

using KamataEngine::Vector3;

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

	void InputKey();

	KamataEngine::WorldTransform& Transform() { return worldTransform_; }
	const KamataEngine::WorldTransform& Transform() const { return worldTransform_; }

private:
	// ワールド変換（位置・回転・拡大と最終行列）
	KamataEngine::WorldTransform worldTransform_{};
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	// プレイヤーの移動速度
	Vector3 velocity_ = {};
	// 速度増加
	static inline const float kAcceleration = 0.1f;
	// 速度減衰
	static inline const float kAttenuation = 0.2f;
	// 最大移動速度
	static inline const float kLimitRunSpeed = 0.2f;

	// 設置フラグ
	bool onGround_ = true;
	// 重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.98f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 0.5f;
	// ジャンプ方向(上方向)
	static inline const float kJumpAcceleration = 10.0f;
	//int jumpBoostFrames_ = 0;
	//int kJumpBoostMaxFrames_ = 60;
};