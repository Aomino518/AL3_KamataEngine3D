#pragma once
#include <KamataEngine.h>

/// <summary>
/// 天球
/// </summary>
namespace KamataEngine {
class Skydome {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, Camera* camera);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデルデータ
	Model* model_ = nullptr;
	// カメラ
	Camera* camera_ = nullptr;
};
} // namespace KamataEngine
