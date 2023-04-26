/* Author: Connor Geshan */

#ifndef APP_IS_INCLUDED
#define APP_IS_INCLUDED

#include "wx/wx.h"

#include "MainWindow.h"

class App : public wxApp 
{
private:
    MainWindow *win;
public:
    bool OnInit();
};

#endif 