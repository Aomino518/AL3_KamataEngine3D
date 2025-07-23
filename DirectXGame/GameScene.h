#pragma once
#include "KamataEngine.h"
#include <vector>
#include "3d/DebugCamera.h"
#include "Skydome.h"
#include "Player.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "Enemy.h"
#include "Matrix.h"
#include "DethParticles.h"

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

	// 全ての当たり判定を行う
	void CheckAllCollisions();

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

	Model* modelEnemy_ = nullptr;

	Model* dethParticles_model_ = nullptr;

	Skydome* skydome_ = nullptr;

	Player* player_ = nullptr;

	//Enemy* enemy_ = nullptr;

	std::list<Enemy*> enemies_;

	// マップチップフィールド
	MapChipField* mapChipField_;

	CameraController* cameraController_ = nullptr;

	DethParticles* dethParticles_ = nullptr;

#ifdef _DEBUG
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
#endif _DEBUG
};
} // namespace KamataEngine
