#pragma once
#include <KamataEngine.h>

/// <summary>
/// 天球
/// </summary>
class Skydome 
{
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

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
	KamataEngine::WorldTransform worldtransform_;
	KamataEngine::Model* model_  = nullptr;
};
