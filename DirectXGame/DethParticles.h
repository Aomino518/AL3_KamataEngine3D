#pragma once
#include "KamataEngine.h"
#include <array>
#include "WTFUpdate.h"

using namespace KamataEngine;

class DethParticles {
public:
	void Initialize(Model* model, Camera* camera, const Vector3 &position);

	void Update();

	void Draw();

private:
	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransform_;

	Model* model_ = nullptr;

	Camera* camera_ = nullptr;

	//ObjectColor objectColor_;

};
