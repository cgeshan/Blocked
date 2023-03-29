#include "wx/wx.h"
#include "App.h"
#include "MainWindow.h"

wxIMPLEMENT_APP(App);

bool App::OnInit() 
{
    // Create a window 
    MainWindow *win = new MainWindow(NULL, "Blocked: by Unoptimized");
    win -> SetClientSize(win -> WIN_WIDTH, win -> WIN_HEIGHT);
    win -> Show();
    return true;   
}