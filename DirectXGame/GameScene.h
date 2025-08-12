#pragma once
#include "KamataEngine.h"	
#include "Player.h"

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

private:
	Player* player_ = nullptr;
	Model* model_ = nullptr;
	WorldTransform worldTransform_;
	Camera camera_;
};
