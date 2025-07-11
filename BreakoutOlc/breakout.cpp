#include "breakout.h"

Breakout::Breakout(void)
{
	sAppName = "Breakout";
	for (int32_t indexX = 0; indexX < _NumberBricksX; indexX++)
	{
		for (int32_t indexY = 0; indexY < _NumberBricksY; indexY++)
		{
			_bricks[indexX][indexY].doesExist = true;
			_bricks[indexX][indexY].pos.x = indexX * Config::GridSizeUnit;
			_bricks[indexX][indexY].pos.y = indexY * Config::GridSizeUnit + Config::TopLayerHeight;
		}
	}
}

bool Breakout::OnUserCreate(void)
{
	return true;
}

bool Breakout::OnUserUpdate(float fElapsedTime)
{	
	if (!_isGameOver && !_isGameWon)
	{
		Input(fElapsedTime);
		Logic(fElapsedTime);
	}
	Display();
	return true;
}

void Breakout::Logic(float fElapsedTime)
{
	if (!_isGameStarted)
	{
		return;
	}

	BallMovement(fElapsedTime);

	CollisionDetectionBrick();
	CollisionDetectionRoof();
	CollisionDetectionPaddle();
	CollisionDetectionWall();
	CollisionDetectionBottom();

	CheckWinCondition();
}

void Breakout::BallMovement(float fElapsedTime)
{
	_ball.pos.x += _ball.speed * fElapsedTime * cos(_ball.angleRad);
	_ball.pos.y += _ball.speed * fElapsedTime * sin(_ball.angleRad);
}

bool Breakout::CollisionDetectionBottom(void)
{
	if (_ball.pos.y >= Config::ScreenHeight)
	{
		_isGameOver = true;
		return true;
	}
	return false;
}

bool Breakout::CollisionDetectionWall(void)
{
	if (_ball.pos.x <= 0)
	{
		_ball.angleRad = Config::PI - _ball.angleRad;
		return true;
	}
	if (_ball.pos.x >= Config::GameWidth - Config::GridSizeUnit)
	{
		_ball.angleRad = Config::PI - _ball.angleRad;
		return true;
	}
	return false;
}

bool Breakout::CollisionDetectionRoof(void)
{
	if (_ball.pos.y <= Config::TopLayerHeight)
	{
		_ball.angleRad = -_ball.angleRad;
		return true;
	}
	return false;
}

bool Breakout::CollisionDetectionPaddle(void)
{
	if (CheckAABBCollision(_ball.pos.x, _ball.pos.y, Config::GridSizeUnit, Config::GridSizeUnit,
							_paddlePos.x, _paddlePos.y, Config::PaddleWidth, Config::GridSizeUnit))
	{
		float relativeIntersect = (_ball.pos.x - (_paddlePos.x + Config::PaddleWidth / 2.0f)) / (Config::PaddleWidth / 2.0f);
		_ball.angleRad = -Config::PI / 2 + (relativeIntersect * Config::MaxBounceAngle);
		_ball.speed = (std::abs(relativeIntersect) + 1) * Config::BallSpeed90;
		return true;
	}

	return false;
}

bool Breakout::CollisionDetectionBrick(void)
{
	for (size_t indexX = 0; indexX < _NumberBricksX; indexX++)
	{
		for (size_t indexY = 0; indexY < _NumberBricksY; indexY++)
		{
			if (!_bricks[indexX][indexY].doesExist)
			{
				continue;
			}
			
			if (!CheckAABBCollision(_ball.pos.x, _ball.pos.y, Config::GridSizeUnit, Config::GridSizeUnit,
				_bricks[indexX][indexY].pos.x, _bricks[indexX][indexY].pos.y, Config::GridSizeUnit, Config::GridSizeUnit))
			{
				continue;
			}

			_bricks[indexX][indexY].doesExist = false;
			_points++;

			float ballCenterX = _ball.pos.x + Config::GridSizeUnit / 2.0f;
			float ballCenterY = _ball.pos.y + Config::GridSizeUnit / 2.0f;

			float brickCenterX = _bricks[indexX][indexY].pos.x + Config::GridSizeUnit / 2.0f;
			float brickCenterY = _bricks[indexX][indexY].pos.y + Config::GridSizeUnit / 2.0f;

			float deltaX = ballCenterX - brickCenterX;
			float deltaY = ballCenterY - brickCenterY;

			float overlapX = (Config::GridSizeUnit / 2.0f + Config::GridSizeUnit / 2.0f) - std::abs(deltaX);
			float overlapY = (Config::GridSizeUnit / 2.0f + Config::GridSizeUnit / 2.0f) - std::abs(deltaY);

			if (overlapX < overlapY)
			{
				_ball.angleRad = Config::PI - _ball.angleRad;
			}
			else
			{
				_ball.angleRad = -_ball.angleRad;
			}

			return true;
		}
	}
	return false;
}

void Breakout::CheckWinCondition(void)
{
	for (size_t indexX = 0; indexX < _NumberBricksX; indexX++)
	{
		for (size_t indexY = 0; indexY < _NumberBricksY; indexY++)
		{
			if (_bricks[indexX][indexY].doesExist == true)
			{
				return;
			}
		}
	}
	_isGameWon = true;
}

void Breakout::Input(float fElapsedTime)
{
	// Handle User Input
	if (GetKey(olc::Key::LEFT).bHeld)
	{
		_paddlePos.x -= Config::PaddleSpeed * fElapsedTime;
		if (!_isGameStarted)
		{
			_ball.pos.x -= Config::PaddleSpeed * fElapsedTime;
		}
		if (_paddlePos.x <= 0)
		{
			if (!_isGameStarted)
			{
				_ball.pos.x -= _paddlePos.x;
			}
			_paddlePos.x = 0;
		}
	}
	if (GetKey(olc::Key::RIGHT).bHeld)
	{
		_paddlePos.x += Config::PaddleSpeed * fElapsedTime;
		if (!_isGameStarted)
		{
			_ball.pos.x += Config::PaddleSpeed * fElapsedTime;
		}
		if (_paddlePos.x >= Config::GameWidth - Config::PaddleWidth)
		{
			if (!_isGameStarted)
			{
				_ball.pos.x -= _paddlePos.x - (Config::GameWidth - Config::PaddleWidth);
			}
			_paddlePos.x = Config::GameWidth - Config::PaddleWidth;
		}
	}
	if (!_isGameStarted && GetKey(olc::Key::SPACE).bHeld)
	{
		_isGameStarted = true;
	}
}

bool Breakout::CheckAABBCollision(float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh)
{
	return (ax < bx + bw && ax + aw > bx && ay < by + bh && ay + ah > by);
}

void Breakout::Display(void)
{
	if (_isGameWon)
	{
		DrawString(Config::GameWidth + Config::GridSizeUnit, Config::TopLayerHeight + Config::GameHeight / 2, "YOU WON!", olc::BLACK, 1);
		return;
	}
	
	// Erase previous frame
	Clear(olc::WHITE);

	// Draw Top Layer Text
	std::string gameString = "BREAKOUT";
	olc::vi2d textSize = GetTextSize(gameString);
	DrawString((Config::ScreenWidth - textSize.x) / 2, (Config::TopLayerHeight - textSize.y) / 2, gameString, olc::BLACK, 1);

	// Draw Score
	std::string pointString = "Points: ";
	pointString.append(std::to_string(_points));
	DrawString(Config::GameWidth + Config::GridSizeUnit, Config::TopLayerHeight + Config::GridSizeUnit, pointString, olc::BLACK, 1);

	if (_isGameOver)
	{
		DrawString(Config::GameWidth + Config::GridSizeUnit, Config::TopLayerHeight + Config::GameHeight / 2, "GAME OVER!", olc::BLACK, 1);
	}

	// Draw Paddle
	FillRect(_paddlePos.x, _paddlePos.y, Config::PaddleWidth, Config::PaddleHeight, olc::RED);

	// Draw Ball
	FillRect(_ball.pos.x, _ball.pos.y, Config::BallWidth, Config::BallHeight, olc::GREEN);

	// Draw Bricks
	for (size_t indexX = 0; indexX < _NumberBricksX; indexX++)
	{
		for (size_t indexY = 0; indexY < _NumberBricksY; indexY++)
		{
			if (_bricks[indexX][indexY].doesExist)
			{
				FillRect(_bricks[indexX][indexY].pos.x, _bricks[indexX][indexY].pos.y, Config::GridSizeUnit, Config::GridSizeUnit, olc::BLUE);
			}
		}
	}

	// Draw Boundary
	DrawLine(0, Config::TopLayerHeight, Config::ScreenWidth, Config::TopLayerHeight, olc::BLACK);
	for (int32_t y = Config::TopLayerHeight; y < Config::ScreenHeight; y += Config::GridSizeUnit)
	{
		DrawLine(0, y, Config::GameWidth, y, olc::BLACK);
	}
	for (int32_t x = Config::GameWidth; x > 0; x -= Config::GridSizeUnit)
	{
		DrawLine(x, Config::TopLayerHeight, x, Config::ScreenHeight, olc::BLACK);
	}
}