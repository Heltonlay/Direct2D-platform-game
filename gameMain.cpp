#include <windows.h>
#include <d2d1.h>
#include "Globals.hh"
#include <iostream>

float gravity;
bool isSpaceReleased;

void gameStart()
{
    g_player = new Player();
}

void gameMain()
{
    if (GetAsyncKeyState('A') & 0x8000)
        g_player->AddPosition(D2D1::Point2F(-100.0f * g_deltaT, 0));
    if (GetAsyncKeyState('D') & 0x8000)
        g_player->AddPosition(D2D1::Point2F(100.0f * g_deltaT, 0));

    g_player->AddPosition(D2D1::Point2F(0, gravity * g_deltaT));
    gravity += 9.81f * 20 * g_deltaT;

    if (GetAsyncKeyState(VK_SPACE) & 0x8000)
    {
        if (isSpaceReleased)
            gravity = -150.0f;
        isSpaceReleased = false;
    }
    else
        isSpaceReleased = true;

    D2D1_POINT_2F playerPosition{g_player->GetPosition()};
    if (playerPosition.y > 400)
    {
        g_player->SetPosition(D2D1::Point2F(playerPosition.x, 400));
    }
}