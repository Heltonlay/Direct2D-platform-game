#include "Player.hh"

void Player::Draw(ID2D1HwndRenderTarget **pRenderTarget, ID2D1SolidColorBrush **pBrush) const
{
    (*pRenderTarget)->FillRectangle(D2D1::RectF(m_position.x, m_position.y, m_position.x + 20, m_position.y + 20), *pBrush);
}