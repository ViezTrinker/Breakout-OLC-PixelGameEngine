#ifndef CONFIG_H
#define CONFIG_H

#include <cstdint>

namespace Config
{
	inline constexpr int32_t ScreenWidth = 300;
	inline constexpr int32_t ScreenHeight = 245;

	inline constexpr int32_t PixelWidth = 2;
	inline constexpr int32_t PixelHeight = 2;

	inline constexpr int32_t TopLayerWidth = ScreenWidth;
	inline constexpr int32_t TopLayerHeight = 15;

	inline constexpr int32_t GameWidth = 200;
	inline constexpr int32_t GameHeight = 230;

	inline constexpr int32_t GridSizeUnit = 10;

	inline constexpr int32_t InfoBoardWidth = 100;
	inline constexpr int32_t InfoBoardHeight = GameHeight;

	inline constexpr int32_t PaddleWidth = 50;
	inline constexpr int32_t PaddleHeight = GridSizeUnit;

	inline constexpr int32_t BallWidth = GridSizeUnit;
	inline constexpr int32_t BallHeight = GridSizeUnit;

	inline constexpr double PaddleSpeed = 100;
	inline constexpr double BallSpeed90 = 100;

	inline constexpr int32_t BrickLayers = 10;

	inline constexpr double PI = 3.14159;
	inline constexpr double MaxBounceAngle = PI / 3;

	static_assert(ScreenWidth == InfoBoardWidth + GameWidth);
	static_assert(ScreenHeight == TopLayerHeight + GameHeight);

	static_assert(GameWidth % GridSizeUnit == 0);
	static_assert(GameHeight % GridSizeUnit == 0);
	static_assert(PaddleWidth % GridSizeUnit == 0);

	static_assert(BrickLayers* GridSizeUnit < GameHeight);
}

#endif // CONFIG_H