#include "GameScene.h"

using namespace KamataEngine;

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete player_; 
	delete model_;
}

// 初期化
void GameScene::Initialize() {
	model_ = Model::CreateFromOBJ("player");
	worldTransform_.Initialize();
	camera_.Initialize();
	// 自キャラの生成
	player_ = new Player();
	player_->Init(model_, &camera_);

}

// 更新
void GameScene::Update() { 
	player_->Update(); 
}

// 描画
void GameScene::Draw() { 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());
	player_->Draw(); 
	Model::PostDraw();
}
