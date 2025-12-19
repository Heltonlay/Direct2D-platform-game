#include "MainWindow.hh"
#include "Player.hh"
#include "Globals.hh"
void gameMain();

void MainWindow::OnPaint()
{
    LARGE_INTEGER start{};
    LARGE_INTEGER end{};
    QueryPerformanceCounter(&start);

    HRESULT hr{CreateGraphicsResources()};
    if (SUCCEEDED(hr))
    {
        PAINTSTRUCT ps;
        BeginPaint(m_hWnd, &ps);
        pRenderTarget->BeginDraw();

        pRenderTarget->Clear(D2D1::ColorF(D2D1::ColorF::DeepSkyBlue));

        if (g_player)
            g_player->Draw(&pRenderTarget, &pBrush);

        hr = pRenderTarget->EndDraw();
        EndPaint(m_hWnd, &ps);

        if (FAILED(hr) || hr == D2DERR_RECREATE_TARGET)
            DiscardGraphicsResources();
        else
            InvalidateRect(m_hWnd, NULL, FALSE);
    }

    QueryPerformanceCounter(&end);
    g_deltaT = static_cast<float>(end.QuadPart - start.QuadPart) / static_cast<float>(g_performanceFrequency.QuadPart);

    gameMain();
}

void MainWindow::Resize()
{
    if (pRenderTarget != NULL)
    {
        RECT rc;
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
        ValidateRect(m_hWnd, NULL);
        if (MessageBox(m_hWnd, L"Are you sure you want to close?", L"Please be sure", MB_OKCANCEL) == IDOK)
            DestroyWindow(m_hWnd);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    default:
        return DefWindowProc(m_hWnd, uMsg, wParam, lParam);
    }
}
