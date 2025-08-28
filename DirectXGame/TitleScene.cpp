#include "TitleScene.h"
#include <numbers>
#include "Matrix.h"

using namespace KamataEngine;

TitleScene::~TitleScene() { 
	delete modelPlayer_;
	delete fade_; 
	delete modelDome_;
	delete spriteTitleName_;
	delete spriteTitleStartUi_;
}

void TitleScene::Init() { 
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelDome_ = Model::CreateFromOBJ("dome");

	tHTitleName = TextureManager::Load("./Resources/titleUi/titleName.png");
	tHStartUi = TextureManager::Load("./Resources/titleUi/titleStartUi.png");
	tHMaou = TextureManager::Load("./Resources/titleUi/maou.png");
	spriteTitleName_ = Sprite::Create(tHTitleName, {250, 90});
	spriteTitleStartUi_ = Sprite::Create(tHStartUi, {420, 570});
	spriteMaou_ = Sprite::Create(tHMaou, {1000, 640});

	sHTitleBgm = Audio::GetInstance()->LoadWave("./Resources/sounds/bgm_TitleScene.mp3");
	vHTitleBgm = Audio::GetInstance()->PlayWave(sHTitleBgm, true, 0.5f);

	sHSelect = Audio::GetInstance()->LoadWave("./Resources/sounds/se_Select.mp3");

	// カメラ初期化
	camera_.Initialize();

	const float kPlayerScale = 8.0f;

	worldTransformPlayer_.Initialize();

	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};

	worldTransformPlayer_.translation_.x = -2.0f;
	worldTransformPlayer_.translation_.y = -2.0f;

	fade_ = new Fade(); 
	fade_->Initialize();

	fade_->Start(Fade::Status::FadeIn, 1.0f);

	worldTransformDome_.Initialize();
}

void TitleScene::Update() { 
	switch (phase_) {
	case Phase::kFadeIn:
		fade_->Update();

		if (fade_->IsFinished()) {
			phase_ = Phase::kMain;
		}
		break;
	case Phase::kMain:
		if (Input::GetInstance()->PushKey(DIK_SPACE)) {
			vHSelect = Audio::GetInstance()->PlayWave(sHSelect, false, 0.5f);
			fade_->Start(Fade::Status::FadeOut, 1.0f);
			phase_ = Phase::kFadeOut;
		}
		break;
	case Phase::kFadeOut:
		fade_->Update();
		if (fade_->IsFinished()) {
			Audio::GetInstance()->StopWave(vHTitleBgm);
			finished_ = true;
		}
		break;
	}

	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeTitleMove);

	float angle = counter_ / kTimeTitleMove * 2.0f * std::numbers::pi_v<float>;

	worldTransformTitle_.translation_.y = std::sin(angle) + 10.0f;

	worldTransformPlayer_.rotation_.y += 0.01f;

	worldTransformDome_.TransferMatrix();

	camera_.TransferMatrix();

	WtfUpdate(worldTransformPlayer_);
}

void TitleScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);

	modelPlayer_->Draw(worldTransformPlayer_, camera_);
	modelDome_->Draw(worldTransformDome_, camera_);

	Model::PostDraw();

	Sprite::PreDraw(commandList);
	spriteTitleName_->Draw();
	spriteTitleStartUi_->Draw();
	spriteMaou_->Draw();
	Sprite::PostDraw();

	fade_->Draw();
}
