#pragma once
#include <KamataEngine.h>
#include "Fade.h"
#include "Dome.h"

class GameOverScene {
public:
	~GameOverScene();

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

	// 天球
	Dome* dome_ = nullptr;
	KamataEngine::Model* modelDome_ = nullptr;

	Fade* fade_ = nullptr;

	bool finished = false;

	uint32_t tHBackTitle;
	Sprite* spriteBackTitle_ = nullptr;

	uint32_t tHGameOver;
	Sprite* spriteGameOver_ = nullptr;

	uint32_t sHGameOver;
	uint32_t vHGameOver;

	uint32_t sHSelect;
	uint32_t vHSelect;
};
