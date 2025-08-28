#pragma once
#include <KamataEngine.h>
#include <cstdint>
#include <string>
#include <vector>

using KamataEngine::Vector3;

enum class MapChipType { 
	kBlank, // 空白
	kBlock, // ブロック
};

struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField {
public:
	struct IndexSet {
		uint32_t xIndex;
		uint32_t yIndex;
	};

	// 範囲矩形
	struct Rect {
		float left;
		float right;
		float bottom;
		float top;
	};

	/// <summary>
	/// マップチップデータリセット関数
	/// </summary>
	void ResetMapChipData();

	/// <summary>
	/// CSVを読み込む関数
	/// </summary>
	/// <param name="filePath">CSVファイル</param>
	void LoadMapChipCsv(const std::string& filePath);

	/// <summary>
	/// マップチップ種別の取得関数
	/// </summary>
	/// <param name="xIndex">Xインデックス</param>
	/// <param name="yIndex">Yインデックス</param>
	/// <returns>マップチップ種別</returns>
	MapChipType GetMapChipTypeByIndex(uint32_t xIndex, uint32_t yIndex);

	/// <summary>
	/// マップチップ座標の取得関数
	/// </summary>
	/// <param name="xIndex">Xインデックス</param>
	/// <param name="yIndex">Yインデックス</param>
	/// <returns>マップチップ座標</returns>
	Vector3 GetMapChipPositionByIndex(uint32_t xIndex, uint32_t yIndex);

	uint32_t GetNumBlockVirtical() const { return kNumBlockVirtical; }

	uint32_t GetNumBlockHorizontal() const { return kNumBlockHorizontal; }

	IndexSet GetMapChipIndexSetByPosition(const Vector3& position);

	MapChipField::Rect GetRectByIndex(uint32_t xIndex, uint32_t yIndex);

private:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 2.0f;
	static inline const float kBlockHeight = 2.0f;
	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 48;
	static inline const uint32_t kNumBlockHorizontal = 26;

	MapChipData mapChipData_;
};
