#include "GameScene.h"
#include "Matrix.h"
#include "WTFUpdate.h"

namespace KamataEngine {
GameScene::~GameScene() {
	delete model_;
	delete modelSkydome_;
	delete modelPlayer_;
	delete skydome_;
	delete player_;
	delete mapChipField_;
	delete cameraController_;
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete dethParticles_;
	delete dethParticles_model_;

#ifdef DEBUG_
	delete debugCamera_;
#endif DEBUG_
}

// 初期化
void GameScene::Initialize() {
	// 3Dモデルの生成
	model_ = Model::CreateFromOBJ("block", true);

	// 天球の生成
	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球の初期化
	skydome_->Initialize(modelSkydome_, &camera_);

	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();

	// プレイヤー生成
	player_ = new Player();
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 18);
	player_->SetMapChipField(mapChipField_);
	// プレイヤーの初期化
	player_->Initialize(modelPlayer_, &camera_, playerPosition);

	// カメラ
	camera_.Initialize();

	cameraController_ = new CameraController();
	cameraController_->Initialize(&camera_);
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};
	cameraController_->SetMovableArea(cameraArea);

	// enemy_ = new Enemy();
	modelEnemy_ = Model::CreateFromOBJ("enemy", true);
	// Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14, 18);
	// enemy_->Initialize(modelEnemy_, &camera_, enemyPosition);

	for (int32_t i = 0; i < 2; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(14 + i * 2, 18);
		newEnemy->Initialize(modelEnemy_, &camera_, enemyPosition);
		enemies_.push_back(newEnemy);
	}

	dethParticles_model_ = Model::CreateFromOBJ("deathParticle");

	// 仮の生成処理
	// dethParticles_ = new DethParticles;
	// dethParticles_->Initialize(dethParticles_model_, &camera_, playerPosition);

	// ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

#ifdef _DEBUG
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
#endif _DEBUG
}

void GameScene::Update() {
	ChangePhase();

	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kPlay;
		}

		skydome_->Update();
		cameraController_->Update();
		player_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

#ifdef _DEBUG
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
#endif
		// カメラの処理
		if (isDebugCameraActive_) {
			// デバッグカメラの更新
			debugCamera_->Update();
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロジェクション行列の転送
			camera_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			camera_.UpdateMatrix();
		}

		// ブロックの更新
		for (const std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				// 定数バッファに転送する
				WtfUpdate(*worldTransformBlock);
			}
		}

		break;
	case Phase::kPlay:
		skydome_->Update();
		cameraController_->Update();

		player_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		#ifdef _DEBUG
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			isDebugCameraActive_ = !isDebugCameraActive_;
		}
#endif

		// カメラの処理
		if (isDebugCameraActive_) {
			// デバッグカメラの更新
			debugCamera_->Update();
			camera_.matView = debugCamera_->GetCamera().matView;
			camera_.matProjection = debugCamera_->GetCamera().matProjection;
			// ビュープロジェクション行列の転送
			camera_.TransferMatrix();
		} else {
			// ビュープロジェクション行列の更新と転送
			camera_.UpdateMatrix();
		}

		// ブロックの更新
		for (const std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				// 定数バッファに転送する
				WtfUpdate(*worldTransformBlock);
			}
		}

		CheckAllCollisions();

		if (player_->IsDead()) {
			// 死亡演出フェーズに切り替え
			phase_ = Phase::kDeath;
			// 自キャラの座標を取得
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();
			dethParticles_ = new DethParticles;
			dethParticles_->Initialize(dethParticles_model_, &camera_, deathParticlesPosition);
		}

		break;
	case Phase::kDeath:
		if (dethParticles_ && dethParticles_->GetIsFinished()) {
			finished_ = true;
		}

		skydome_->Update();
		cameraController_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		if (dethParticles_) {
			dethParticles_->Update();
		}

		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished_ = true;
		}

		skydome_->Update();
		cameraController_->Update();

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		break;
	}
}

// 描画
void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	Model::PreDraw(dxCommon->GetCommandList());
	for (const std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			model_->Draw(*worldTransformBlock, camera_);
		}
	}

	if (!player_->IsDead()) {
		player_->Draw();
	}

	skydome_->Draw();
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	if (dethParticles_) {
		dethParticles_->Draw();
	}

	Model::PostDraw();
}

void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// worldTransformBlocks_.resize(kNumBlockHorizontal);

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

void GameScene::CheckAllCollisions() {
	AABB aabb1, aabb2;

#pragma region
	// 自キャラの座標
	aabb1 = player_->GetAABB();

	// 自キャラと敵弾全ての当たり判定
	for (Enemy* enemy : enemies_) {
		// 敵弾の座標
		aabb2 = enemy->GetAABB();

		// AABB同士の交差判定
		if (IsCollision(aabb1, aabb2)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision(enemy);
			// 敵の衝突時関数を呼び出す
			enemy->OnCollision(player_);
		}
	}
#pragma endregion
}

void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		if (player_->IsDead()) {
			// 死亡演出
			phase_ = Phase::kDeath;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			dethParticles_ = new DethParticles;
			dethParticles_->Initialize(dethParticles_model_, &camera_, deathParticlesPosition);
		}
		break;
	case Phase::kDeath:
		break;
	}
}

} // namespace KamataEngine
