#ifndef PLAYER_HH
#define PLAYER_HH
#include <d2d1.h>

class Player
{
public:
    Player()
        : m_position{0, 0}
    {
    }

    void Draw(ID2D1HwndRenderTarget **pRenderTarget, ID2D1SolidColorBrush **pBrush);

    D2D1_POINT_2F GetPosition()
    {
        return m_position;
    }
    void SetPosition(D2D1_POINT_2F position)
    {
        m_position = position;
    }
    void AddPosition(D2D1_POINT_2F position)
    {
        m_position.x += position.x;
        m_position.y += position.y;
    }

private:
    D2D1_POINT_2F m_position{};
};

#endif