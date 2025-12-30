#ifndef OBSTACLE_HH
#define OBSTACLE_HH
#include <d2d1.h>

class Obstacle
{
public:
    Obstacle(D2D1_POINT_2F position, D2D1_POINT_2F size) : m_position{position}, m_size{size}
    {
    }

    const D2D1_POINT_2F &GetPosition() const { return m_position; }
    const D2D1_POINT_2F &GetSize() const { return m_size; }

    void SetPosition(float x, float y)
    {
        m_position.x = x;
        m_position.y = y;
    }
    void SetSize(float x, float y)
    {
        m_size.x = x;
        m_size.y = y;
    }
    void Draw(ID2D1HwndRenderTarget **pRenderTarget, ID2D1SolidColorBrush **pBrush) const;

private:
    D2D1_POINT_2F m_position;
    D2D1_POINT_2F m_size;
};

#endif