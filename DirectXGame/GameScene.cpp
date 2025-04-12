#include "GameScene.h"
#include "2d/ImGuiManager.h"
#include "3d/PrimitiveDrawer.h"
#include "3d/AxisIndicator.h"

using namespace KamataEngine;

// デストラクタ
GameScene::~GameScene() {
	//delete sprite_;
	delete model_;
	delete debugCamera_;
};

// 初期化
void GameScene::Initialize() {
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("uvChecker.png");

	// スプライトのインスタンスを生成
	//sprite_ = Sprite::Create(textureHandle_, {100, 50});

	// 3Dモデルの生成
	model_ = Model::Create();

	// ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	// カメラの初期化
	camera_.Initialize();

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);

	// 縦軸方向表示を表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する (アドレス渡し)
	AxisIndicator::GetInstance()->SetTargetCamera(&debugCamera_->GetCamera());

	// サウンドデータの読み込み
	//soundHandle_ = Audio::GetInstance()->LoadWave("fanfare.wav");

	// 音声再生
	//Audio::GetInstance()->PlayWave(soundHandle_);

	//voiceHandle_ = Audio::GetInstance()->PlayWave(soundHandle_, true);

	// ライン描画が参照するカメラを指定する　(アドレス渡し)
	//PrimitiveDrawer::GetInstance()->SetCamera(&camera_);
}

// 更新
void GameScene::Update() {
	// スプライトの今の座標を入力
	//Vector2 position = sprite_->GetPosition();
	// 座標を{ 2, 1 }移動
	//position.x += 2.0f;
	//position.y += 1.0f;
	// 移動した座標をスプライト反映
	//sprite_->SetPosition(position);

	// スペースキーを押した瞬間
	//if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		// 音声停止
		//Audio::GetInstance()->StopWave(voiceHandle_);
	//}

	debugCamera_->Update();

	#ifdef _DEBUG
	ImGui::Begin("Debug1");
	// flot3入力ボックス
	ImGui::InputFloat3("InputFlot3", inputFloat3);
	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
	// デバッグテキストの表示
	//ImGui::Text("Aoto Mori %d.%d.%d", 2050, 12, 31);
	ImGui::End();

	// デモウィンドウの表示を有効化
	ImGui::ShowDemoWindow();
	#endif
}

// 描画
void GameScene::Draw() {
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	// スプライト描画前処理
	//Sprite::PreDraw(dxCommon->GetCommandList());

	//sprite_->Draw();

	// スプライト描画後処理
	//Sprite::PostDraw();

	// 3Dモデル描画前処理
	Model::PreDraw(dxCommon->GetCommandList());


	model_->Draw(worldTransform_, debugCamera_->GetCamera(), textureHandle_);
	//PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

	Model::PostDraw();
}
