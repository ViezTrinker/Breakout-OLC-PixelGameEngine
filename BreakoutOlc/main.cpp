#include "breakout.h"
#include "config.h"

int main()
{
    Breakout breakout;
    if (breakout.Construct(Config::ScreenWidth, Config::ScreenHeight, Config::PixelWidth, Config::PixelHeight))
    {
        breakout.Start();
    }
}