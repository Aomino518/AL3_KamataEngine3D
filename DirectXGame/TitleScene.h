#pragma once
#include <KamataEngine.h>
#include "Fade.h"

class TitleScene {
public:
	// シーンのフェーズ
	enum class Phase {
		kFadeIn,  // フェードイン
		kMain,    // メイン部
		kFadeOut, // フェードアウト
	};

	~TitleScene();

	void Init();

	void Update();

	void Draw();

	bool IsFinished() const { return finished_; }

private:
	bool finished_ = false;

	Fade* fade_ = nullptr;

	static inline const float kTimeTitleMove = 2.0f;

	// ビュープロジェクション
	Camera camera_;
	KamataEngine::WorldTransform worldTransformTitle_;
	KamataEngine::WorldTransform worldTransformPlayer_;
	KamataEngine::WorldTransform worldTransformDome_;

	KamataEngine::Model* modelPlayer_ = nullptr;
	KamataEngine::Model* modelDome_ = nullptr;

	float counter_ = 0.0f;

	// 現在のフェーズ
	Phase phase_ = Phase::kFadeIn;

	// タイトルネーム
	uint32_t tHTitleName;
	Sprite* spriteTitleName_ = nullptr;

	// 文字UI
	uint32_t tHStartUi;
	Sprite* spriteTitleStartUi_ = nullptr;

	// 文字UI
	uint32_t tHMaou;
	Sprite* spriteMaou_ = nullptr;

	uint32_t sHTitleBgm;
	uint32_t vHTitleBgm;

	uint32_t sHSelect;
	uint32_t vHSelect;
};
