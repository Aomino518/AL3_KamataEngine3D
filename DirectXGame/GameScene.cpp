#include "GameScene.h"
#include "Matrix.h"

using namespace KamataEngine;

GameScene::~GameScene() { 
	delete model_; 

	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		delete worldTransformBlock;
	}
	worldTransformBlocks_.clear();
}

// 初期化
void GameScene::Initialize() {
	// 3Dモデルの生成
	model_ = Model::Create();

	// 要素数
	const uint32_t kNumBlockHorizontal = 20;
	// ブロック1個分の横幅
	const float kBlockWidth = 2.0f;
	// 要素数を変更する
	worldTransformBlocks_.resize(kNumBlockHorizontal);

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockHorizontal; ++i) {
		worldTransformBlocks_[i] = new WorldTransform();
		worldTransformBlocks_[i]->Initialize();
		worldTransformBlocks_[i]->translation_.x = kBlockWidth * i;
		worldTransformBlocks_[i]->translation_.y = 0.0f;
	}

	// カメラ
	camera_.Initialize();

	textureHandle_ = TextureManager::Load("cube/cube.jpg");
}

// 更新
void GameScene::Update() {
	// ブロックの更新
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		worldTransformBlock->matWorld_ = KamataEngine::MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);
		// 定数バッファに転送する
		worldTransformBlock->TransferMatrix();
	}
}

// 描画
void GameScene::Draw() { 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());
	for (WorldTransform* worldTransformBlock : worldTransformBlocks_) {
		model_->Draw(*worldTransformBlock, camera_);
	}
	Model::PostDraw();
}

