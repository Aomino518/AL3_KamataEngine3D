#pragma once
#include <KamataEngine.h>
#include "Fade.h"
#include "Dome.h"

class ClearScene {
public:
	~ClearScene();

	void Init();

	void Update();

	void Draw();

	bool IsFinished() const { return finished; }

private:
	enum class Phase {
		kFadeIn,
		kGameOver,
		kFadeOut,
	};

	Phase phase_ = Phase::kFadeIn;

	KamataEngine::WorldTransform worldTransform_{};
	KamataEngine::Camera camera_{};

	Fade* fade_ = nullptr;

	bool finished = false;

	// 天球
	Dome* dome_ = nullptr;
	KamataEngine::Model* modelDome_ = nullptr;

	uint32_t tHBackTitle;
	Sprite* spriteBackTitleSpace_ = nullptr;

	uint32_t tHGameClear;
	Sprite* spriteGameClear_ = nullptr;

	uint32_t sHGameClear;
	uint32_t vHGameHandle;

	uint32_t sHSelect;
	uint32_t vHSelect;
};
