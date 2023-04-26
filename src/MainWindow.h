/* Author: Connor Geshan */

#ifndef MAINWINDOW_IS_INCLUDED
#define MAINWINDOW_IS_INCLUDED

#ifdef WIN32 
    // #define WIN32_LEAN_AND_MEAN
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
#include "wx/filename.h"
#include <wx/grid.h>

#include "GLCanvas.h"
#include "Block.h"

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

    // STL controls
    wxStaticText *stlInTextLabel;
    wxTextCtrl *stlInFile;
    wxButton *fileExplorerButton;

    // Block render height sliders
    wxSlider *vizSlider;

    // Voxel options
    wxStaticText *voxelSizeLabel;
    wxTextCtrl *voxelSize;

    // Stabilize options
    wxStaticText *stabilizeLabel;
    wxCheckBox *isStable;

    // Block options
    wxStaticText *blockOptsLabel;

    wxCheckBox *oneByOne; 
    wxCheckBox *oneByTwo; 
    wxCheckBox *oneByThree;
    wxCheckBox *oneByFour; 
    wxCheckBox *twoByTwo;
    wxCheckBox *twoByThree; 
    wxCheckBox *twoByFour;
    wxCheckBox *L_Shaped;

    // Cost options
    wxStaticText *costLabel1;
    wxStaticText *costLabel2;
    wxStaticText *costLabel3;
    wxStaticText *costLabel4;
    wxStaticText *costLabel5;
    wxStaticText *costLabel6;
    wxStaticText *costLabel7;
    wxStaticText *costLabel8;
    wxStaticText *totalCost;

    wxTextCtrl *costBlock1x1;
    wxTextCtrl *costBlock1x2;
    wxTextCtrl *costBlock1x3;
    wxTextCtrl *costBlock1x4;
    wxTextCtrl *costBlock2x2;
    wxTextCtrl *costBlock2x3;
    wxTextCtrl *costBlock2x4;
    wxTextCtrl *costBlockL;
    wxTextCtrl *total;
    float defaultCost;

    // Save, reset, build, and close buttons
    wxButton *buildButton;
    wxButton *resetButton;
    wxButton *saveButton;
    wxButton *closeButton;

    // Bill of Materials
    wxStaticText *BOMLabel;
    wxStaticText *blockType1Label;
    wxStaticText *blockType2Label;
    wxStaticText *blockQty1Label;
    wxStaticText *blockQty2Label;

    wxTextCtrl *block1;
    wxTextCtrl *block2;
    wxTextCtrl *block3;
    wxTextCtrl *block4;
    wxTextCtrl *block5;
    wxTextCtrl *block6;
    wxTextCtrl *block7;
    wxTextCtrl *block8;
    wxTextCtrl *block1_Qty;
    wxTextCtrl *block2_Qty;
    wxTextCtrl *block3_Qty;
    wxTextCtrl *block4_Qty;
    wxTextCtrl *block5_Qty;
    wxTextCtrl *block6_Qty;
    wxTextCtrl *block7_Qty;
    wxTextCtrl *block8_Qty;

    wxButton *exportButton;
protected:
    void OnNew(wxCommandEvent &evt);
    void OnOpen(wxCommandEvent &evt);
    void OnSave(wxCommandEvent &evt);
    void OnSaveClicked(wxCommandEvent &evt);
    void OnExit(wxCommandEvent &evt);
    void OnCloseClicked(wxCommandEvent &evt);
    void OnResetClicked(wxCommandEvent &evt);

    void OnChooseFileClicked(wxCommandEvent &evt);
    void OnVoxelizeClicked(wxCommandEvent &evt);
    void OnExportClicked(wxCommandEvent &evt);
    void OnSliderScroll(wxScrollEvent& event);
public:
    const int WIN_WIDTH = 900, WIN_HEIGHT = 700;
    wxString filename;
    wxFileName STL_PATH;
    std::vector <std::vector <Block>> mergedBlocks;

    MainWindow(wxWindow *parent, const wxString &title);
    virtual ~MainWindow();

    void CreateWindowLayout();
    void CreateWindowControls();
    void AddControlsToSizers();
    void BindEventHandlers();

    void ClearWindow();
    void PopulateBOM(std::vector <Block> opts, std::vector <int> blockTotals);
    void ClearBOM();
    void SaveBlocks(const wxString filename, std::vector <std::vector <Block>> &blocks);
    void SaveBOM(const wxString filename, std::vector <int> &totals);
    void Import(wxString filename);
    void DisplayErrorMessage(std::string msg);

    std::vector <Block> GetBlockOptions();
    std::vector <int> CountBlocks(std::vector <std::vector <Block>> blocks); 
    void CalculateTotalCost(std::vector <std::vector <Block>> blocks);
    
    DECLARE_EVENT_TABLE()
};

#endif