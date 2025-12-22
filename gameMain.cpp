#include <windows.h>
#include <d2d1.h>
#include <math.h>
#include "Globals.hh"
#include <iostream>
#define GetKey(key) GetAsyncKeyState(key) & 0x8000

float gravity;
bool isSpaceReleased;

void gameStart()
{
    g_player = new Player();

    g_obstacles.push_back({
        D2D1::Point2F(300, 300),
        D2D1::Point2F(100, 50),
    });
}

void gameMain()
{
    if (GetKey('A'))
        g_player->AddPosition(D2D1::Point2F(-100.0f * g_deltaT, 0));
    if (GetKey('D'))
        g_player->AddPosition(D2D1::Point2F(100.0f * g_deltaT, 0));

    g_player->AddPosition(D2D1::Point2F(0, gravity * g_deltaT));
    gravity += 9.81f * 20 * g_deltaT;

    if (GetKey(VK_SPACE))
    {
        if (isSpaceReleased)
            gravity = -150.0f;
        isSpaceReleased = false;
    }
    else
        isSpaceReleased = true;

    D2D1_POINT_2F playerPos{g_player->GetPosition()};

    for (int i{0}; i < g_obstacles.size(); i++)
    {
        D2D1_POINT_2F obstaclePos{g_obstacles[i].GetPosition()};
        D2D1_POINT_2F obstacleSize{g_obstacles[i].GetSize()};

        if ((playerPos.x + 20) > obstaclePos.x &&
            playerPos.x < (obstaclePos.x + obstacleSize.x) &&
            (playerPos.y + 20) > obstaclePos.y &&
            playerPos.y < (obstaclePos.y + obstacleSize.y))
        {
            // if((playerPos.y + 20) > obstaclePos.y)
        }
    }
}