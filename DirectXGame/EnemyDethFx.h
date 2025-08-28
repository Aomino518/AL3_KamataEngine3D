#pragma once
#include <KamataEngine.h>

using KamataEngine::Vector3;

class EnemyDethFx {
public:
	void Init(KamataEngine::Model* model, KamataEngine::Camera* camera, const KamataEngine::WorldTransform& worldTransform);

	void Update();

	void Draw();

	bool IsFinished() const { return isFinished_; }

	const KamataEngine::WorldTransform* GetTargetWorldTransform() const { return &worldTransform_; }

private:
	KamataEngine::Model* model_ = nullptr;
	KamataEngine::Camera* camera_ = nullptr;

	KamataEngine::WorldTransform worldTransform_;

	// 終了フラグ
	bool isFinished_ = false;
	// 目標スケール
	float finishScale = 0.3f;
};
