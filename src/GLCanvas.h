/* Author: Connor Geshan */

#ifndef GLCANVAS_IS_INCLUDED
#define GLCANVAS_IS_INCLUDED

#include "wx/wx.h"
#include "wx/glcanvas.h"

#include "ysclass.h"
#include "Block.h"

class GLCanvas : public wxGLCanvas 
{
private:
    wxGLContext *glContext;
	wxString STLPath;

protected:
    
public:
    GLCanvas(wxPanel *parent, int *args);
    virtual ~GLCanvas();
    
    // Getters
	int GetWidth();
	int GetHeight();
    
	// Event handlers
	void Render(wxPaintEvent &evt);
    void Resized(wxSizeEvent &evt);
	void LeftMouseDown(wxMouseEvent &evt);
	void KeyPressed(wxKeyEvent &evt);

	// STL Helpers
	std::vector <float> vtx, nom, col;
	void SetSTLPath(wxString filename);

	// Rendering state machine
	bool noRender, stlRender, blockRender, zoom;
	
	// Rendering
	YsMatrix4x4 viewAngle;
	YsMatrix4x4 Rkey;
	YsMatrix4x4 tfm;
	const double PI = 3.14159265358979323;
	bool needsRendered;
	double viewDist;

	void PushXYZ(std::vector <float> &vtx, float x, float y, float z);
	void PushRGBA(std::vector <float> &col, float r, float g, float b, float a);
	void GetBoundingBox(YsVec3 bbx[2], const std::vector <float> &vtx);
	bool ReadBinarySTL(std::vector <float> &vtx, std::vector <float> &nom, const char fName[]);
	void MakeShape(std::vector <float> &vtx, std::vector <float> &nom, std::vector <float> &col, std::vector <std::vector <Block>> &blocks, int sliderValue, int state);
	void AddColor(std::vector <float> &col, float red, float green, float blue);
	void MakeBlock(std::vector <float> &vtx, std::vector <float> &nom,  Block &b, float layerHeight);
	void Clear();

    DECLARE_EVENT_TABLE()
};

#endif 
