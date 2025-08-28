#include "GameScene.h"
#include "Player.h"
#include <cassert>
#include <imgui.h>

using namespace KamataEngine;

GameScene::GameScene() = default;

GameScene::~GameScene() {
	SafeDelete(player_);
	SafeDelete(modelPlayer_);
	SafeDelete(mapChipField_);
	SafeDelete(modelBlock_);
	SafeDelete(cameraController_);
	SafeDelete(modelEnemy_);
	SafeDelete(dethParticle_);
	SafeDelete(modelParticle_);
	SafeDelete(fade_);
	SafeDelete(modelDethFx_);
	SafeDelete(spriteBack_);
	SafeDelete(goal_);
	SafeDelete(modelGoal_);
	SafeDelete(dome_);
	SafeDelete(modelDome_);
	SafeDelete(ground_);
	SafeDelete(modelGround_);
	SafeDelete(spriteAbKey_);
	SafeDelete(spriteJumpSpace_);
	SafeDelete(spritePauseEsc_);
	SafeDelete(spriteSelectBackTitle_);
	SafeDelete(spriteSelectCotinue_);
	SafeDelete(spriteSelectSpace_);
	SafeDelete(spriteWsKey_);

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}

	for (EnemyDethFx* dethFx : enemiesDethFx_) {
		delete dethFx;
	}

	for (int i = 0; i < 3; i++) {
		SafeDelete(item_[i]);
		SafeDelete(modelItem_[i]);
	}

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

	// モデルの作成
	CreateModels();

	// スプライトの生成
	CreateSprite();

	// カメラの初期化
	camera_.Initialize();

	// 生成と初期化処理
	CreateAndInits();

	sHPlayScene = Audio::GetInstance()->LoadWave("./Resources/sounds/bgm_PlayScene.mp3");
	vHPlayScene = Audio::GetInstance()->PlayWave(sHPlayScene, true, 0.5f);

	sHSelect = Audio::GetInstance()->LoadWave("./Resources/sounds/se_Select.mp3");

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);

#ifdef _DEBUG
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
#endif _DEBUG
}

// 更新
void GameScene::Update() {
	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kPlay;
		}
		// プレイヤー更新
		player_->Update();
		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		for (int i = 0; i < 3; i++) {
			// アイテムの更新
			item_[i]->Update();
		}

		// ゴールの更新
		goal_->Update();
		// カメラコントローラーの更新
		cameraController_->Update();

		dome_->Update();
		ground_->Update();

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
		if (!player_) {
			return;
		}

		// プレイヤー更新
		player_->Update();
		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
		// ゴールの更新
		goal_->Update();
		// カメラコントローラーの更新
		cameraController_->Update();
		dome_->Update();
		ground_->Update();

		ChangePhase();

		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
			phase_ = Phase::kPause;
		}

		for (Enemy* enemy : enemies_) {
			if (!enemy->IsDead()) {
				continue;
			}

			if (enemy->HasSpawnedDeathFx()) {
				continue; 
			}

			EnemyDethFx* newFx = new EnemyDethFx();
			newFx->Init(modelDethFx_, &camera_, enemy->GetWorldTransform());
			enemiesDethFx_.push_back(newFx);
			enemy->MarkSpawnedDeathFx();    
		}

		for (EnemyDethFx* dethFx : enemiesDethFx_) {
			dethFx->Update();
		}

		for (int i = 0; i < 3; i++) {
			// アイテムの更新
			item_[i]->Update();
		}

		if (goal_->GetGoal()) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}

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
		} else {
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

		CheckAllCollisions();

		break;
	case Phase::kDeath:
		isGameOver_ = true;

		// 敵の更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}
		// カメラコントローラーの更新
		cameraController_->Update();

		// パーティクルの更新
		if (dethParticle_) {
			dethParticle_->Update();
		}

		if (dethParticle_ && dethParticle_->GetIsFinished()) {
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		dome_->Update();
		ground_->Update();

		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			Audio::GetInstance()->StopWave(vHPlayScene);
			finished_ = true;
		}
		dome_->Update();
		ground_->Update();
		cameraController_->Update();

		break;
	case Phase::kPause:
		// ゲームを続ける
		if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
			phase_ = Phase::kPlay;
		}

		if (menu_ == kContinu) {
			if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
				vHSelect = Audio::GetInstance()->PlayWave(sHSelect, false, 0.5f);
				phase_ = Phase::kPlay;
			}

			if (Input::GetInstance()->TriggerKey(DIK_S)) {
				menu_ = kBackTitle;
			}
		}

		// タイトルへ戻る
		if (menu_ == kBackTitle) {
			if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
				vHSelect = Audio::GetInstance()->PlayWave(sHSelect, false, 0.5f);
				backTitle_ = true;
				fade_->Start(Fade::Status::FadeOut, 1.0f);
				phase_ = Phase::kFadeOut;
			}

			if (Input::GetInstance()->TriggerKey(DIK_W)) {
				menu_ = kContinu;
			}
		}
		break;
	}
}

// 描画
void GameScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

#ifdef _DEBUG
	// ImGuiManagerインスタンスの取得
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	// デバッグテキスト
	imguiManager->Begin();
	ImGui::Begin("Debug Window");
	ImGui::Text("onGround_ = %d", player_->GetOnGround());
	ImGui::Text("landing = %d", player_->GetLanding());
	ImGui::Text("hitWall = %d", player_->GetHitWall());
	ImGui::Text("ceiling = %d", player_->GetCeiling());
	ImGui::Text("x = %f", player_->Transform().translation_.x);
	ImGui::Text("y = %f", player_->Transform().translation_.y);
	ImGui::Text("itemCount_ = %d", player_->GetItemCount());
	ImGui::Text("isGoal_ = %d", goal_->GetGoal());
	ImGui::End();
	imguiManager->End();
#endif _DEBUG

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
	if (player_ && !player_->IsDead()) {
		player_->Draw();
	}

	// 敵
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	if (dethParticle_) {
		dethParticle_->Draw();
	}

	for (EnemyDethFx* dethFx : enemiesDethFx_) {
		dethFx->Draw();
	}

	for (int i = 0; i < 3; i++) {
		// アイテムの更新
		item_[i]->Draw();
	}

	if (goal_ && player_->GetItemCount() == 3) {
		goal_->Draw();
	}

	if (dome_) {
		dome_->Draw();
	}

	if (ground_) {
		ground_->Draw();
	}

	Model::PostDraw();



	Sprite::PreDraw(dxCommon->GetCommandList());

	switch (phase_) { 
	case Phase::kFadeIn:
		spriteAbKey_->Draw();
		spriteJumpSpace_->Draw();
		spritePauseEsc_->Draw();

		break;
	case Phase::kPlay:
		spriteAbKey_->Draw();
		spriteJumpSpace_->Draw();
		spritePauseEsc_->Draw();

		break;
	case Phase::kDeath:
		spriteAbKey_->Draw();
		spriteJumpSpace_->Draw();
		spritePauseEsc_->Draw();

		break;
	case Phase::kFadeOut:
		spriteAbKey_->Draw();
		spriteJumpSpace_->Draw();
		spritePauseEsc_->Draw();

		break;
	case Phase::kPause:
		spriteBack_->Draw();
		spriteSelectSpace_->Draw();
		spriteWsKey_->Draw();

		if (menu_ == kContinu) {
			spriteSelectCotinue_->Draw();
		} else {
			spriteSelectBackTitle_->Draw();
		}

		break;
	}
	fade_->Draw();

#ifdef _DEBUG 
	imguiManager->Draw();
#endif _DEBUG

	Sprite::PostDraw();

}

// モデルの作成
void GameScene::CreateModels() {
	// プレイヤーモデルの作成
	modelPlayer_ = Model::CreateFromOBJ("player");
	assert(modelPlayer_ && "Model::CreateFromOBJ(\"player\") failed.");

	// ブロックモデルの作成
	modelBlock_ = Model::CreateFromOBJ("block", true);
	assert(modelBlock_ && "Model::CreateFromOBJ(\"block\") failed.");

	// 敵モデルの作成
	modelEnemy_ = Model::CreateFromOBJ("enemy");
	assert(modelEnemy_ && "Model::CreateFromOBJ(\"enemy\") failed.");

	// パーティクルモデルの生成
	modelParticle_ = Model::CreateFromOBJ("dethParticle");
	assert(modelParticle_ && "Model::CreateFromOBJ(\"dethParticle\") failed.");

	// デスモーションモデルの生成
	modelDethFx_ = Model::CreateFromOBJ("enemy");
	assert(modelDethFx_ && "Model::CreateFromOBJ(\"enemyDethFx\") failed.");

	// アイテムモデルの生成
	for (int i = 0; i < 3; i++) {
		modelItem_[i] = Model::CreateFromOBJ("key");
	}
	assert(modelItem_[0] && "Model::CreateFromOBJ(\"modelItem[0]\") failed.");
	assert(modelItem_[1] && "Model::CreateFromOBJ(\"modelItem[1]\") failed.");
	assert(modelItem_[2] && "Model::CreateFromOBJ(\"modelItem[2]\") failed.");

	// ゴールモデルの生成
	modelGoal_ = Model::CreateFromOBJ("goal");
	assert(modelGoal_ && "Model::CreateFromOBJ(\"modelGoal\") failed.");

	modelDome_ = Model::CreateFromOBJ("dome");
	assert(modelDome_ && "Model::CreateFromOBJ(\"modelDome_\") failed.");

	modelGround_ = Model::CreateFromOBJ("ground");
	assert(modelGround_ && "Model::CreateFromOBJ(\"modelGround_\") failed.");
}

// スプライトの生成
void GameScene::CreateSprite() { 
	spriteBack_ = Sprite::Create(0, Vector2{}); 
	spriteBack_->SetSize(Vector2(WinApp::kWindowWidth, WinApp::kWindowHeight));
	spriteBack_->SetColor(Vector4(0, 0, 0, 0.5f));

	tHAbKey = TextureManager::Load("./Resources/playUi/adkey.png");
	spriteAbKey_ = Sprite::Create(tHAbKey, {1100, 600});

	tHJumpSpace = TextureManager::Load("./Resources/playUi/jumpSpace.png");
	spriteJumpSpace_ = Sprite::Create(tHJumpSpace, {970, 620});

	tHPauseEsc = TextureManager::Load("./Resources/playUi/pauseEsc.png");
	spritePauseEsc_ = Sprite::Create(tHPauseEsc, {0, 0});

	tHSelectBackTitle = TextureManager::Load("./Resources/playUi/pauseSelectBackTitle.png");
	spriteSelectBackTitle_ = Sprite::Create(tHSelectBackTitle, {450, 200});

	tHSelectCotinue = TextureManager::Load("./Resources/playUi/pauseSelectCotinue.png");
	spriteSelectCotinue_ = Sprite::Create(tHSelectCotinue, {450, 200});

	tHSelectSpace = TextureManager::Load("./Resources/playUi/selectSpace.png");
	spriteSelectSpace_ = Sprite::Create(tHSelectSpace, {970, 620});

	tHWsKey = TextureManager::Load("./Resources/playUi/wsKey.png");
	spriteWsKey_ = Sprite::Create(tHWsKey, {1100, 600});
}

// 生成と初期化
void GameScene::CreateAndInits() {

	// マップチップフィールドの生成と初期化
	mapChipField_ = new MapChipField();
	mapChipField_->LoadMapChipCsv("Resources/map.csv");
	GenerateBlocks();

	// ワールド行列初期化
	worldTransform_.Initialize();

	// 天球の生成
	dome_ = new Dome();
	// 天球の初期化
	dome_->Initialize(modelDome_, &camera_);

	// 地面の生成
	ground_ = new Ground();
	// 地面の初期化
	ground_->Initialize(modelGround_, &camera_);

	// 自キャラの生成と初期化
	player_ = new Player();
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 46);
	player_->Init(modelPlayer_, &camera_, playerPosition);
	player_->SetMapField(mapChipField_);

	// 敵の生成と初期化
	Vector3 enemyPosition[8];
	enemyPosition[0] = mapChipField_->GetMapChipPositionByIndex(14, 46);
	enemyPosition[1] = mapChipField_->GetMapChipPositionByIndex(13, 40);
	enemyPosition[2] = mapChipField_->GetMapChipPositionByIndex(15, 31);
	enemyPosition[3] = mapChipField_->GetMapChipPositionByIndex(8, 22);
	enemyPosition[4] = mapChipField_->GetMapChipPositionByIndex(18, 22);
	enemyPosition[5] = mapChipField_->GetMapChipPositionByIndex(8, 16);
	enemyPosition[6] = mapChipField_->GetMapChipPositionByIndex(21, 18);
	enemyPosition[7] = mapChipField_->GetMapChipPositionByIndex(13, 4);
	for (int32_t i = 0; i < 8; ++i) {
		Enemy* newEnemy = new Enemy();
		newEnemy->Init(modelEnemy_, &camera_, enemyPosition[i]);
		enemies_.push_back(newEnemy);
	}

	// 鍵の生成と初期化
	Vector3 itemPosition[3];
	itemPosition[0] = mapChipField_->GetMapChipPositionByIndex(6, 41);
	itemPosition[1] = mapChipField_->GetMapChipPositionByIndex(12, 22);
	itemPosition[2] = mapChipField_->GetMapChipPositionByIndex(1, 11);
	for (int i = 0; i < 3; i++) {
		item_[i] = new Item();
		item_[i]->Init(modelItem_[i], &camera_, itemPosition[i], Item::ItemStatus::Key);
	}

	// ゴールの生成と初期化
	goal_ = new Goal();
	Vector3 goalPosition = mapChipField_->GetMapChipPositionByIndex(6, 4);
	goal_->Init(modelGoal_, &camera_, goalPosition);

	cameraController_ = new CameraController();
	cameraController_->Initialize(&camera_);
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100.0f - 12.0f, 6.0f, 100.0f - 6.0f};
	cameraController_->SetMovableArea(cameraArea);
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

// 全ての当たり判定
void GameScene::CheckAllCollisions() {
	AABB aabbP, aabbE, aabbI[3], aabbG;

	// 自キャラの座標
	aabbP = player_->GetAABB();

	// アイテムの座標
	for (int i = 0; i < 3; i++) {
		aabbI[i] = item_[i]->GetAABB();
	}

	// ゴールの座標
	aabbG = goal_->GetAABB();

	for (Enemy* enemy : enemies_) {
		// 敵の座標
		aabbE = enemy->GetAABB();
		// AABB同士の交差判定
		if (IsCollision(aabbP, aabbE) && !enemy->IsDead()) {
			const float velocityY = player_->GetVelocity().y;
			const float eps = 0.02f;

			const float playerBottomNow = aabbP.min.y;
			const float enemyTop = aabbE.max.y;
			const float playerBottomPrev = playerBottomNow - velocityY;

			if (velocityY < 0.0f && playerBottomPrev > enemyTop + eps && playerBottomNow <= enemyTop + eps) {
				enemy->Kill();
				player_->StompBounce(1.0f);
			} else {
				// 自キャラの衝突時コールバックを呼び出す
				player_->OnCollision(enemy);
			}

			// 敵の衝突時関数を呼び出す
			enemy->OnCollision(player_);
		}
	}

	// アイテムとの判定
	for (int i = 0; i < 3; i++) {
		if (IsCollision(aabbP, aabbI[i]) && !item_[i]->GetterIsGet() && item_[i]) {
			item_[i]->OnCollision(player_);
		}
	}

	// ゴールとの判定
	for (int i = 0; i < 3; i++) {
		if (IsCollision(aabbP, aabbG) && item_[i]->GetterIsGet()) {
			goal_->OnCollision(player_);
		}
	}
}

// プレイヤーデスシーンに移行する関数
void GameScene::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		if (player_->IsDead()) {
			// 死亡演出
			phase_ = Phase::kDeath;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			dethParticle_ = new DethParticles;
			dethParticle_->Initialize(modelParticle_, &camera_, deathParticlesPosition);
		}
		break;
	case Phase::kDeath:
		break;
	}
}
