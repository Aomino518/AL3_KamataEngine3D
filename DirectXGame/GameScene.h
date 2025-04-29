#pragma once
#include "KamataEngine.h"
#include <vector>

class GameScene {
public:
	~GameScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

private:
	// 3Dモデルデータ
	KamataEngine::Model* model_ = nullptr;

	// ワールドトランスフォーム
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;
	// カメラ
	KamataEngine::Camera camera_;

	uint32_t textureHandle_ = 0;

#ifdef _DEBUG
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
#endif _DEBUG

};
