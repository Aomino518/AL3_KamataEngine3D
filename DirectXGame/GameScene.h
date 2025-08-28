#pragma once
#include "KamataEngine.h"
#include <vector>
#include "MapChipField.h"
#include "Matrix.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DethParticles.h"
#include "Fade.h"
#include "EnemyDethFx.h"
#include "Item.h"
#include "Goal.h"
#include "Dome.h"
#include "Ground.h"

class Player;

class GameScene {
public:
	GameScene();
	~GameScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// モデルの作成
	void CreateModels();

	// スプライトの生成
	void CreateSprite();

	// 生成と初期化処理
	void CreateAndInits();

	/// <summary>
	/// 安全な解放処理
	/// </summary>
	/// <typeparam name="T">すべての型</typeparam>
	/// <param name="ptr">解放したいインスタンス</param>
	template<typename T>
	void SafeDelete(T*& ptr) {
		delete ptr;
		ptr = nullptr;
	}

	/// <summary>
	/// 表示ブロックの生成
	/// </summary>
	void GenerateBlocks();

	void CheckAllCollisions();

	void ChangePhase();

	bool IsFinished() const { return finished_; }

	bool IsGoal() const { return goal_->GetGoal(); }

	bool IsGameOver() const { return isGameOver_; }

	bool IsBackTitle() const { return backTitle_; }

private:
	// ゲームのフェーズ (型)
	enum class Phase {
		kFadeIn,
		kPlay,  // ゲームプレイ
		kDeath, // デス演出
		kFadeOut,
		kPause,
	};

	// ポーズ画面のメニュー項目
	enum PHUSE {
		kContinu,
		kBackTitle,
	};

	// ゲーム現在フェーズ
	Phase phase_ = Phase::kFadeIn;

	PHUSE menu_ = kContinu;

	// プレイヤー
	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

	// ブロック
	KamataEngine::Model* modelBlock_ = nullptr;

	// 敵
	std::list<Enemy*> enemies_;
	KamataEngine::Model* modelEnemy_ = nullptr;

	// デスパーティクル
	DethParticles* dethParticle_ = nullptr;
	KamataEngine::Model* modelParticle_ = nullptr;

	// 敵のデスモーション
	std::list<EnemyDethFx*> enemiesDethFx_;
	KamataEngine::Model* modelDethFx_ = nullptr;

	// ゴール
	Goal* goal_ = nullptr;
	KamataEngine::Model* modelGoal_ = nullptr;

	// 鍵
	Item* item_[3]{};
	KamataEngine::Model* modelItem_[3]{};

	// 天球
	Dome* dome_ = nullptr;
	KamataEngine::Model* modelDome_ = nullptr;

	Ground* ground_ = nullptr;
	KamataEngine::Model* modelGround_ = nullptr;

	KamataEngine::Sprite* spriteBack_ = nullptr;

	uint32_t tHAbKey;
	Sprite* spriteAbKey_ = nullptr;

	uint32_t tHJumpSpace;
	Sprite* spriteJumpSpace_ = nullptr;

	uint32_t tHPauseEsc;
	Sprite* spritePauseEsc_ = nullptr;

	uint32_t tHSelectBackTitle;
	Sprite* spriteSelectBackTitle_ = nullptr;

	uint32_t tHSelectCotinue;
	Sprite* spriteSelectCotinue_ = nullptr;

	uint32_t tHSelectSpace;
	Sprite* spriteSelectSpace_ = nullptr;

	uint32_t tHWsKey;
	Sprite* spriteWsKey_ = nullptr;

	uint32_t sHPlayScene;
	uint32_t vHPlayScene;

	uint32_t sHSelect;
	uint32_t vHSelect;

	KamataEngine::WorldTransform worldTransform_{};
	KamataEngine::Camera camera_{};

	// ワールドトランスフォームブロック
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// マップチップフィールド
	MapChipField* mapChipField_ = nullptr;
	
	// 追従カメラ
	CameraController* cameraController_ = nullptr;

	bool finished_ = false;

	Fade* fade_ = nullptr;

	bool isGameOver_ = false;

	bool backTitle_ = false;

#ifdef _DEBUG
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
#endif _DEBUG
};
