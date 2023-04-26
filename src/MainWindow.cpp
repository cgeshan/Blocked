/* Author: Connor Geshan */

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

#include "wx/wx.h"
#include "wx/filename.h"
#include "wx/aboutdlg.h"
#include "wx/progdlg.h"

#include "MainWindow.h"
#include "GLCanvas.h"
#include "Voxelize.h"
#include "Block.h"
#include "Merge.h"

int args[] = {WX_GL_RGBA, WX_GL_DOUBLEBUFFER, WX_GL_DEPTH_SIZE, 16, 0};

wxBEGIN_EVENT_TABLE(MainWindow, wxFrame)
    EVT_COMMAND_SCROLL(wxID_ANY, MainWindow::OnSliderScroll)
wxEND_EVENT_TABLE()

MainWindow::MainWindow(wxWindow *parent, const wxString& title) : wxFrame(nullptr, wxID_ANY, title) 
{   
    std::cout << "Generating main window...\n";
    canvas = new GLCanvas(mainPanel, args); 

    CreateWindowLayout();
    CreateWindowControls();
    AddControlsToSizers();
    BindEventHandlers();
    ClearWindow();
    
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
    gbSizer = new wxGridBagSizer(FromDIP(9), FromDIP(5));
}

void MainWindow::CreateWindowControls()
{
    // Static texts
    stlInTextLabel  = new wxStaticText(mainPanel, wxID_ANY, "STL File: ");
    voxelSizeLabel  = new wxStaticText(mainPanel, wxID_ANY, "No. of Voxels:");
    blockOptsLabel  = new wxStaticText(mainPanel, wxID_ANY, "Block Options: ");
    costLabel1      = new wxStaticText(mainPanel, wxID_ANY, "Cost: ");
    costLabel2      = new wxStaticText(mainPanel, wxID_ANY, "Cost: ");
    costLabel3      = new wxStaticText(mainPanel, wxID_ANY, "Cost: ");
    costLabel4      = new wxStaticText(mainPanel, wxID_ANY, "Cost: ");
    costLabel5      = new wxStaticText(mainPanel, wxID_ANY, "Cost: ");
    costLabel6      = new wxStaticText(mainPanel, wxID_ANY, "Cost: ");
    costLabel7      = new wxStaticText(mainPanel, wxID_ANY, "Cost: ");
    costLabel8      = new wxStaticText(mainPanel, wxID_ANY, "Cost: ");
    totalCost       = new wxStaticText(mainPanel, wxID_ANY, "Total Cost: ");
    BOMLabel        = new wxStaticText(mainPanel, wxID_ANY, "Bill of Materials: ");
    blockType1Label = new wxStaticText(mainPanel, wxID_ANY, "Type of Block: ");
    blockType2Label = new wxStaticText(mainPanel, wxID_ANY, "Type of Block: ");
    blockQty1Label  = new wxStaticText(mainPanel, wxID_ANY, "Qty: ");
    blockQty2Label  = new wxStaticText(mainPanel, wxID_ANY, "Qty: ");

    // Buttons
    buildButton        = new wxButton(mainPanel, wxID_ANY, "Build");
    resetButton        = new wxButton(mainPanel, wxID_ANY, "Reset");
    saveButton         = new wxButton(mainPanel, wxID_ANY, "Save");
    closeButton        = new wxButton(mainPanel, wxID_ANY, "Close");
    exportButton       = new wxButton(mainPanel, wxID_ANY, "Export");
    fileExplorerButton = new wxButton(mainPanel, wxID_ANY, "Choose File");

    // User Text Inputs
    stlInFile = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    voxelSize = new wxTextCtrl(mainPanel, wxID_ANY, "10", wxDefaultPosition, wxSize(35, -1), wxALIGN_RIGHT);

    // Bill of materials grid
    block1  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    block2  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    block3  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    block4  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    block5  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    block6  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    block7  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);
    block8  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY);

    block1_Qty  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxALIGN_RIGHT);
    block2_Qty  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxALIGN_RIGHT);
    block3_Qty  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxALIGN_RIGHT);
    block4_Qty  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxALIGN_RIGHT);
    block5_Qty  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxALIGN_RIGHT);
    block6_Qty  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxALIGN_RIGHT);
    block7_Qty  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxALIGN_RIGHT);
    block8_Qty  = new wxTextCtrl(mainPanel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxALIGN_RIGHT);

    // User sliders
    vizSlider = new wxSlider(mainPanel, wxID_ANY, 100, 0, 100, wxDefaultPosition, wxDefaultSize, wxSL_VERTICAL | wxSL_INVERSE);

    // Stabilize options
    stabilizeLabel  = new wxStaticText(mainPanel, wxID_ANY, "Stabilize? ");
    isStable   = new wxCheckBox(mainPanel, wxID_ANY, "");

    // Block size options
    oneByOne   = new wxCheckBox(mainPanel, wxID_ANY, "1x1 Blocks");
    oneByTwo   = new wxCheckBox(mainPanel, wxID_ANY, "1x2 Blocks");
    oneByThree = new wxCheckBox(mainPanel, wxID_ANY, "1x3 Blocks");
    oneByFour  = new wxCheckBox(mainPanel, wxID_ANY, "1x4 Blocks");
    twoByTwo   = new wxCheckBox(mainPanel, wxID_ANY, "2x2 Blocks");
    twoByThree = new wxCheckBox(mainPanel, wxID_ANY, "2x3 Blocks");
    twoByFour  = new wxCheckBox(mainPanel, wxID_ANY, "2x4 Blocks");
    L_Shaped   = new wxCheckBox(mainPanel, wxID_ANY, "L   Blocks");

    // Cost options
    costBlock1x1  = new wxTextCtrl(mainPanel, wxID_ANY, "1.00", wxDefaultPosition, wxSize(45, -1), wxALIGN_RIGHT);
    costBlock1x2  = new wxTextCtrl(mainPanel, wxID_ANY, "1.00", wxDefaultPosition, wxSize(45, -1), wxALIGN_RIGHT);
    costBlock1x3  = new wxTextCtrl(mainPanel, wxID_ANY, "1.00", wxDefaultPosition, wxSize(45, -1), wxALIGN_RIGHT);
    costBlock1x4  = new wxTextCtrl(mainPanel, wxID_ANY, "1.00", wxDefaultPosition, wxSize(45, -1), wxALIGN_RIGHT);
    costBlock2x2  = new wxTextCtrl(mainPanel, wxID_ANY, "1.00", wxDefaultPosition, wxSize(45, -1), wxALIGN_RIGHT);
    costBlock2x3  = new wxTextCtrl(mainPanel, wxID_ANY, "1.00", wxDefaultPosition, wxSize(45, -1), wxALIGN_RIGHT);
    costBlock2x4  = new wxTextCtrl(mainPanel, wxID_ANY, "1.00", wxDefaultPosition, wxSize(45, -1), wxALIGN_RIGHT);
    costBlockL    = new wxTextCtrl(mainPanel, wxID_ANY, "1.00", wxDefaultPosition, wxSize(45, -1), wxALIGN_RIGHT);
    total         = new wxTextCtrl(mainPanel, wxID_ANY, "1.00", wxDefaultPosition, wxDefaultSize, wxTE_READONLY | wxALIGN_RIGHT);;

    canvas->MoveAfterInTabOrder(fileExplorerButton);
}

void MainWindow::AddControlsToSizers()
{
    // OpenGL Canvas
    gbSizer -> Add(canvas,             {0 , 0}, {13, 6}, wxEXPAND | wxALL);

    // Bill of materials 
    gbSizer -> Add(BOMLabel,           {13, 0}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(blockType1Label,    {14, 0}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(blockType2Label,    {14, 3}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(blockQty1Label,     {14, 2}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(blockQty2Label,     {14, 5}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block1,             {15, 0}, { 1, 2}, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block2,             {16, 0}, { 1, 2}, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block3,             {17, 0}, { 1, 2}, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block4,             {18, 0}, { 1, 2}, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block5,             {15, 3}, { 1, 2}, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block6,             {16, 3}, { 1, 2}, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block7,             {17, 3}, { 1, 2}, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block8,             {18, 3}, { 1, 2}, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block1_Qty,         {15, 2}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block2_Qty,         {16, 2}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block3_Qty,         {17, 2}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block4_Qty,         {18, 2}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block5_Qty,         {15, 5}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block6_Qty,         {16, 5}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block7_Qty,         {17, 5}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(block8_Qty,         {18, 5}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(exportButton,       {19, 0}, { 1, 1}, wxEXPAND);
    gbSizer -> Add(totalCost,          {19, 3}, { 1, 2}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(total,              {19, 5}, { 1, 1}, wxALIGN_LEFT  | wxALIGN_CENTER_VERTICAL);

    // Slider for block render
    gbSizer -> Add(vizSlider,          { 1, 6}, {10, 1}, wxEXPAND);

    // STL File stuff
    gbSizer -> Add(stlInTextLabel,     { 0, 7}, { 1, 4}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(stlInFile,          { 1, 7}, { 1, 4}, wxEXPAND | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(fileExplorerButton, { 2, 7}, { 1, 4}, wxEXPAND | wxALIGN_CENTER_VERTICAL);

    // Voxel size options
    gbSizer -> Add(voxelSizeLabel,     { 3, 7}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(voxelSize,          { 3, 10}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    // Stabilize options
    gbSizer -> Add(stabilizeLabel,     { 4, 7}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(isStable,           { 4, 10}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    // Block options
    gbSizer -> Add(blockOptsLabel,     { 5, 7}, { 1, 2}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(oneByOne,           { 6, 7}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(oneByTwo,           { 7, 7}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(oneByThree,         { 8, 7}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(oneByFour,          { 9, 7}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(twoByTwo,           {10, 7}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(twoByThree,         {11, 7}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(twoByFour,          {12, 7}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(L_Shaped,           {13, 7}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    // Cost functions
    gbSizer -> Add(costLabel1,          { 6, 9}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costLabel2,          { 7, 9}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costLabel3,          { 8, 9}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costLabel4,          { 9, 9}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costLabel5,          {10, 9}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costLabel6,          {11, 9}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costLabel7,          {12, 9}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costLabel8,          {13, 9}, { 1, 1}, wxALIGN_RIGHT | wxALIGN_CENTER_VERTICAL);

    gbSizer -> Add(costBlock1x1,       { 6,10}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costBlock1x2,       { 7,10}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costBlock1x3,       { 8,10}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costBlock1x4,       { 9,10}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costBlock2x2,       {10,10}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costBlock2x3,       {11,10}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costBlock2x4,       {12,10}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);
    gbSizer -> Add(costBlockL,         {13,10}, { 1, 1}, wxALIGN_LEFT | wxALIGN_CENTER_VERTICAL);

    // Operation Buttons
    gbSizer -> Add(buildButton,        {18, 7}, { 1, 1}, wxEXPAND);
    gbSizer -> Add(resetButton,        {18, 8}, { 1, 3}, wxEXPAND);
    gbSizer -> Add(saveButton,         {19, 7}, { 1, 1}, wxEXPAND);
    gbSizer -> Add(closeButton,        {19, 8}, { 1, 3}, wxEXPAND);

    // GLCanvas panel grows 3x as fast as other panels
    gbSizer -> AddGrowableRow( 0, 1);
    gbSizer -> AddGrowableRow( 1, 1);    
    gbSizer -> AddGrowableRow( 2, 1);
    gbSizer -> AddGrowableRow( 3, 1);
    gbSizer -> AddGrowableRow( 4, 1);
    gbSizer -> AddGrowableRow( 5, 1);
    gbSizer -> AddGrowableRow( 6, 1);
    gbSizer -> AddGrowableRow( 7, 1);
    gbSizer -> AddGrowableRow( 8, 1);
    gbSizer -> AddGrowableRow( 9, 1);
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

    // gbSizer -> AddGrowableCol( 0, 1);
    gbSizer -> AddGrowableCol( 1, 2);
    gbSizer -> AddGrowableCol( 3, 2);

    
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
    closeButton        -> Bind(wxEVT_BUTTON, &MainWindow::OnCloseClicked, this);
    saveButton         -> Bind(wxEVT_BUTTON, &MainWindow::OnSaveClicked, this);
    buildButton        -> Bind(wxEVT_BUTTON, &MainWindow::OnVoxelizeClicked, this);
    resetButton        -> Bind(wxEVT_BUTTON, &MainWindow::OnResetClicked, this);
    exportButton       -> Bind(wxEVT_BUTTON, &MainWindow::OnExportClicked, this);
    fileExplorerButton -> Bind(wxEVT_BUTTON, &MainWindow::OnChooseFileClicked, this);

    // Set Canvas Focus 
    canvas -> MoveAfterInTabOrder(stlInFile);
}

void MainWindow::OnNew(wxCommandEvent &evt)
{
    // Closes the current window and opens another
    std::cout << "Creating new window..." << std::endl;
    ClearWindow();
    Refresh();
}

void MainWindow::OnOpen(wxCommandEvent &evt)
{
    // Allows the user to open a save voxelization
    filename = wxFileSelector("Open Saved Voxelization", "", "", "");
    Import(filename);
}

void MainWindow::OnSave(wxCommandEvent &evt)
{
    // Save the voxelized stl file and BOM
    wxFileDialog *SaveDialog = new wxFileDialog(
		this, _("Save Voxelized Blocks As _?"), wxEmptyString, wxEmptyString,
		_("Text File (*.txt)|*.txt"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

	// Creates a Save Dialog with 4 file types
	if (SaveDialog->ShowModal() == wxID_OK) // If the user clicked "OK"
	{
		CurrentPath = SaveDialog->GetPath();
        wxString extension = wxFileName(CurrentPath).GetExt();
        if (extension != "txt") 
        {
            CurrentPath += ".txt";
        }
		// set the path of our current document to the file the user chose to save under
		SaveBlocks(CurrentPath, mergedBlocks);
	}
	// Clean up after ourselves
	SaveDialog->Destroy();
}

void MainWindow::OnSaveClicked(wxCommandEvent &evt)
{
    // Save the voxelized stl file and BOM
    wxFileDialog *SaveDialog = new wxFileDialog(
		this, _("Save Voxelized Blocks As _?"), wxEmptyString, wxEmptyString,
		_("Text File (*.txt)|*.txt"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

	// Creates a Save Dialog with 4 file types
	if (SaveDialog->ShowModal() == wxID_OK) // If the user clicked "OK"
	{
		CurrentPath = SaveDialog->GetPath();
        wxString extension = wxFileName(CurrentPath).GetExt();
        if (extension != "txt") 
        {
            CurrentPath += ".txt";
        }
		// set the path of our current document to the file the user chose to save under
		SaveBlocks(CurrentPath, mergedBlocks);
	}

	// Clean up after ourselves
	SaveDialog->Destroy();
}

void MainWindow::OnExit(wxCommandEvent &evt)
{
    // Closes the window
    std::cout << "Closing Application...\n";
    Close(true);
}

void MainWindow::OnCloseClicked(wxCommandEvent &evt)
{
    // Closes the window
    std::cout << "Closing Application...\n";
    Close(true);
}

void MainWindow::OnResetClicked(wxCommandEvent &evt)
{
    // Resets the window and clears all inputs/outputs
    canvas -> noRender = true;
    canvas -> needsRendered = false;
    canvas -> stlRender = false;
    canvas -> blockRender = false;
    ClearWindow();
    Refresh();
}

void MainWindow::OnChooseFileClicked(wxCommandEvent &evt)
{
    canvas -> Clear();
    ClearBOM();

    // Allows the user to import an STL file from explorer
    filename = wxFileSelector("Select STL File", "", "", "",
    #if wxUSE_ZLIB
            "STL (*.stl)|*.stl;",
    #else
            "STL (*.stl)|*.stl;",
    #endif
            wxFD_OPEN);
    
    std::cout << filename << " selected, rendering STL..." << std::endl;
    canvas -> SetSTLPath(filename);
    canvas -> noRender = false;
    canvas -> needsRendered = true;
    canvas -> stlRender = true;
    canvas -> blockRender = false;
    canvas -> zoom = false;
    stlInFile -> Clear();
    stlInFile -> AppendText(filename);
    
    Refresh(true);
}

void MainWindow::OnVoxelizeClicked(wxCommandEvent &evt)
{
    if(stlInFile -> GetValue() != "")
    {
        double test;
        if((costBlock1x1 -> IsEmpty() || costBlock1x1 -> GetValue().ToDouble(&test) == false) && oneByOne   -> GetValue() == true ||
           (costBlock1x2 -> IsEmpty() || costBlock1x2 -> GetValue().ToDouble(&test) == false) && oneByTwo   -> GetValue() == true ||
           (costBlock1x3 -> IsEmpty() || costBlock1x3 -> GetValue().ToDouble(&test) == false) && oneByThree -> GetValue() == true ||
           (costBlock1x4 -> IsEmpty() || costBlock1x4 -> GetValue().ToDouble(&test) == false) && oneByFour  -> GetValue() == true ||
           (costBlock2x2 -> IsEmpty() || costBlock2x2 -> GetValue().ToDouble(&test) == false) && twoByTwo   -> GetValue() == true ||
           (costBlock2x3 -> IsEmpty() || costBlock2x3 -> GetValue().ToDouble(&test) == false) && twoByThree -> GetValue() == true ||
           (costBlock2x4 -> IsEmpty() || costBlock2x4 -> GetValue().ToDouble(&test) == false) && twoByFour  -> GetValue() == true ||
           (costBlockL   -> IsEmpty() || costBlockL   -> GetValue().ToDouble(&test) == false) && L_Shaped   -> GetValue() == true )
        {
            DisplayErrorMessage("Please ensure all cost options for selected blocks are numbers.");
        }
        else
        {
            if(voxelSize -> GetValue() != "")
            {
                std::vector <std::vector <Block>> voxelLayers;
                std::vector <Block> opts = GetBlockOptions();
                {
                    wxProgressDialog progBar("Build Progress", "Building...", 100, this,
                                            wxPD_AUTO_HIDE | wxPD_APP_MODAL | wxPD_CAN_ABORT | wxPD_SMOOTH);
                
                    // Conduct voxelization
                    int size;
                    voxelSize -> GetValue().ToInt(&size);
                    mergedBlocks = STLToBlocksInt(filename, size);

                    MergeThreaded(mergedBlocks, opts, isStable -> GetValue(), [&progBar](int progress)
                                                    {
                                                        return !progBar.Update(progress);
                                                    });

                    progBar.Update(100);
                };

                // Count number of each block type
                std::vector <int> blockTotals = CountBlocks(mergedBlocks);

                // Populate Bill of Materials
                PopulateBOM(opts, blockTotals);
                CalculateTotalCost(mergedBlocks);

                // Update rendering state machine
                canvas -> noRender = false;
                canvas -> needsRendered = false;
                canvas -> stlRender = false;
                canvas -> blockRender = true;
                canvas -> zoom = false;
                
                // Make blocks for rendering
                canvas -> MakeShape(canvas -> vtx, canvas -> nom, canvas -> col, mergedBlocks, vizSlider -> GetValue(), 1);
                Refresh(true);
            }
            else
            {
                DisplayErrorMessage("Please enter a voxel size. Default is 10 ");
            }
        }
    }
    else
    {
        DisplayErrorMessage("Please select a STL file to convert.");
    }
    canvas -> SetFocus();
}

void MainWindow::OnExportClicked(wxCommandEvent &evt)
{
    // Exports the BOM
    wxFileDialog *SaveDialog = new wxFileDialog(
		this, _("Save Bill of Materials As _?"), wxEmptyString, wxEmptyString,
		_("Text File (*.txt)|*.txt"),
		wxFD_SAVE | wxFD_OVERWRITE_PROMPT, wxDefaultPosition);

	// Creates a Save Dialog 
	if (SaveDialog -> ShowModal() == wxID_OK) // If the user clicked "OK"
	{
		CurrentPath = SaveDialog -> GetPath();
        wxString extension = wxFileName(CurrentPath).GetExt();
        if (extension != "txt") 
        {
            CurrentPath += ".txt";
        }
		// set the path of our current document to the file the user chose to save under
        std::vector <int> totals = CountBlocks(mergedBlocks);
		SaveBOM(CurrentPath, totals);
	}
    // Save blocks and bill of materials
    std::cout << "Exporting bill of materials to " << CurrentPath << std::endl;

	// Clean up after ourselves
	SaveDialog->Destroy();
}

void MainWindow::OnSliderScroll(wxScrollEvent &event)
{
    canvas -> MakeShape(canvas -> vtx, canvas -> nom, canvas -> col, mergedBlocks, vizSlider -> GetValue(), 2);
    Refresh(true);
}

void MainWindow::ClearWindow()
{
    std::cout << "Clearing window...\n";

    canvas    -> Clear();
    stlInFile -> Clear();
    filename.Clear();

    // Force 1x1 blocks to always be an option
    oneByOne   -> SetValue(true);
    oneByOne   -> Enable(false);
    oneByTwo   -> SetValue(false);
    oneByThree -> SetValue(false);
    oneByFour  -> SetValue(false);
    twoByTwo   -> SetValue(false);
    twoByThree -> SetValue(false);
    twoByFour  -> SetValue(false);
    L_Shaped   -> SetValue(false);

    costBlock1x1 -> Clear(); 
    costBlock1x2 -> Clear();
    costBlock1x3 -> Clear();
    costBlock1x4 -> Clear();
    costBlock2x2 -> Clear();
    costBlock2x3 -> Clear();
    costBlock2x4 -> Clear();
    costBlockL   -> Clear();

    costBlock1x1 -> AppendText("1.00"); 
    costBlock1x2 -> AppendText("1.00");
    costBlock1x3 -> AppendText("1.00");
    costBlock1x4 -> AppendText("1.00");
    costBlock2x2 -> AppendText("1.00");
    costBlock2x3 -> AppendText("1.00");
    costBlock2x4 -> AppendText("1.00");
    costBlockL   -> AppendText("1.00");

    ClearBOM();
}

void MainWindow::PopulateBOM(std::vector <Block> opts, std::vector <int> quantities)
{
    // Clear bill of materials
    ClearBOM();

    std::vector <std::string> chosenBlocks;
    std::vector <int> indices;

    std::cout << "Generating Bill of Materials...\n";

    chosenBlocks.push_back("1x1 Blocks");
    indices.push_back(0);

    if(oneByTwo -> GetValue() == true)
    {  
        chosenBlocks.push_back("1x2 Blocks");
        indices.push_back(1);
    }
    if(oneByThree -> GetValue() == true)
    {
        chosenBlocks.push_back("1x3 Blocks");
        indices.push_back(2);
    }
    if(oneByFour -> GetValue() == true)
    {
        chosenBlocks.push_back("1x4 Blocks");
        indices.push_back(3);
    }
    if(twoByTwo -> GetValue() == true)
    {
        chosenBlocks.push_back("2x2 Blocks");
        indices.push_back(4);
    }
    if(twoByThree -> GetValue() == true)
    {
        chosenBlocks.push_back("2x3 Blocks"); 
        indices.push_back(5);
    }
    if(twoByFour -> GetValue() == true)
    {  
        chosenBlocks.push_back("2x4 Blocks");
        indices.push_back(6);
    }
    if(L_Shaped -> GetValue() == true)
    {
        chosenBlocks.push_back("L-Shaped Blocks");
        indices.push_back(7);
    }

    // Populate bill of materials
    switch(chosenBlocks.size())
    {
        case 1:
            block1 -> AppendText(chosenBlocks[0]);
            break;
        case 2:
            block1 -> AppendText(chosenBlocks[0]);
            block2 -> AppendText(chosenBlocks[1]);
            break;
        case 3:
            block1 -> AppendText(chosenBlocks[0]);
            block2 -> AppendText(chosenBlocks[1]);
            block3 -> AppendText(chosenBlocks[2]);
            break;
        case 4:
            block1 -> AppendText(chosenBlocks[0]);
            block2 -> AppendText(chosenBlocks[1]);
            block3 -> AppendText(chosenBlocks[2]);
            block4 -> AppendText(chosenBlocks[3]);
            break;
        case 5:
            block1 -> AppendText(chosenBlocks[0]);
            block2 -> AppendText(chosenBlocks[1]);
            block3 -> AppendText(chosenBlocks[2]);
            block4 -> AppendText(chosenBlocks[3]);
            block5 -> AppendText(chosenBlocks[4]);
            break;
        case 6:
            block1 -> AppendText(chosenBlocks[0]);
            block2 -> AppendText(chosenBlocks[1]);
            block3 -> AppendText(chosenBlocks[2]);
            block4 -> AppendText(chosenBlocks[3]);
            block5 -> AppendText(chosenBlocks[4]);
            block6 -> AppendText(chosenBlocks[5]);
            break;
        case 7:
            block1 -> AppendText(chosenBlocks[0]);
            block2 -> AppendText(chosenBlocks[1]);
            block3 -> AppendText(chosenBlocks[2]);
            block4 -> AppendText(chosenBlocks[3]);
            block5 -> AppendText(chosenBlocks[4]);
            block6 -> AppendText(chosenBlocks[5]);
            block7 -> AppendText(chosenBlocks[6]);
            break;
        case 8:
            block1 -> AppendText(chosenBlocks[0]);
            block2 -> AppendText(chosenBlocks[1]);
            block3 -> AppendText(chosenBlocks[2]);
            block4 -> AppendText(chosenBlocks[3]);
            block5 -> AppendText(chosenBlocks[4]);
            block6 -> AppendText(chosenBlocks[5]);
            block7 -> AppendText(chosenBlocks[6]);
            block8 -> AppendText(chosenBlocks[7]);
            break;
        default:
            break;
    }

    // Populate quantity


    switch(chosenBlocks.size())
    {
        case 1:
            block1_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[0]]));
            break; 
        case 2: 
            block1_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[0]]));
            block2_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[1]]));
            break; 
        case 3: 
            block1_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[0]]));
            block2_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[1]]));
            block3_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[2]]));
            break; 
        case 4: 
            block1_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[0]]));
            block2_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[1]]));
            block3_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[2]]));
            block4_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[3]]));
            break; 
        case 5: 
            block1_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[0]]));
            block2_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[1]]));
            block3_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[2]]));
            block4_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[3]]));
            block5_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[4]]));
            break; 
        case 6: 
            block1_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[0]]));
            block2_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[1]]));
            block3_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[2]]));
            block4_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[3]]));
            block5_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[4]]));
            block6_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[5]]));
            break; 
        case 7: 
            block1_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[0]]));
            block2_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[1]]));
            block3_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[2]]));
            block4_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[3]]));
            block5_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[4]]));
            block6_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[5]]));
            block7_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[6]]));
            break; 
        case 8: 
            block1_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[0]]));
            block2_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[1]]));
            block3_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[2]]));
            block4_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[3]]));
            block5_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[4]]));
            block6_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[5]]));
            block7_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[6]]));
            block8_Qty -> AppendText(wxString::Format(wxT("%i"), quantities[indices[7]]));
            break;
        default:
            break;
    }
}

void MainWindow::ClearBOM()
{
    block1 -> Clear();
    block2 -> Clear();
    block3 -> Clear();
    block4 -> Clear();
    block5 -> Clear();
    block6 -> Clear();
    block7 -> Clear();
    block8 -> Clear();

    block1_Qty -> Clear();
    block2_Qty -> Clear();
    block3_Qty -> Clear();
    block4_Qty -> Clear();
    block5_Qty -> Clear();
    block6_Qty -> Clear();
    block7_Qty -> Clear();
    block8_Qty -> Clear();

    total -> Clear();
}

void MainWindow::SaveBlocks(const wxString filename, std::vector <std::vector <Block>> &blocks)
{
    std::string strFileName = std::string(filename.mb_str());
    std::ofstream outfile(strFileName, std::ofstream::out);

    if (outfile.is_open())
    {
        // write each block to the file
        for (auto &layer : blocks)
        {
            for (auto &block : layer)
            {
                outfile << block.x << "," << block.y << "," << block.z << "," << block.GetSizeX() << "," << block.GetSizeY() << "," << block.cost << "," << block.isStable << ",";
            }
            outfile << std::endl;
        }

        outfile.close();
        std::cout << "Blocks saved to " << filename << std::endl;
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void MainWindow::SaveBOM(const wxString filename, std::vector <int> &totals)
{
    std::vector <wxString> bom;
    std::vector <wxString> quantities;

    if(block1 -> GetValue() != "" && block1_Qty -> GetValue() != "")
    {  
        bom.push_back(block1 -> GetValue());
        quantities.push_back(block1_Qty -> GetValue());
    }
    if(block2 -> GetValue() != "" && block1_Qty -> GetValue() != "")
    {  
        bom.push_back(block2 -> GetValue());
        quantities.push_back(block2_Qty -> GetValue());
    }
    if(block3 -> GetValue() != "" && block1_Qty -> GetValue() != "")
    {  
        bom.push_back(block3 -> GetValue());
        quantities.push_back(block3_Qty -> GetValue());
    }
    if(block4 -> GetValue() != "" && block1_Qty -> GetValue() != "")
    {  
        bom.push_back(block4 -> GetValue());
        quantities.push_back(block4_Qty -> GetValue());
    }
    if(block5 -> GetValue() != "" && block1_Qty -> GetValue() != "")
    {  
        bom.push_back(block5 -> GetValue());
        quantities.push_back(block5_Qty -> GetValue());
    }
    if(block6 -> GetValue() != "" && block1_Qty -> GetValue() != "")
    {  
        bom.push_back(block6 -> GetValue());
        quantities.push_back(block6_Qty -> GetValue());
    }
    if(block7 -> GetValue() != "" && block1_Qty -> GetValue() != "")
    {  
        bom.push_back(block7 -> GetValue());
        quantities.push_back(block7_Qty -> GetValue());
    }
    if(block8 -> GetValue() != "" && block8_Qty -> GetValue() != "")
    {  
        bom.push_back(block8 -> GetValue());
        quantities.push_back(block8_Qty -> GetValue());
    }

    bom.push_back("Total Cost");
    quantities.push_back(total -> GetValue());

    // Save bill of materials as txt file
    std::string strFileName = std::string(filename.mb_str());
    std::ofstream outfile(strFileName, std::ofstream::out);
    if (outfile.is_open())
    {
        // write block name and quantity to file
        for (size_t i = 0; i < bom.size(); i++)
        {
            outfile << bom[i] << "," << quantities[i] << std::endl;
        }

        outfile.close();
        std::cout << "Bill of Materials saved to " << filename << std::endl;
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}

void MainWindow::Import(wxString filename)
{
    // Import blocks
    bool success;
    std::vector <std::vector <Block>> imported;
    std::string strFileName = std::string(filename.mb_str());
    std::ifstream infile(strFileName, std::ifstream::in);
    if (infile.is_open())
    {
        try
        {
            std::string line;
            while (std::getline(infile, line))
            {
                std::vector<Block> layer;
                std::stringstream ss(line);
                std::string item;

                while (std::getline(ss, item, ','))
                {
                    int bx = std::stoi(item);
                    std::getline(ss, item, ',');
                    int by = std::stoi(item);
                    std::getline(ss, item, ',');
                    int bz = std::stoi(item);
                    std::getline(ss, item, ',');
                    int bsize_x = std::stoi(item);
                    std::getline(ss, item, ',');
                    int bsize_y = std::stoi(item);
                    std::getline(ss, item, ',');
                    float bcost = std::stof(item);
                    std::getline(ss, item, ',');
                    bool bStable = std::stof(item);

                    Block block(bx, by, bz, bsize_x, bsize_y, bcost);
                    block.isStable = bStable;

                    layer.push_back(block);
                }

                imported.push_back(layer);
            }
            success = true;
            std::cout << "Blocks imported from " << filename << std::endl;
        }

        catch (const std::exception& ex)
        {
            std::cerr << "Exception: " << ex.what() << std::endl;
            success = false;
        }

        infile.close();
    }
    else
    {
        std::cerr << "Unable to open file: " << filename << std::endl;
        success = false;
    }

    if(success)
    {
        // Update rendering state machine
        canvas -> noRender = false;
        canvas -> needsRendered = false;
        canvas -> stlRender = false;
        canvas -> blockRender = true;
        canvas -> zoom = false;
        
        // Make blocks for rendering
        canvas -> MakeShape(canvas -> vtx, canvas -> nom, canvas -> col, imported, vizSlider -> GetValue(), 1);
        Refresh(true);
    }
    else
    {
        DisplayErrorMessage("Attempting to import unsupported file. Please ensure you are opening a saved block file only.");
    }
}

void MainWindow::DisplayErrorMessage(std::string msg)
{
    wxAboutDialogInfo errorMsg;
    errorMsg.SetName(_("Error"));
    errorMsg.SetDescription(_(msg));

    wxAboutBox(errorMsg);
}

std::vector <Block> MainWindow::GetBlockOptions()
{
    std::vector <Block> BlockOptions;

    double cost1x1;
    costBlock1x1 -> GetValue().ToDouble(&cost1x1);
    Block oneByOneBlock(1, 1, cost1x1);
    BlockOptions.push_back(oneByOneBlock);

    if (oneByTwo->GetValue() == true)
    {
        double cost1x2;
        costBlock1x2 -> GetValue().ToDouble(&cost1x2);

        Block oneByTwoBlock(1, 2, cost1x2);
        Block twoByOneBlock(2, 1, cost1x2);
        BlockOptions.push_back(oneByTwoBlock);
        BlockOptions.push_back(twoByOneBlock);
    }
    if (oneByThree->GetValue() == true)
    {
        double cost1x3;
        costBlock1x3 -> GetValue().ToDouble(&cost1x3);
        
        Block oneByThreeBlock(1, 3, cost1x3);
        Block threeByOneBlock(3, 1, cost1x3);
        BlockOptions.push_back(oneByThreeBlock);
        BlockOptions.push_back(threeByOneBlock);
    }
    if (oneByFour->GetValue() == true)
    {
        double cost1x4;
        costBlock1x4 -> GetValue().ToDouble(&cost1x4);
        
        Block oneByFourBlock(1, 4, cost1x4);
        Block fourByOneBlock(4, 1, cost1x4);
        BlockOptions.push_back(oneByFourBlock);
        BlockOptions.push_back(fourByOneBlock);
    }
    if (twoByTwo->GetValue() == true)
    {
        double cost2x2;
        costBlock2x2 -> GetValue().ToDouble(&cost2x2);
        
        Block twoByTwoBlock(2, 2, cost2x2);
        BlockOptions.push_back(twoByTwoBlock);
    }
    if (twoByThree->GetValue() == true)
    {
        double cost2x3;
        costBlock2x3 -> GetValue().ToDouble(&cost2x3);
        
        Block twoByThreeBlock(2, 3, cost2x3);
        Block threeByTwoBlock(3, 2, cost2x3);
        BlockOptions.push_back(twoByThreeBlock);
        BlockOptions.push_back(threeByTwoBlock);
    }
    if (twoByFour->GetValue() == true)
    {
        double cost2x4;
        costBlock2x4 -> GetValue().ToDouble(&cost2x4);
        
        Block twoByFourBlock(2, 4, cost2x4);
        Block fourByTwoBlock(4, 2, cost2x4);
        BlockOptions.push_back(twoByFourBlock);
        BlockOptions.push_back(fourByTwoBlock);
    }
    if (L_Shaped->GetValue() == true)
    {
        double costL;
        costBlockL -> GetValue().ToDouble(&costL);
        for (int i = 0; i < 2; ++i)
        {
            for (int j = 0; j < 2; ++j)
            {
                Block l_block(2, 2, costL);
                l_block.SetBlockElementEmpty(i, j);
                BlockOptions.push_back(l_block);
            }
        }
    }
    return BlockOptions;
}

std::vector <int> MainWindow::CountBlocks(std::vector <std::vector <Block>> blocks)
{
    std::vector <int> blockTotals(8, 0);
    
    for (int layer = 0; layer < blocks.size(); layer++) // iterate through layers
    {
        for (auto& b : blocks[layer]) // iterate through blocks in each layer 
        {
            // ------------------- 1 by 1 ------------------- //
            if (b.GetSizeX() == 1 && b.GetSizeY() == 1)
            {
                blockTotals[0] ++;
            }
            // ------------------- 2 by 1 ------------------- //
            else if ((b.GetSizeX() == 1 && b.GetSizeY() == 2) ||
                     (b.GetSizeX() == 2 && b.GetSizeY() == 1))
            {
                blockTotals[1] ++;
            }
            // ------------------- 3 by 1 ------------------- //
            else if ((b.GetSizeX() == 1 && b.GetSizeY() == 3) ||
                     (b.GetSizeX() == 3 && b.GetSizeY() == 1))
            {
                blockTotals[2] ++;
            }
            // ------------------- 4 by 1 ------------------- //
            else if ((b.GetSizeX() == 1 && b.GetSizeY() == 4) ||
                     (b.GetSizeX() == 4 && b.GetSizeY() == 1))
            {
                blockTotals[3] ++;
            }
            // ------------------- 2 by 2 ------------------- //
            else if (b.GetSizeX() == 2 && b.GetSizeY() == 2 && b.IsRect())
            {
                blockTotals[4] ++;
            }
            // ------------------- 2 by 3 ------------------- //
            else if ((b.GetSizeX() == 2 && b.GetSizeY() == 3) ||
                     (b.GetSizeX() == 3 && b.GetSizeY() == 2))
            {
                blockTotals[5] ++;
            }
            // ------------------- 2 by 4 ------------------- //
            if ((b.GetSizeX() == 2 && b.GetSizeY() == 4) ||
                     (b.GetSizeX() == 4 && b.GetSizeY() == 2))
            {
                blockTotals[6] ++;
            }
             // ------------------- 2 by 2 L ------------------- //
            if (b.GetSizeX() == 2 && b.GetSizeY() == 2 && !b.IsRect())
            {
                blockTotals[7] ++;
            }
        }
    }
    return blockTotals; 
}

void MainWindow::CalculateTotalCost(std::vector<std::vector<Block>> blocks)
{
    float cost;
    for(auto &layer : blocks)
    {
        cost += CalculateCost(layer);
    }

    total -> AppendText(wxString::Format(wxT("%.2f"), std::round(cost * 100) / 100));
}