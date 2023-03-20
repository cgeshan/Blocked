#include "wx/wx.h"
#include "MainWindow.h"
#include "GLCanvas.h"

MainWindow::MainWindow(wxWindow *parent, const wxString& title) : wxFrame(nullptr, wxID_ANY, title) 
{
    CreateWindowControls();
    BindEventHandlers();
}

MainWindow::~MainWindow()
{
    
}

void MainWindow::CreateWindowControls()
{
    menuBar = new wxMenuBar();
    fileMenu = new wxMenu();
    fileMenu -> Append(wxID_NEW, "&New");
    fileMenu -> Append(wxID_OPEN, "&Open");
    fileMenu -> AppendSeparator();
    fileMenu -> Append(wxID_ANY, "&Export BOM");
    fileMenu -> Append(wxID_SAVE, "&Save");
    fileMenu -> AppendSeparator();
    fileMenu -> Append(wxID_EXIT, "&Quit");

    menuBar -> Append(fileMenu, "&File");
    SetMenuBar(menuBar);

    // Static texts
    stlInText = new wxStaticText(mainPanel, wxID_ANY, "STL File: ", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 570));
    voxelOpts = new wxStaticText(mainPanel, wxID_ANY, "Voxelization Options: ", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 360));
    cFunOpts = new wxStaticText(mainPanel, wxID_ANY, "Cost Funtion Options: ", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 210));
    BOM = new wxStaticText(mainPanel, wxID_ANY, "Bill of Materials: ", wxPoint(WIN_WIDTH - 795, WIN_HEIGHT - 195));

    // Buttons
    voxelizeButton = new wxButton(mainPanel, wxID_ANY, "Voxelize", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 100), wxSize(90, 35));
    resetButton = new wxButton(mainPanel, wxID_ANY, "Reset", wxPoint(WIN_WIDTH - 100, WIN_HEIGHT - 100), wxSize(90, 35));
    saveButton = new wxButton(mainPanel, wxID_ANY, "Save", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 50), wxSize(90, 35));
    closeButton = new wxButton(mainPanel, wxID_ANY, "Close", wxPoint(WIN_WIDTH - 100, WIN_HEIGHT - 50), wxSize(90, 35));
    exportButton = new wxButton(mainPanel, wxID_ANY, "Export", wxPoint(WIN_WIDTH - 790, WIN_HEIGHT - 50), wxSize(90, 35));
    fileExplorer = new wxButton(mainPanel, wxID_ANY, "Choose File", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 515), wxSize(90, 25));

    // User Text Inputs
    stlInFile = new wxTextCtrl(mainPanel, wxID_ANY, "", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 550), wxSize(195, -1));

    // User sliders
    vizSlider = new wxSlider(mainPanel, wxID_ANY, 100, 0, 100, wxPoint(WIN_WIDTH - 230, WIN_HEIGHT - 500), wxSize(-1, 250), wxSL_VERTICAL | wxSL_INVERSE);

    // Block size options
    oneByOne = new wxCheckBox(mainPanel, wxID_ANY, "1x1 Blocks", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 340));
    oneByTwo = new wxCheckBox(mainPanel, wxID_ANY, "1x2 Blocks", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 310));
    oneByThree = new wxCheckBox(mainPanel, wxID_ANY, "1x3 Blocks", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 280));
    oneByFour = new wxCheckBox(mainPanel, wxID_ANY, "1x4 Blocks", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 250));

    // Cost function options
    cFun1 = new wxCheckBox(mainPanel, wxID_ANY, "Cost Function Option 1", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 190));
    cFun2 = new wxCheckBox(mainPanel, wxID_ANY, "Cost Function Option 2", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 160));
    cFun3 = new wxCheckBox(mainPanel, wxID_ANY, "Cost Function Option 3", wxPoint(WIN_WIDTH - 200, WIN_HEIGHT - 130));

    // OpenGL canvas for rendering stl file and voxelized stl file
    canvas = new GLCanvas(mainPanel, wxID_ANY, wxPoint(0, 0), wxSize(570, 400));
}

void MainWindow::BindEventHandlers()
{

}