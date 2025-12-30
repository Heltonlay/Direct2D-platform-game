#include <windows.h>
#include <d2d1.h>
#include <cmath>
#include "Utils.hh"
#include "Globals.hh"
#include <iostream>
#define GetKey(key) GetAsyncKeyState(key) & 0x8000

bool isSpaceReleased;
bool isGrounded;
D2D1_POINT_2F velocity{};
D2D1_POINT_2F movement{};
D2D1_POINT_2F broadPhaseBoxPos{};
float gravity;
float collisionTime{1.0f};
float normalx{0.0f};
float normaly{0.0f};

void gameStart()
{
    g_player = new Player();

    g_obstacles = {
        {
            D2D1::Point2F(100, 300),
            D2D1::Point2F(250, 50),
        },
        {
            D2D1::Point2F(180, 280),
            D2D1::Point2F(50, 20),
        },
        {
            D2D1::Point2F(240, 260),
            D2D1::Point2F(50, 40),
        },
        {
            D2D1::Point2F(1200, 400),
            D2D1::Point2F(70, 40),
        },
        {
            D2D1::Point2F(420, 300),
            D2D1::Point2F(20, 150),
        },
    };
}

void gameMain()
{
    if (GetKey('A'))
        movement.x = Utils::Lerp(movement.x, -120.0f, 10.0f * g_deltaT);
    else if (GetKey('D'))
        movement.x = Utils::Lerp(movement.x, 120.0f, 10.0f * g_deltaT);
    else
        movement.x = Utils::Lerp(movement.x, 0.0f, 10.0f * g_deltaT);

    movement.y = gravity;
    velocity = movement;
    velocity.x *= g_deltaT;
    velocity.y *= g_deltaT;
    gravity += 9.81f * 40 * g_deltaT;

    const D2D1_POINT_2F &playerPos{g_player->GetPosition()};
    broadPhaseBoxPos = playerPos;
    broadPhaseBoxPos.x -= 30;
    broadPhaseBoxPos.y -= 30;

    collisionTime = 1.0f;
    normalx = 0.0f;
    normaly = 0.0f;

    for (int i{0}; i < g_obstacles.size(); i++)
    {
        const D2D1_POINT_2F &obstaclePos{g_obstacles[i].GetPosition()};
        const D2D1_POINT_2F &obstacleSize{g_obstacles[i].GetSize()};

        if (!(broadPhaseBoxPos.x + 80 < obstaclePos.x || broadPhaseBoxPos.x > obstaclePos.x + obstacleSize.x ||
              broadPhaseBoxPos.y + 80 < obstaclePos.y || broadPhaseBoxPos.y > obstaclePos.y + obstacleSize.y))
        {
            const float result{Utils::SweptAABB(playerPos, obstaclePos, obstacleSize, velocity, normalx, normaly)};
            if (result < collisionTime)
                collisionTime = result;
        }
    }
    g_player->AddPosition(movement.x * collisionTime * g_deltaT, movement.y * collisionTime * g_deltaT);

    if (normalx != 0.0f)
        movement.x = 0.0f;
    if (normaly != 0.0f)
    {
        gravity = 1.0f;
        if (normaly == -1.0f)
            isGrounded = true;
    }
    else
        isGrounded = false;

    const float remainingTime{1.0f - collisionTime};
    const float dotprod{(velocity.x * normaly + velocity.y * normalx) * remainingTime};

    if (!(normalx != 0 && normaly != 0))
        g_player->AddPosition(dotprod * normaly, dotprod * normalx);

    if (GetKey(VK_SPACE))
    {
        if (isSpaceReleased && isGrounded)
            gravity = -150.0f;
        isSpaceReleased = false;
    }
    else
        isSpaceReleased = true;
}