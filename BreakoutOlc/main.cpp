#include "breakout.h"
#include "config.h"

#pragma comment(lib, "Winmm.lib")

int main()
{
    Breakout breakout;
    if (breakout.Construct(Config::ScreenWidth, Config::ScreenHeight, Config::PixelWidth, Config::PixelHeight))
    {
        breakout.Start();
    }
}