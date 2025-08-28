#pragma once
#include <KamataEngine.h>

class Player;
using namespace KamataEngine;

class CameraController {
public:
	struct Rect {
		float left = 0.0f;   // 左端
		float right = 1.0f;  // 右端
		float bottom = 0.0f; // 下端
		float top = 0.0f;    // 上端
	};

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void SetTarget(Player* target) { this->target_ = target; }

	void SetMovableArea(Rect area) { this->movableArea_ = area; }

	void Reset();

private:
	Camera* camera_ = nullptr;
	Player* target_ = nullptr;
	// 追従対称とカメラの座標の差 (オフセット)
	Vector3 targetOffset_ = {0, 0, -25.0f};
	// カメラ移動範囲
	Rect movableArea_ = {0.0f, 300.0f, 0.0f, 600.0f};
	// カメラの目標座標
	Vector3 goalPoint_ = {0.0f, 0.0f, 0.0f};
	// 座標補間割合
	static inline const float kInterpolationRateX = 0.1f;
	static inline const float kInterpolationRateY = 0.1f;
	// 速度掛け率
	static inline const float kVelocityBiasX = 20.0f;

	// Y軸上昇掛け軸
	float kVelocityBiasYUp = 0.0f;
	// Y軸下降掛け軸
	float kVelocityBiasYDown = 1.0f;

	// 追従対称の各方向へのカメラ移動範囲
	static inline const Rect targetMargin = {-9.0f, 9.0f, -9.0f, 9.0f};
};
