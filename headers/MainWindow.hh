#ifndef MAINWINDOW_HH
#define MAINWINDOW_HH
#include "BaseWindow.hh"
#include "Globals.hh"

class MainWindow : public BaseWindow<MainWindow>
{
private:
    void OnPaint();
    void Resize();
    
public:
    PCWSTR ClassName() { return L"MainWindow"; }
    LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
};

#endif