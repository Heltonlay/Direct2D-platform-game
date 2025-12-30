#include "MainWindow.hh"
#include "Player.hh"
#include "Globals.hh"
#include <iostream>

void gameMain();

RECT rc{};

void MainWindow::OnPaint()
{
    HRESULT hr{CreateGraphicsResources()};

    if (rc.right == 0)
        GetClientRect(m_hWnd, &rc);

    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hWnd, &ps);
        pRenderTarget->BeginDraw();

        pRenderTarget->SetTransform(D2D1::Matrix3x2F::Scale(D2D1::SizeF(rc.right / 1280.0f, rc.bottom / 720.0f)));

        pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DeepSkyBlue));

        if (g_player)
            g_player->Draw(&pRenderTarget, &pBrush);

        if (g_obstacles.size() > 0)
            for (int i{0}; i < g_obstacles.size(); i++)
                g_obstacles[i].Draw(&pRenderTarget, &pBrush);

        pRenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());

        hr = pRenderTarget->EndDraw();
        EndPaint(m_hWnd, &ps);

        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
            DiscardGraphicsResources();
        else
            InvalidateRect(m_hWnd, NULL, FALSE);

        QueryPerformanceCounter(&g_deltaEnd);
        g_deltaT = static_cast<float>(g_deltaEnd.QuadPart - g_deltaStart.QuadPart) / static_cast<float>(g_performanceFrequency.QuadPart);
        QueryPerformanceCounter(&g_deltaStart);
        gameMain();
    }
}

void MainWindow::Resize()
{
    if (pRenderTarget != NULL)
    {
        GetClientRect(m_hWnd, &rc);
        pRenderTarget->Resize(D2D1::SizeU(rc.right, rc.bottom));
    }
}

LRESULT MainWindow::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_SIZE:
        Resize();
        return 0;
    case WM_PAINT:
        OnPaint();
        return 0;
    case WM_CLOSE:
        DestroyWindow(m_hWnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
    }
}
