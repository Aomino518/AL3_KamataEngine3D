#include "ClearScene.h"
#include "GameOverScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include <KamataEngine.h>
#include <Windows.h>

TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;
GameOverScene* gameOverScene = nullptr;
ClearScene* clearScene = nullptr;

enum class Scene {
	kUnknown = 0,
	kTitle,
	kGame,
	kGameOver,
	kClearScene,
};

Scene scene = Scene::kUnknown;

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			// シーン変更
			scene = Scene::kGame;
			// 旧シーンの解放
			delete titleScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			gameScene = new GameScene();
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFinished()) {
			if (gameScene->IsGameOver()) {
				scene = Scene::kGameOver;
				delete gameScene;
				gameScene = nullptr;
				gameOverScene = new GameOverScene;
				gameOverScene->Init();
			} else if (gameScene->IsGoal()) {
				scene = Scene::kClearScene;
				delete gameScene;
				gameScene = nullptr;
				clearScene = new ClearScene;
				clearScene->Init();
			} else if (gameScene->IsBackTitle()) {
				scene = Scene::kTitle;
				delete gameScene;
				gameScene = nullptr;
				titleScene = new TitleScene;
				titleScene->Init();
			}
		}
		break;
	case Scene::kGameOver:
		if (gameOverScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameOverScene;
			gameOverScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Init();
		}
		break;
	case Scene::kClearScene:
		if (clearScene->IsFinished()) {
			scene = Scene::kTitle;
			delete gameOverScene;
			gameOverScene = nullptr;
			titleScene = new TitleScene;
			titleScene->Init();
		}
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kGameOver:
		gameOverScene->Update();
		break;
	case Scene::kClearScene:
		clearScene->Update();
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kGameOver:
		gameOverScene->Draw();
		break;
	case Scene::kClearScene:
		clearScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(_In_ HINSTANCE, _In_opt_ HINSTANCE, _In_ LPSTR, _In_ int) {

	// エンジンの初期化
	KamataEngine::Initialize(L"LE2C_25_モリ_アオト_AL3");
	using namespace KamataEngine;
	// DirectXCommonインスタンスの取得
	DirectXCommon* dxCommon = DirectXCommon::GetInstance();

	scene = Scene::kTitle;
	titleScene = new TitleScene();
	titleScene->Init();

	// メインループ
	while (true) {
		// エンジンの更新
		if (KamataEngine::Update()) {
			break;
		}

		// ゲームシーンの更新
		ChangeScene();
		UpdateScene();

		// 描画開始
		dxCommon->PreDraw();
		DrawScene();
		// 描画終了
		dxCommon->PostDraw();
	}

	delete titleScene;
	// ゲームシーンの解放
	delete gameScene;
	delete gameOverScene;
	delete clearScene;
	// nullptrの代入
	gameScene = nullptr;
	titleScene = nullptr;
	gameOverScene = nullptr;
	clearScene = nullptr;
	// エンジンの終了処理
	KamataEngine::Finalize();

	return 0;
}
