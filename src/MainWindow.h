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
#include "wx/gbsizer.h"
#include "wx/glcanvas.h"
#include "GLCanvas.h"

class MainWindow : public wxFrame 
{
private:
    // Create a main panel, sizers, and OpenGL canvas
    wxPanel *mainPanel = new wxPanel(this);
    wxBoxSizer *mainSizer;
    wxGridBagSizer *gbSizer;
    GLCanvas *canvas;

    // Create a menu bar
    wxMenu *fileMenu = new wxMenu;
    wxMenuBar *menuBar = new wxMenuBar;

    // Saving helpers
    wxString CurrentPath;

    // Labels
    wxStaticText *stlInTextLabel;
    wxStaticText *blockOptsLabel;
    wxStaticText *cFunOptsLabel;
    wxStaticText *BOMLabel;

    // Buttons
    wxButton *buildButton;
    wxButton *resetButton;
    wxButton *saveButton;
    wxButton *closeButton;
    wxButton *exportButton;
    wxButton *fileExplorerButton;

    // User Text Inputs
    wxTextCtrl *stlInFile;

    // User sliders
    wxSlider *vizSlider;

    // Block size options
    wxCheckBox *oneByOne; 
    wxCheckBox *oneByTwo; 
    wxCheckBox *oneByThree;
    wxCheckBox *oneByFour; 
    wxCheckBox *oneByFive;
    wxCheckBox *oneBySix; 
    wxCheckBox *oneBySeven;
    wxCheckBox *oneByEight;

    // Cost function options
    wxCheckBox *cFun1;
    wxCheckBox *cFun2;
    wxCheckBox *cFun3;

public:
    const int WIN_WIDTH = 800, WIN_HEIGHT = 600;

    MainWindow(wxWindow *parent, const wxString &title);
    virtual ~MainWindow();

    void CreateWindowLayout();
    void CreateWindowControls();
    void AddControlsToSizers();
    void BindEventHandlers();

    void OnNew(wxCommandEvent &evt);
    void OnOpen(wxCommandEvent &evt);
    void OnSave(wxCommandEvent &evt);
    void OnExit(wxCommandEvent &evt);

    void OnCloseClicked(wxCommandEvent &evt);
    void OnSaveClicked(wxCommandEvent &evt);
    void OnVoxelizeClicked(wxCommandEvent &evt);
    void OnResetClicked(wxCommandEvent &evt);
    void OnExportClicked(wxCommandEvent &evt);
    void OnChooseFileClicked(wxCommandEvent &evt);

};

#endif