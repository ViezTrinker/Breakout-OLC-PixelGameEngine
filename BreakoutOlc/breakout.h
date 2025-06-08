#ifndef BREAKOUT_H
#define BREAKOUT_H

#include <cmath>
#include <string_view>

#include "config.h"
#include "olcPixelGameEngine.h"

class Breakout : public olc::PixelGameEngine
{
public:
	Breakout(void);

    bool OnUserCreate(void) override;
    bool OnUserUpdate(float fElapsedTime) override;
private:
    struct Pos
    {
        float x;
        float y;
    };

    struct Brick
    {
        Pos pos;
        bool doesExist;
    };

    struct Ball
    {
        Pos pos;
        float angleRad;
        float speed;
    };

    void Input(float fElapsedTime);
    void Logic(float fElapsedTime);
    void Display(void);

    void BallMovement(float fElapsedTime);
    bool CollisionDetectionBrick(void);
    bool CollisionDetectionPaddle(void);
    bool CollisionDetectionWall(void);
    bool CollisionDetectionRoof(void);
    bool CollisionDetectionBottom(void);

    bool CheckAABBCollision(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh);

    void CheckWinCondition(void);

    Pos _paddlePos = {(Config::GameWidth - Config::PaddleWidth) / 2, Config::ScreenHeight - Config::PaddleHeight};
    Ball _ball = { { (Config::GameWidth - Config::BallWidth) / 2, Config::ScreenHeight - Config::PaddleHeight - Config::BallHeight }, 1.5 * Config::PI, Config::BallSpeed90 };


    static constexpr uint32_t _NumberBricksX = Config::GameWidth / Config::GridSizeUnit;
    static constexpr uint32_t _NumberBricksY = Config::BrickLayers;
    Brick _bricks[Config::GameWidth / Config::GridSizeUnit][_NumberBricksY] = {};

    bool _isGameStarted = false;
    bool _isGameOver = false;
    bool _isGameWon = false;
    uint32_t _points = 0;
};

#endif // BREAKOUT_H