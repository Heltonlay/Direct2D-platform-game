#include <windows.h>
#include <d2d1.h>
#include <cmath>
#include "Utils.hh"
#include "Globals.hh"
#include <iostream>
#define GetKey(key) GetAsyncKeyState(key) & 0x8000

float gravity;
bool isSpaceReleased;
D2D1_POINT_2F lastPos{};

void gameStart()
{
    g_player = new Player();

    g_obstacles.push_back({
        D2D1::Point2F(200, 300),
        D2D1::Point2F(150, 50),
    });

    lastPos = g_player->GetPosition();
}

void gameMain()
{
    if (GetKey('A'))
        g_player->AddPosition(D2D1::Point2F(-100.0f * g_deltaT, 0));
    if (GetKey('D'))
        g_player->AddPosition(D2D1::Point2F(100.0f * g_deltaT, 0));

    g_player->AddPosition(D2D1::Point2F(0, gravity * g_deltaT));
    gravity += 9.81f * 20 * g_deltaT;

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
            D2D1_POINT_2F playerMid{lastPos.x + 10, lastPos.y + 10};

            float distanceUp{Utils::GetDistance(playerMid, D2D1::Point2F(playerMid.x, obstaclePos.y))};
            float distanceDown{Utils::GetDistance(playerMid, D2D1::Point2F(playerMid.x, obstaclePos.y + obstacleSize.y))};
            float distanceLeft{Utils::GetDistance(playerMid, D2D1::Point2F(obstaclePos.x, playerMid.y))};
            float distanceRight{Utils::GetDistance(playerMid, D2D1::Point2F(obstaclePos.x + obstacleSize.x, playerMid.y))};
            float min{std::min(std::min(
                                   std::min(distanceUp, distanceDown),
                                   distanceLeft),
                               distanceRight)};

            if (min == distanceUp)
            {
                g_player->SetPosition(D2D1::Point2F(playerPos.x, obstaclePos.y - 20));
                gravity = 0;
            }
            else if (min == distanceDown)
                g_player->SetPosition(D2D1::Point2F(playerPos.x, obstaclePos.y + obstacleSize.y));
            else if (min == distanceLeft)
                g_player->SetPosition(D2D1::Point2F(obstaclePos.x - 20, playerPos.y));
            else if (min == distanceRight)
                g_player->SetPosition(D2D1::Point2F(obstaclePos.x + obstacleSize.x, playerPos.y));
        }
    }

    if (GetKey(VK_SPACE))
    {
        if (isSpaceReleased)
            gravity = -150.0f;
        isSpaceReleased = false;
    }
    else
        isSpaceReleased = true;

    lastPos = playerPos;
}