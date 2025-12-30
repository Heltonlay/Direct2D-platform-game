#include "Obstacle.hh"

void Obstacle::Draw(ID2D1HwndRenderTarget **pRenderTarget, ID2D1SolidColorBrush **pBrush) const
{
    (*pRenderTarget)->FillRectangle(D2D1::RectF(m_position.x, m_position.y, m_position.x + m_size.x, m_position.y + m_size.y), *pBrush);
}