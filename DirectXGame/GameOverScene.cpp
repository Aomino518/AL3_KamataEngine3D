#include "GameOverScene.h"

GameOverScene::~GameOverScene() { 
	delete fade_; 
	delete dome_;
	delete modelDome_;
	delete spriteBackTitle_;
	delete spriteGameOver_;
}

void GameOverScene::Init() {
	// ワールド行列初期化
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();
	dome_ = new Dome();
	modelDome_ = Model::CreateFromOBJ("dome");
	dome_->Initialize(modelDome_, &camera_);

	tHBackTitle = TextureManager::Load("./Resources/resultUi/backTitleSpace.png");
	spriteBackTitle_ = Sprite::Create(tHBackTitle, {300, 600});

	tHGameOver = TextureManager::Load("./Resources/resultUi/gameOver.png");
	spriteGameOver_ = Sprite::Create(tHGameOver, {200, 200});

	sHGameOver = Audio::GetInstance()->LoadWave("./Resources/sounds/se_Gameover.mp3");
	vHGameOver = Audio::GetInstance()->PlayWave(sHGameOver, false, 0.5f);

	sHSelect = Audio::GetInstance()->LoadWave("./Resources/sounds/se_Select.mp3");

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void GameOverScene::Update() {
	switch (phase_) {
	case GameOverScene::Phase::kFadeIn:
		dome_->Update();
		fade_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kGameOver;
		}
		break;
	case GameOverScene::Phase::kGameOver:
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			vHSelect = Audio::GetInstance()->PlayWave(sHSelect, false, 0.5f);
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		dome_->Update();
		break;
	case GameOverScene::Phase::kFadeOut:
		fade_->Update();
		dome_->Update();
		if (fade_->IsFinished()) {
			finished = true;
		}

		break;
	}
}

void GameOverScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	switch (phase_) {
	case GameOverScene::Phase::kFadeIn:
		Model::PreDraw(dxCommon->GetCommandList());

		dome_->Draw();

		Model::PostDraw();

		Sprite::PreDraw(dxCommon->GetCommandList());

		spriteGameOver_->Draw();
		spriteBackTitle_->Draw();

		Sprite::PostDraw();

		fade_->Draw();

		break;
	case GameOverScene::Phase::kGameOver:
		Model::PreDraw(dxCommon->GetCommandList());

		dome_->Draw();

		Model::PostDraw();

		Sprite::PreDraw(dxCommon->GetCommandList());

		spriteGameOver_->Draw();
		spriteBackTitle_->Draw();

		Sprite::PostDraw();

		break;
	case GameOverScene::Phase::kFadeOut:

		Model::PreDraw(dxCommon->GetCommandList());

		dome_->Draw();

		Model::PostDraw();

		Sprite::PreDraw(dxCommon->GetCommandList());

		spriteGameOver_->Draw();
		spriteBackTitle_->Draw();

		Sprite::PostDraw();

		fade_->Draw();

		break;
	}
}
