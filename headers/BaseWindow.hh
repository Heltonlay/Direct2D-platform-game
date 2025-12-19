#ifndef BASEWINDOW_HH
#define BASEWINDOW_HH
#include <windows.h>
#include <initguid.h>
#include <d2d1.h>
#include <dwrite.h>

template <class DERIVED_TYPE>
class BaseWindow
{
public:
    BaseWindow()
        : m_hWnd{NULL}, pFactory{NULL}, pRenderTarget{NULL}, pBrush{NULL}, pWriteFactory{NULL}, pTextFormat{NULL}
    {
    }
    static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    BOOL Create(
        LPCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0,
        int x = CW_USEDEFAULT, int y = CW_USEDEFAULT, int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
        HWND hWndParent = 0, HMENU hMenu = 0);
    HWND Window() { return m_hWnd; }

protected:
    template <class T>
    void SafeRelease(T **p)
    {
        if (*p)
        {
            (*p)->Release();
            *p = NULL;
        }
    }
    ID2D1Factory *pFactory;
    ID2D1HwndRenderTarget *pRenderTarget;
    ID2D1SolidColorBrush *pBrush;

    IDWriteFactory *pWriteFactory;
    IDWriteTextFormat *pTextFormat;

    HWND m_hWnd;
    virtual PCWSTR ClassName() = 0;
    virtual LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) = 0;

    HRESULT CreateGraphicsResources();
    void DiscardGraphicsResources();
};

template <class DERIVED_TYPE>
BOOL BaseWindow<DERIVED_TYPE>::Create(
    LPCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle,
    int x, int y, int nWidth, int nHeight,
    HWND hWndParent, HMENU hMenu)
{
    HINSTANCE hInstance{GetModuleHandle(NULL)};

    WNDCLASS wc{0};
    wc.lpszClassName = ClassName();
    wc.hInstance = hInstance;
    wc.lpfnWndProc = DERIVED_TYPE::WindowProc;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);

    RegisterClass(&wc);

    m_hWnd = CreateWindowEx(
        dwExStyle, ClassName(), lpWindowName, dwStyle,
        x, y, nWidth, nHeight,
        hWndParent, hMenu, GetModuleHandle(NULL), this);

    return m_hWnd ? TRUE : FALSE;
}

template <class DERIVED_TYPE>
LRESULT CALLBACK BaseWindow<DERIVED_TYPE>::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    DERIVED_TYPE *pThis{NULL};

    if (uMsg == WM_NCCREATE)
    {
        CREATESTRUCT *pCreate{reinterpret_cast<CREATESTRUCT *>(lParam)};
        pThis = reinterpret_cast<DERIVED_TYPE *>(pCreate->lpCreateParams);
        SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
        pThis->m_hWnd = hWnd;
    }
    else
        pThis = reinterpret_cast<DERIVED_TYPE *>(GetWindowLongPtr(hWnd, GWLP_USERDATA));

    if (uMsg == WM_CREATE && pThis)
    {
        // Initialize Direct2D and DirectWrite factories
        if (FAILED(D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &(pThis->pFactory))))
            return -1;
        if (FAILED(DWriteCreateFactory(
                DWRITE_FACTORY_TYPE_SHARED,
                IID_IDWriteFactory,
                reinterpret_cast<IUnknown **>(&(pThis->pWriteFactory)))))
            return -1;
    }

    return pThis
               ? pThis->HandleMessage(uMsg, wParam, lParam)
               : DefWindowProc(hWnd, uMsg, wParam, lParam);
}

template <class DERIVED_TYPE>
HRESULT BaseWindow<DERIVED_TYPE>::CreateGraphicsResources()
{
    HRESULT hr{S_OK};

    // Create Direct2D resources
    if (pRenderTarget == NULL)
    {
        RECT rc;
        GetClientRect(m_hWnd, &rc);

        hr = pFactory->CreateHwndRenderTarget(
            D2D1::RenderTargetProperties(),
            D2D1::HwndRenderTargetProperties(m_hWnd, D2D1::SizeU(rc.right, rc.bottom)),
            &pRenderTarget);

        if (SUCCEEDED(hr))
            hr = pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(0, 0, 0), &pBrush);
    }

    // Create DirectWrite resources
    if (pTextFormat == NULL)
    {
        hr = pWriteFactory->CreateTextFormat(
            L"Verdana",
            NULL,
            DWRITE_FONT_WEIGHT_NORMAL,
            DWRITE_FONT_STYLE_NORMAL,
            DWRITE_FONT_STRETCH_NORMAL,
            20,
            L"",
            &pTextFormat);
    }
    return hr;
}

template <class DERIVED_TYPE>
void BaseWindow<DERIVED_TYPE>::DiscardGraphicsResources()
{
    SafeRelease(&pFactory);
    SafeRelease(&pRenderTarget);
    SafeRelease(&pBrush);
    SafeRelease(&pWriteFactory);
    SafeRelease(&pTextFormat);
}

#endif