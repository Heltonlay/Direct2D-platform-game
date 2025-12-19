#include <windows.h>
#include <d2d1.h>
#include "Globals.hh"
#include <iostream>

void gameStart()
{
    g_player = new Player();
}

void gameMain()
{
    if (GetAsyncKeyState('W'))
        g_player->AddPosition(D2D1::Point2F(0, -100.0f * g_deltaT));
    if (GetAsyncKeyState('A'))
        g_player->AddPosition(D2D1::Point2F(-100.0f * g_deltaT, 0));
    if (GetAsyncKeyState('S'))
        g_player->AddPosition(D2D1::Point2F(0, 100.0f * g_deltaT));
    if (GetAsyncKeyState('D'))
        g_player->AddPosition(D2D1::Point2F(100.0f * g_deltaT, 0));
}