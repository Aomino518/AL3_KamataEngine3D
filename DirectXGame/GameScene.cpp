#include "GameScene.h"
#include "Player.h"
#include <cassert>

using namespace KamataEngine;

GameScene::GameScene() = default;

GameScene::~GameScene() { 
	SafeDelete(player_);
	SafeDelete(modelPlayer_);
	SafeDelete(mapChipField_);
	SafeDelete(modelBlock_);

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();


#ifdef _DEBUG
	SafeDelete(debugCamera_);
#endif _DEBUG
}

// 初期化
void GameScene::Initialize() {
	// カメラの初期化
	camera_.Initialize();

	// モデルの作成
	CreateModels();

	// 生成と初期化処理
	CreateAndInits();

#ifdef _DEBUG
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
#endif _DEBUG
}

// 更新
void GameScene::Update() { 
	if (!player_) {
		return;
	}

	// プレイヤー更新
	player_->Update(); 

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_TAB)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		camera_.matView = debugCamera_->GetCamera().matView;
		camera_.matProjection = debugCamera_->GetCamera().matProjection;
		// ビュープロジェクション行列の転送
		camera_.TransferMatrix();
	} 
	else {
		// ビュープロジェクション行列の更新と転送
		camera_.UpdateMatrix();
	}

#endif _DEBUG

	// ブロックの更新
	for (const std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			// 定数バッファに転送する
			WtfUpdate(*worldTransformBlock);
		}
	}
}

// 描画
void GameScene::Draw() { 
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());
	// ブロック
	for (const std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, camera_);
		}
	}

	// プレイヤー
	if (player_) {
		player_->Draw();
	}

	Model::PostDraw();

}

// モデルの作成
void GameScene::CreateModels() {
	// プレイヤーモデルの作成
	modelPlayer_ = Model::CreateFromOBJ("player");
	assert(modelPlayer_ && "Model::CreateFromOBJ(\"player\") failed.");

	// ブロックモデルの作成
	modelBlock_ = Model::CreateFromOBJ("block", true);
	assert(modelBlock_ && "Model::CreateFromOBJ(\"block\") failed.");
}

// 生成と初期化
void GameScene::CreateAndInits() {
	// マップチップフィールドの生成と初期化
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/testBlocks.csv");
	GenerateBlocks();

	// ワールド行列初期化
	worldTransform_.Initialize();

	// 自キャラの生成と初期化
	player_ = new Player();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 14);
	player_->Init(modelPlayer_, &camera_, playerPosition);

}

// 表示ブロックの生成
void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 列数を設定 (縦方向のブロック数)
	worldTransformBlocks_.resize(kNumBlockVirtical);
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		// 1列の要素数を設定 (縦方向のブロック数)
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	// キューブの生成
	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}
