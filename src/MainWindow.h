#ifndef MAINWINDOW_IS_INCLUDED
#define MAINWINDOW_IS_INCLUDED

#ifdef WIN32 
    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>
#endif

#if defined(__APPLE__) && defined(__MACH__)
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif

#include "wx/wxprec.h"
#include "wx/wx.h"
#include "wx/glcanvas.h"
#include "GLCanvas.h"

class MainWindow : public wxFrame 
{
private:
    // Create a main panel and OpenGL canvas
    wxPanel* mainPanel = new wxPanel(this);
    GLCanvas *canvas;

    // Create a menu bar
    wxMenu* fileMenu = new wxMenu;
    wxMenuBar* menuBar = new wxMenuBar;

    // Static texts
    wxStaticText *stlInText;
    wxStaticText *voxelOpts;
    wxStaticText *cFunOpts;
    wxStaticText *BOM;

    // Buttons
    wxButton *voxelizeButton;
    wxButton *resetButton;
    wxButton *saveButton;
    wxButton *closeButton;
    wxButton *exportButton;
    wxButton *fileExplorer;

    // User Text Inputs
    wxTextCtrl *stlInFile;

    // User sliders
    wxSlider *vizSlider;

    // Block size options
    wxCheckBox *oneByOne; 
    wxCheckBox *oneByTwo; 
    wxCheckBox *oneByThree;
    wxCheckBox *oneByFour;

    // Cost function options
    wxCheckBox *cFun1;
    wxCheckBox *cFun2;
    wxCheckBox *cFun3;

public:
    const int WIN_WIDTH = 800, WIN_HEIGHT = 600;

    MainWindow(wxWindow *parent, const wxString &title);
    ~MainWindow();

    void CreateWindowControls();
    void BindEventHandlers();
};

#endif