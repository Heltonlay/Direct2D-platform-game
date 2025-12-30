#ifndef UTILS_hH
#define UTILS_HH
#include <d2d1.h>
#include <cmath>

namespace Utils
{

    inline float GetDistance(D2D1_POINT_2F v1, D2D1_POINT_2F v2)
    {
        D2D1_POINT_2F difference{v1.x - v2.x, v1.y - v2.y};
        return std::sqrtf(std::abs(difference.x + difference.y));
    }
}

#endif