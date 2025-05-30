#pragma once
#include <KamataEngine.h>
#include "Player.h"


using namespace KamataEngine;

class CameraController {
private:
	Camera* camera_ = nullptr;
	Player* target_ = nullptr;
	// 追従対称とカメラの座標の差 (オフセット)
	Vector3 targetOffset_ = {0, 0, -15.0f};

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	void SetTarget(Player* target) { this->target_ = target; }

	void Reset();
};

