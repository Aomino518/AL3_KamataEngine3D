#include "GameScene.h"

using namespace KamataEngine;

// デストラクタ
GameScene::~GameScene() { 
	delete model_; 
	delete player_;
}

// 初期化
void GameScene::Initialize() { 
	textureHandle_ = TextureManager::Load("uvChecker.png"); 

	// 3Dモデルの生成
	model_ = Model::Create();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_, &camera_);
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
