#ifndef UTILS_hH
#define UTILS_HH
#include <d2d1.h>
#include <cmath>
#include <iostream>

namespace Utils
{
    inline float GetDistance(D2D1_POINT_2F &v1, D2D1_POINT_2F &v2)
    {
        return std::sqrtf(std::abs((v1.x - v2.x) + (v1.y - v2.y)));
    }

    inline float Lerp(float a, float b, float t)
    {
        if (t == 0.0f)
            return a;
        if (t == 1.0f)
            return b;
        return a + (b - a) * t;
    }

    float SweptAABB(const D2D1_POINT_2F &playerPos, const D2D1_POINT_2F &obstaclePos, const D2D1_POINT_2F &obstacleSize, const D2D1_POINT_2F &velocity, float &normalx, float &normaly)
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
                normalx = 1.0f;
            else
                normalx = -1.0f;
        }
        else
        {
            if (dyEntry < 0.0f)
                normaly = 1.0f;
            else
                normaly = -1.0f;
        }

        return entryTime;
    }
}

#endif