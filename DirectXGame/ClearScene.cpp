#include "ClearScene.h"

ClearScene::~ClearScene() { 
	delete fade_; 
	delete dome_;
	delete modelDome_;
	delete spriteBackTitleSpace_;
	delete spriteGameClear_;
}

void ClearScene::Init() {
	// ワールド行列初期化
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();
	dome_ = new Dome();
	modelDome_ = Model::CreateFromOBJ("dome");
	dome_->Initialize(modelDome_, &camera_);

	tHBackTitle = TextureManager::Load("./Resources/resultUi/backTitleSpace.png");
	spriteBackTitleSpace_ = Sprite::Create(tHBackTitle, {300, 600});

	tHGameClear = TextureManager::Load("./Resources/resultUi/gameClear.png");
	spriteGameClear_ = Sprite::Create(tHGameClear, {200, 200});

	sHGameClear = Audio::GetInstance()->LoadWave("./Resources/sounds/se_Clear.mp3");
	vHGameHandle = Audio::GetInstance()->PlayWave(sHGameClear, false, 0.5f);

	sHSelect = Audio::GetInstance()->LoadWave("./Resources/sounds/se_Select.mp3");

	fade_ = new Fade();
	fade_->Initialize();
	fade_->Start(Fade::Status::FadeIn, 1.0f);
}

void ClearScene::Update() {
	switch (phase_) {
	case ClearScene::Phase::kFadeIn:
		fade_->Update();
		dome_->Update();
		if (fade_->IsFinished()) {
			phase_ = Phase::kGameOver;
		}

		break;
	case ClearScene::Phase::kGameOver:
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			vHSelect = Audio::GetInstance()->PlayWave(sHSelect, false, 0.5f);
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		dome_->Update();
		break;
	case ClearScene::Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			finished = true;
		}
		dome_->Update();
		break;
	}
}

void ClearScene::Draw() {
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	switch (phase_) {
	case ClearScene::Phase::kFadeIn:
		Model::PreDraw(dxCommon->GetCommandList());

		dome_->Draw();

		Model::PostDraw();

		Sprite::PreDraw(dxCommon->GetCommandList());

		spriteBackTitleSpace_->Draw();
		spriteGameClear_->Draw();

		Sprite::PostDraw();

		fade_->Draw();

		break;
	case ClearScene::Phase::kGameOver:
		Model::PreDraw(dxCommon->GetCommandList());

		dome_->Draw();

		Model::PostDraw();

		Sprite::PreDraw(dxCommon->GetCommandList());

		spriteBackTitleSpace_->Draw();
		spriteGameClear_->Draw();

		Sprite::PostDraw();

		break;
	case ClearScene::Phase::kFadeOut:
		Model::PreDraw(dxCommon->GetCommandList());

		dome_->Draw();

		Model::PostDraw();

		Sprite::PreDraw(dxCommon->GetCommandList());

		spriteBackTitleSpace_->Draw();
		spriteGameClear_->Draw();

		Sprite::PostDraw();

		fade_->Draw();

		break;
	}
}
