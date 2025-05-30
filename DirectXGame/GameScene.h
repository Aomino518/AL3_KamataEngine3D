#pragma once
#include "KamataEngine.h"
#include <vector>
#include "3d/DebugCamera.h"
#include "Skydome.h"
#include "Player.h"
#include "MapChipField.h"
#include "CameraController.h"

namespace KamataEngine {
class GameScene {
public:
	~GameScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	void GenerateBlocks();

private:
	// 3Dモデルデータ
	Model* model_ = nullptr;

	// ワールドトランスフォーム
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	// カメラ
	Camera camera_;

	// 天球
	Model* modelSkydome_ = nullptr;

	// プレイヤー
	Model* modelPlayer_ = nullptr;

	Skydome* skydome_ = nullptr;

	Player* player_ = nullptr;

	// マップチップフィールド
	MapChipField* mapChipField_;

	CameraController* cameraController_;

#ifdef _DEBUG
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
#endif _DEBUG
};
} // namespace KamataEngine
