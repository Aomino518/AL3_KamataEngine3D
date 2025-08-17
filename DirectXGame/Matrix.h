#pragma once
#include <3d/WorldTransform.h>

namespace KamataEngine {

struct AABB {
	Vector3 min;
	Vector3 max;
};

// 加算
Vector3 Add(const Vector3& v1, const Vector3& v2);

Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2);

// 減算
Vector3 Subtract(const Vector3& v1, const Vector3& v2);

Matrix4x4 Subtract(const Matrix4x4& m1, const Matrix4x4& m2);

// 行列の積
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

Vector3 Multiply(const float& scalor, const Vector3& v);

// <summary>
/// X軸回転行列関数
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns>X軸回転行列</returns>
Matrix4x4 MakeRotateXMatrix(float radian);

/// <summary>
/// Y軸回転行列関数
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns>Y軸回転行列</returns>
Matrix4x4 MakeRotateYMatrix(float radian);

/// <summary>
/// Z軸回転行列関数
/// </summary>
/// <param name="radian">ラジアン</param>
/// <returns>Z軸回転行列</returns>
Matrix4x4 MakeRotateZMatrix(float radian);

/// <summary>
/// 行列の積を求める関数
/// </summary>
/// <param name="m1">行列1</param>
/// <param name="m2">行列2</param>
/// <returns>行列の積</returns>
Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

/// <summary>
/// 平行移動行列関数
/// </summary>
/// <param name="translate">方向</param>
/// <returns>平行移動行列</returns>
Matrix4x4 MakeTranslateMatrix(const Vector3& translate);

/// <summary>
/// 拡大縮小行列関数
/// </summary>
/// <param name="scale">スケール</param>
/// <returns>拡大縮小行列</returns>
Matrix4x4 MakeScaleMatrix(const Vector3& scale);

/// <summary>
/// 3次元アフィン変換行列
/// </summary>
/// <param name="scale">拡縮行列</param>
/// <param name="rotate">回転行列</param>
/// <param name="translate">平行移動行列</param>
/// <returns></returns>
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

/// <summary>
/// 線形補間 (Linear Interpolation)
/// </summary>
/// <param name="start">開始値</param>
/// <param name="end">終了値</param>
/// <param name="t">補間係数 (0.0f ～ 1.0f)</param>
/// <returns>補間結果</returns>
Vector3 Lerp(const Vector3& start, const Vector3& end, float t);

Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);

bool IsCollision(const AABB& aabb1, const AABB& aabb2);

Vector3 operator+(const Vector3& v1, const Vector3& v2);

Vector3 operator-(const Vector3& v1, const Vector3& v2);

Vector3 operator*(const Vector3& v1, const Vector3& v2);

Vector3 operator*(float s, const Vector3& v);

Vector3& operator+=(Vector3& v1, const Vector3& v2);

void WtfUpdate(WorldTransform& worldTransform);

} // namespace KamataEngine
