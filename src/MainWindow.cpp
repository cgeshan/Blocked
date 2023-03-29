#include <iostream>
#include <vector>
#include "wx/wx.h"
#include "wx/gbsizer.h"
#include "MainWindow.h"
#include "GLCanvas.h"


MainWindow::MainWindow(wxWindow *parent, const wxString& title) : wxFrame(nullptr, wxID_ANY, title) 
{   
    
    int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};
    canvas = new GLCanvas(mainPanel, args); 

    CreateWindowLayout();
    CreateWindowControls();
    AddControlsToSizers();
    BindEventHandlers();
}

MainWindow::~MainWindow()
{

}

void MainWindow::CreateWindowLayout()
{
    // Menu bar
    menuBar = new wxMenuBar();
    fileMenu = new wxMenu();
    fileMenu -> Append(wxID_NEW, "&New");
    fileMenu -> Append(wxID_OPEN, "&Open");
    fileMenu -> AppendSeparator();
    fileMenu -> Append(wxID_SAVE, "&Save");
    fileMenu -> AppendSeparator();
    fileMenu -> Append(wxID_EXIT, "&Quit");

    menuBar -> Append(fileMenu, "&File");
    SetMenuBar(menuBar);

    // Create an expandable window
    mainSizer = new wxBoxSizer(wxVERTICAL);
    gbSizer = new wxGridBagSizer(FromDIP(5), FromDIP(5));
}

void MainWindow::CreateWindowControls()
{
    // Static texts
    stlInTextLabel = new wxStaticText(mainPanel, wxID_ANY, "STL File: ");
    blockOptsLabel = new wxStaticText(mainPanel, wxID_ANY, "Block Options: ");
    cFunOptsLabel = new wxStaticText(mainPanel, wxID_ANY, "Cost Funtion Options: ");
    BOMLabel = new wxStaticText(mainPanel, wxID_ANY, "Bill of Materials: ");

    // Buttons
    buildButton = new wxButton(mainPanel, wxID_ANY, "Build");
    resetButton = new wxButton(mainPanel, wxID_ANY, "Reset");
    saveButton = new wxButton(mainPanel, wxID_ANY, "Save");
    closeButton = new wxButton(mainPanel, wxID_ANY, "Close");
    exportButton = new wxButton(mainPanel, wxID_ANY, "Export");
    fileExplorerButton = new wxButton(mainPanel, wxID_ANY, "Choose File");

    // User Text Inputs
    stlInFile = new wxTextCtrl(mainPanel, wxID_ANY, "");

    // User sliders
    vizSlider = new wxSlider(mainPanel, wxID_ANY, 100, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL | wxSL_INVERSE);

    // Block size options
    oneByOne = new wxCheckBox(mainPanel, wxID_ANY, "1x1 Blocks");
    oneByTwo = new wxCheckBox(mainPanel, wxID_ANY, "1x2 Blocks");
    oneByThree = new wxCheckBox(mainPanel, wxID_ANY, "1x3 Blocks");
    oneByFour = new wxCheckBox(mainPanel, wxID_ANY, "1x4 Blocks");
    oneByFive = new wxCheckBox(mainPanel, wxID_ANY, "1x5 Blocks");
    oneBySix = new wxCheckBox(mainPanel, wxID_ANY, "1x6 Blocks");
    oneBySeven = new wxCheckBox(mainPanel, wxID_ANY, "1x7 Blocks");
    oneByEight = new wxCheckBox(mainPanel, wxID_ANY, "1x8 Blocks");

    // Cost function options
    cFun1 = new wxCheckBox(mainPanel, wxID_ANY, "Cost Function Option 1");
    cFun2 = new wxCheckBox(mainPanel, wxID_ANY, "Cost Function Option 2");
    cFun3 = new wxCheckBox(mainPanel, wxID_ANY, "Cost Function Option 3");
    
}

void MainWindow::AddControlsToSizers()
{
    gbSizer -> Add(canvas,              {0 , 0}, {13, 2}, wxEXPAND | wxALL);
    gbSizer -> Add(BOMLabel,           {13, 0}, {1 , 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    // gbSizer -> Add(BOMGrid,            {14, 0}, {5 , 2}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(exportButton,       {19, 0}, {1 , 1}, wxEXPAND);
    gbSizer -> Add(vizSlider,          {1 , 2}, {10, 1}, wxEXPAND);
    gbSizer -> Add(stlInTextLabel,     {1 , 3}, {1 , 2}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(stlInFile,          {2 , 3}, {1 , 2}, wxEXPAND);
    gbSizer -> Add(fileExplorerButton, {3 , 3}, {1 , 2}, wxEXPAND);
    gbSizer -> Add(blockOptsLabel,     {6 , 3}, {1 , 2}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(oneByTwo,           {7 , 4}, {1 , 1}, wxALIGN_LEFT);
    gbSizer -> Add(oneByOne,           {7 , 3}, {1 , 1}, wxALIGN_LEFT);
    gbSizer -> Add(oneByThree,         {8 , 3}, {1 , 1}, wxALIGN_LEFT);
    gbSizer -> Add(oneByFour,          {8 , 4}, {1 , 1}, wxALIGN_LEFT);
    gbSizer -> Add(oneByFive,          {9 , 3}, {1 , 1}, wxALIGN_LEFT);
    gbSizer -> Add(oneBySix,           {9 , 4}, {1 , 1}, wxALIGN_LEFT);
    gbSizer -> Add(oneBySeven,         {10, 3}, {1 , 1}, wxALIGN_LEFT);
    gbSizer -> Add(oneByEight,         {10, 4}, {1 , 1}, wxALIGN_LEFT);
    gbSizer -> Add(cFunOptsLabel,      {13, 3}, {1 , 2}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(cFun1,              {14, 3}, {1 , 1}, wxALIGN_LEFT);
    gbSizer -> Add(cFun2,              {14, 4}, {1 , 1}, wxALIGN_LEFT);
    gbSizer -> Add(cFun3,              {15, 3}, {1 , 1}, wxALIGN_LEFT);
    gbSizer -> Add(buildButton,     {18, 3}, {1 , 1}, wxEXPAND);
    gbSizer -> Add(resetButton,        {18, 4}, {1 , 1}, wxEXPAND);
    gbSizer -> Add(saveButton,         {19, 3}, {1 , 1}, wxEXPAND);
    gbSizer -> Add(closeButton,        {19, 4}, {1 , 1}, wxEXPAND);

    // GLCanvas panel grows 3x as fast as other panels
    gbSizer -> AddGrowableRow(0 , 1);
    gbSizer -> AddGrowableRow(1 , 1);    
    gbSizer -> AddGrowableRow(2 , 1);
    gbSizer -> AddGrowableRow(3 , 1);
    gbSizer -> AddGrowableRow(4 , 1);
    gbSizer -> AddGrowableRow(5 , 1);
    gbSizer -> AddGrowableRow(6 , 1);
    gbSizer -> AddGrowableRow(7 , 1);
    gbSizer -> AddGrowableRow(8 , 1);
    gbSizer -> AddGrowableRow(9 , 1);
    gbSizer -> AddGrowableRow(10, 1);
    gbSizer -> AddGrowableRow(11, 1);
    gbSizer -> AddGrowableRow(12, 1);
    gbSizer -> AddGrowableRow(13, 1);
    gbSizer -> AddGrowableRow(14, 1);
    gbSizer -> AddGrowableRow(15, 1);
    gbSizer -> AddGrowableRow(16, 1);
    gbSizer -> AddGrowableRow(17, 1);
    gbSizer -> AddGrowableRow(18, 1);
    gbSizer -> AddGrowableRow(19, 1);

    gbSizer -> AddGrowableCol(0,   1);
    gbSizer -> AddGrowableCol(1,   3);

    // Specify minimum window size as (800, 600)
    gbSizer -> SetMinSize(FromDIP(wxSize(WIN_WIDTH, WIN_HEIGHT)));

    mainPanel -> SetSizer(gbSizer);
    mainSizer -> Add(mainPanel, 1, wxEXPAND | wxALL, FromDIP(5));
    this -> SetSizerAndFit(mainSizer);
}

void MainWindow::BindEventHandlers()
{
    // Bind menu
    Bind(wxEVT_MENU, &MainWindow::OnNew, this, wxID_NEW);
    Bind(wxEVT_MENU, &MainWindow::OnSave, this, wxID_SAVE);
    Bind(wxEVT_MENU, &MainWindow::OnOpen, this, wxID_OPEN);
    Bind(wxEVT_MENU, &MainWindow::OnExit, this, wxID_EXIT);

    // Bind buttons
    closeButton -> Bind(wxEVT_BUTTON, &MainWindow::OnCloseClicked, this);
    saveButton -> Bind(wxEVT_BUTTON, &MainWindow::OnSaveClicked, this);
    buildButton -> Bind(wxEVT_BUTTON, &MainWindow::OnVoxelizeClicked, this);
    resetButton -> Bind(wxEVT_BUTTON, &MainWindow::OnResetClicked, this);
    exportButton -> Bind(wxEVT_BUTTON, &MainWindow::OnExportClicked, this);
    fileExplorerButton -> Bind(wxEVT_BUTTON, &MainWindow::OnChooseFileClicked, this);

    // Bind check boxes

    // Bind slider

    //Bind STL input file

}

void MainWindow::OnNew(wxCommandEvent &evt)
{
    // Closes the current window and opens another
}

void MainWindow::OnOpen(wxCommandEvent &evt)
{
    // Allows the user to open a save voxelization
    wxString filename = wxFileSelector("Open Saved Voxelization", "", "", "",
    #if wxUSE_ZLIB
            "DXF Drawing (*.dxf;*.dxf.gz)|*.dxf;*.dxf.gz|All files (*.*)|*.*",
    #else
            "DXF Drawing (*.dxf)|*.dxf)|All files (*.*)|*.*",
    #endif
            wxFD_OPEN);
}

void MainWindow::OnSave(wxCommandEvent &evt)
{
    // Save the voxelized stl file and BOM
    wxFileDialog *SaveDialog = new wxFileDialog(
		this, _("Save Voxelized STL As _?"), wxEmptyString, wxEmptyString,
		_("STL (*.stl)|*.stl"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

	// Creates a Save Dialog with 4 file types
	if (SaveDialog->ShowModal() == wxID_OK) // If the user clicked "OK"
	{
		CurrentPath = SaveDialog->GetPath();
		// set the path of our current document to the file the user chose to save under
		stlInFile->SaveFile(CurrentPath); // Save the file to the selected path
		// Set the Title to reflect the file open
		SetTitle(wxString("Edit - ") << SaveDialog->GetFilename());
	}

	// Clean up after ourselves
	SaveDialog->Destroy();
}

void MainWindow::OnExit(wxCommandEvent &evt)
{
    // Closes the window
    Close(true);
}

void MainWindow::OnCloseClicked(wxCommandEvent &evt)
{
    // Closes the window
    Close(true);
}

void MainWindow::OnSaveClicked(wxCommandEvent &evt)
{
    // Save the voxelized stl file and BOM
    wxFileDialog *SaveDialog = new wxFileDialog(
		this, _("Save Voxelized STL As _?"), wxEmptyString, wxEmptyString,
		_("STL (*.stl)|*.stl"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

	// Creates a Save Dialog with 4 file types
	if (SaveDialog->ShowModal() == wxID_OK) // If the user clicked "OK"
	{
		CurrentPath = SaveDialog->GetPath();
		// set the path of our current document to the file the user chose to save under
		stlInFile->SaveFile(CurrentPath); // Save the file to the selected path
		// Set the Title to reflect the file open
		SetTitle(wxString("Edit - ") << SaveDialog->GetFilename());
	}

	// Clean up after ourselves
	SaveDialog->Destroy();
}

void MainWindow::OnVoxelizeClicked(wxCommandEvent &evt)
{
    /*
    Call the volxelization algorithm:
        - collect selected options from block sizes
        - collect selected cost function
        - input stl file

        return: BOM and voxelize stl
    */
}

void MainWindow::OnResetClicked(wxCommandEvent &evt)
{
    // Resets the window and clears all inputs/outputs
}

void MainWindow::OnExportClicked(wxCommandEvent &evt)
{
    // Exports the BOM
    wxFileDialog *SaveDialog = new wxFileDialog(
		this, _("Save Bill of Materials As _?"), wxEmptyString, wxEmptyString,
		_("Text File (*.txt)|*.txt"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

	// Creates a Save Dialog with 4 file types
	if (SaveDialog->ShowModal() == wxID_OK) // If the user clicked "OK"
	{
		CurrentPath = SaveDialog->GetPath();
		// set the path of our current document to the file the user chose to save under
		stlInFile->SaveFile(CurrentPath); // Save the file to the selected path
		// Set the Title to reflect the file open
		SetTitle(wxString("Edit - ") << SaveDialog->GetFilename());
	}

	// Clean up after ourselves
	SaveDialog->Destroy();
}

void MainWindow::OnChooseFileClicked(wxCommandEvent &evt)
{
    // Allows the user to import an STL file from explorer
    wxString filename = wxFileSelector("Select STL File", "", "", "",
    #if wxUSE_ZLIB
            "STL File *.stl;",
    #else
            "STL File *.stl;",
    #endif
            wxFD_OPEN);
}
