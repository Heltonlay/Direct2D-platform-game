#ifndef UTILS_hH
#define UTILS_HH
#include <d2d1.h>
#include <cmath>

namespace Utils
{
    struct Line
    {
        D2D1_POINT_2F start{};
        D2D1_POINT_2F end{};
    };

    inline float GetDistance(D2D1_POINT_2F v1, D2D1_POINT_2F v2)
    {
        D2D1_POINT_2F difference{v1.x - v2.x, v1.y - v2.y};
        return std::sqrtf(std::abs(difference.x + difference.y));
    }

    // inline D2D1_POINT_2F Raycast(Line line1, Line line2)
    // {
    //     if(line1.start.x < line2.start.x && line1.end.x > line2.end)
    //     {

    //     }
    // }
}

#endif