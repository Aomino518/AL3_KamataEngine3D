#pragma once
#include "KamataEngine.h"
#include <vector>
#include "MapChipField.h"
#include "Matrix.h"

class Player;

class GameScene {
public:
	GameScene();
	~GameScene();

	// 初期化
	void Initialize();

	// 更新
	void Update();

	// 描画
	void Draw();

	// モデルの作成
	void CreateModels();

	// 生成と初期化処理
	void CreateAndInits();

	/// <summary>
	/// 安全な解放処理
	/// </summary>
	/// <typeparam name="T">すべての型</typeparam>
	/// <param name="ptr">解放したいインスタンス</param>
	template<typename T>
	void SafeDelete(T*& ptr) {
		delete ptr;
		ptr = nullptr;
	}

	/// <summary>
	/// 表示ブロックの生成
	/// </summary>
	void GenerateBlocks();

private:
	// プレイヤー
	Player* player_ = nullptr;
	KamataEngine::Model* modelPlayer_ = nullptr;

	// ブロック
	KamataEngine::Model* modelBlock_ = nullptr;

	KamataEngine::WorldTransform worldTransform_{};
	KamataEngine::Camera camera_{};

	// ワールドトランスフォームブロック
	std::vector<std::vector<KamataEngine::WorldTransform*>> worldTransformBlocks_;

	// マップチップフィールド
	MapChipField* mapChipField_;

#ifdef _DEBUG
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	KamataEngine::DebugCamera* debugCamera_ = nullptr;
#endif _DEBUG

};
