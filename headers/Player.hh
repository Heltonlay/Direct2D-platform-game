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

    void Draw(ID2D1HwndRenderTarget **pRenderTarget, ID2D1SolidColorBrush **pBrush) const;

    const D2D1_POINT_2F &GetPosition() const { return m_position; }
    void SetPosition(float x, float y)
    {
        m_position.x = x;
        m_position.y = y;
    }
    void AddPosition(float x, float y)
    {
        m_position.x += x;
        m_position.y += y;
    }

private:
    D2D1_POINT_2F m_position{};
};

#endif