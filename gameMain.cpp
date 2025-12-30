#include <windows.h>
#include <d2d1.h>
#include <cmath>
#include "Utils.hh"
#include "Globals.hh"
#include <iostream>
#define GetKey(key) GetAsyncKeyState(key) & 0x8000

float gravity;
bool isSpaceReleased;
bool isGrounded;
D2D1_POINT_2F lastPos{};
D2D1_POINT_2F velocity{};

float sweptAABB(D2D1_POINT_2F &playerPos, D2D1_POINT_2F &obstaclePos, D2D1_POINT_2F &obstacleSize, float &normalx, float &normaly);

void gameStart()
{
    g_player = new Player();

    g_obstacles = {
        {
            D2D1::Point2F(100, 300),
            D2D1::Point2F(250, 50),
        },
        {
            D2D1::Point2F(180, 200),
            D2D1::Point2F(50, 100),
        }};

    lastPos = g_player->GetPosition();
}

void gameMain()
{
    D2D1_POINT_2F mov{};
    if (GetKey('A'))
        mov.x = -120.0f * g_deltaT;
    if (GetKey('D'))
        mov.x = 120.0f * g_deltaT;

    mov.y = gravity * g_deltaT;
    velocity = mov;
    gravity += 9.81f * 30 * g_deltaT;

    D2D1_POINT_2F playerPos{g_player->GetPosition()};

    float collisionTime{1.0f};
    float normalx{0.0f};
    float normaly{0.0f};

    for (int i{0}; i < g_obstacles.size(); i++)
    {
        D2D1_POINT_2F obstaclePos{g_obstacles[i].GetPosition()};
        D2D1_POINT_2F obstacleSize{g_obstacles[i].GetSize()};

        float result{sweptAABB(playerPos, obstaclePos, obstacleSize, normalx, normaly)};
        if (result < collisionTime)
            collisionTime = result;
    }
    g_player->AddPosition(D2D1::Point2F(mov.x * collisionTime, mov.y * collisionTime));

    if (normaly != 0.0f)
    {
        gravity = 0.1f;
        if (normaly == -1.0f)
            isGrounded = true;
    }
    else
        isGrounded = false;

    float remainingTime{1.0f - collisionTime};
    float dotprod{(velocity.x * normaly + velocity.y * normalx) * remainingTime};

    if (!(normalx != 0 && normaly != 0))
        g_player->AddPosition(D2D1::Point2F(dotprod * normaly, dotprod * normalx));

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

float sweptAABB(D2D1_POINT_2F &playerPos, D2D1_POINT_2F &obstaclePos, D2D1_POINT_2F &obstacleSize, float &normalx, float &normaly)
{
    float dxEntry{}, dxExit{};
    float dyEntry{}, dyExit{};

    if (velocity.x > 0)
    {
        dxEntry = obstaclePos.x - (playerPos.x + 20);
        dxExit = (obstaclePos.x + obstacleSize.x) - playerPos.x;
    }
    else
    {
        dxEntry = (obstaclePos.x + obstacleSize.x) - playerPos.x;
        dxExit = obstaclePos.x - (playerPos.x + 20);
    }

    if (velocity.y > 0)
    {
        dyEntry = obstaclePos.y - (playerPos.y + 20);
        dyExit = (obstaclePos.y + obstacleSize.y) - playerPos.y;
    }
    else
    {
        dyEntry = (obstaclePos.y + obstacleSize.y) - playerPos.y;
        dyExit = obstaclePos.y - (playerPos.y + 20);
    }

    float txEntry{}, txExit{};
    float tyEntry{}, tyExit{};

    if (velocity.x == 0.0f)
    {
        txEntry = -std::numeric_limits<float>::infinity();
        txExit = std::numeric_limits<float>::infinity();
    }
    else
    {
        txEntry = dxEntry / velocity.x;
        txExit = dxExit / velocity.x;
    }

    if (velocity.y == 0.0f)
    {
        tyEntry = -std::numeric_limits<float>::infinity();
        tyExit = std::numeric_limits<float>::infinity();
    }
    else
    {
        tyEntry = dyEntry / velocity.y;
        tyExit = dyExit / velocity.y;
    }

    if (tyEntry > 1.0f)
        tyEntry = -std::numeric_limits<float>::infinity();
    if (txEntry > 1.0f)
        txEntry = -std::numeric_limits<float>::infinity();

    float entryTime{std::max(txEntry, tyEntry)};
    float exitTime{std::min(txExit, tyExit)};

    if (entryTime > exitTime)
        return 1.0f;
    if (txEntry < 0.0f && tyEntry < 0.0f)
        return 1.0f;
    if (txEntry < 0.0f)
    {
        if (playerPos.x + 20 < obstaclePos.x || playerPos.x > obstaclePos.x + obstacleSize.x)
            return 1.0f;
    }
    if (tyEntry < 0.0f)
    {
        if (playerPos.y + 20 < obstaclePos.y || playerPos.y > obstaclePos.y + obstacleSize.y)
            return 1.0f;
    }

    if (txEntry > tyEntry)
    {
        if (dxEntry < 0.0f)
        {
            normalx = 1.0f;
        }
        else
        {
            normalx = -1.0f;
        }
    }
    else
    {
        if (dyEntry < 0.0f)
        {
            normaly = 1.0f;
        }
        else
        {
            normaly = -1.0f;
        }
    }

    return entryTime;
}