#include "MainWindow.hh"
#include "Globals.hh"

void gameStart();

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR pCmdLine, int nCmdShow)
{
    QueryPerformanceFrequency(&g_performanceFrequency);

    MainWindow win{};

    if (!win.Create(
            L"MainWindow", WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, 0,
            (GetSystemMetrics(SM_CXSCREEN) / 2) - 512, (GetSystemMetrics(SM_CYSCREEN) / 2) - 300, 1024, 576))
        return 1;

    ShowWindow(win.Window(), nCmdShow);
    gameStart();
    QueryPerformanceCounter(&g_deltaStart);

    MSG msg{};
    while (GetMessage(&msg, NULL, 0, 0) > 0)
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}