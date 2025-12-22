#include "Obstacle.hh"

void Obstacle::Draw(ID2D1HwndRenderTarget **pRenderTarget, ID2D1SolidColorBrush **pBrush)
{
    (*pRenderTarget)->SetTransform(D2D1::Matrix3x2F::Translation(D2D1::SizeF(m_position.x, m_position.y)));

    (*pRenderTarget)->FillRectangle(D2D1::RectF(0, 0, m_size.x, m_size.y), *pBrush);

    (*pRenderTarget)->SetTransform(D2D1::Matrix3x2F::Identity());
}