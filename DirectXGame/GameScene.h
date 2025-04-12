#pragma once
#include "KamataEngine.h"	
#include "3d/DebugCamera.h"

class GameScene {
public:
	// デストラクタ
	~GameScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// テクスチャハンドル
	uint32_t textureHandle_ = 0;

	// サウンドデータハンドル
	//uint32_t soundHandle_ = 0;

	// 音声再生ハンドル
	//uint32_t voiceHandle_ = 0;

	//KamataEngine::Sprite* sprite_ = nullptr;

	// 3Dモデル
	KamataEngine::Model* model_ = nullptr;

	// ワールドトランスフォーム
	KamataEngine::WorldTransform worldTransform_;
	// カメラ
	KamataEngine::Camera camera_;
	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;

	// ImGuiで値を入力する変数
	float inputFloat3[3] = {0, 0, 0};
};
