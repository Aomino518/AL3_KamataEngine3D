#pragma once
#include <KamataEngine.h>

namespace KamataEngine {
class CameraController {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

private:
	Camera* camera_;
};
}

