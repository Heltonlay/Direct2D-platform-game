#ifndef OBSTACLE_HH
#define OBSTACLE_HH
#include <d2d1.h>

class Obstacle
{
public:
    Obstacle(D2D1_POINT_2F position, D2D1_POINT_2F size) : m_position{position}, m_size{size}
    {
    }

    D2D1_POINT_2F GetPosition() { return m_position; }
    D2D1_POINT_2F GetSize() { return m_size; }

    void SetPosition(D2D1_POINT_2F position)
    {
        m_position = position;
    }
    void SetSize(D2D1_POINT_2F size)
    {
        m_size = size;
    }
    void Draw(ID2D1HwndRenderTarget **pRenderTarget, ID2D1SolidColorBrush **pBrush);

private:
    D2D1_POINT_2F m_position;
    D2D1_POINT_2F m_size;
};

#endif