/* Author: Connor Geshan */

#include <vector>
#include <algorithm>
#include <stdio.h>
#include <stdint.h>
#include <time.h>

#include "wx/wx.h"

#include "GLCanvas.h"
#include "MainWindow.h"

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
    EVT_LEFT_DOWN(GLCanvas::LeftMouseDown)
    EVT_SIZE(GLCanvas::Resized)
    EVT_KEY_DOWN(GLCanvas::KeyPressed)
    EVT_PAINT(GLCanvas::Render)
END_EVENT_TABLE()

GLCanvas:: GLCanvas(wxPanel* parent, int* args) :
    wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE | wxWANTS_CHARS)
{
    std::cout << "Creating Panel...\n";
    glContext = new wxGLContext(this);   

    // Initialize rendering state machine
    noRender = true;
    stlRender = false;
    blockRender = false;
    needsRendered = false;
    zoom = false;

    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

GLCanvas::~GLCanvas()
{
    std::cout << "Closing glCanvas... \n";
    delete glContext;
}

int GLCanvas::GetWidth()
{
    return GetSize().x;
}

int GLCanvas::GetHeight()
{
    return GetSize().y;
}

void GLCanvas::Render(wxPaintEvent& WXUNUSED(event))
{
    // Ensure the glCanvas is visible
    if(!IsShown())
    {
        std::cout << "**ERROR** Window not shown...\n";
        return;
    }

    // Read in STL file if new file is chosen by the user
    if(needsRendered == true )
    {
        if(true != ReadBinarySTL(vtx, nom, STLPath))
        {
            printf("**Error** Unable to read STL\n");
        }
        for(int i = 0; i < vtx.size()/3; ++i)
        {
            GLCanvas::PushRGBA(col, .16, .26, .368, 1);
        }
        needsRendered = false;
    }

    // Begin rendering
    wxPaintDC(this); 
    wxGLCanvas::SetCurrent(*glContext);
    
    glClearColor(0.637f, 0.6370f, 0.6370f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    YsVec3 bbx[2];
    GetBoundingBox(bbx, vtx);

    YsVec3 viewTarget=(bbx[0] + bbx[1])/2.0;

    // Standard view angle for first render
    if(!zoom)
    {
        viewDist = (bbx[1] - bbx[0]).GetLength();
    }

    double hdg = 0.0;

    viewAngle *= Rkey;

    double aspect = (double)GetWidth()/(double)GetHeight();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, aspect, 0.1 * viewDist, 2.2 * viewDist);

    YsMatrix4x4 tfm;
    tfm.Translate(0, 0, -viewDist);
    tfm *= viewAngle;
    tfm.Translate(-viewTarget);

    // Works the same way >>
    tfm.Transpose();
    auto glTfm = tfm.GetArray();
    // Works the same way <<

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    GLfloat lightDir[]={0, 1/sqrt(2.0f), 1/sqrt(2.0f), 0};
    glLightfv(GL_LIGHT0, GL_POSITION, lightDir);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    glMultMatrixd(glTfm);
    
    if(12<=vtx.size())
    {
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
        glColorPointer(4, GL_FLOAT, 0, col.data());
        glVertexPointer(3, GL_FLOAT, 0, vtx.data());
        glNormalPointer(GL_FLOAT, 0, nom.data());
        if(stlRender)
        {
            glDrawArrays(GL_TRIANGLES, 0, vtx.size()/3);
        }
        else if(blockRender)
        {
            glDrawArrays(GL_QUADS, 0, vtx.size()/3);
        }
        glDisableClientState(GL_VERTEX_ARRAY);
        glDisableClientState(GL_NORMAL_ARRAY);
        glDisableClientState(GL_COLOR_ARRAY);
    }

    SwapBuffers();
}

void GLCanvas::Resized(wxSizeEvent &evt)
{
    std::cout << "Resizing Window...refreshing glCanvas\n";
    Refresh(true);
}

void GLCanvas::LeftMouseDown(wxMouseEvent &event)
{
    this -> SetFocus();
}

void GLCanvas::KeyPressed(wxKeyEvent &event)
{
    wxChar key = event.GetUnicodeKey();
    YsMatrix4x4 Rkey, viewangle;

    switch(event.GetKeyCode())
    {
        case WXK_ESCAPE:
            wxTheApp -> ExitMainLoop();
            return;
        
        case WXK_LEFT:
            Rkey.RotateXZ(PI/20.0);
            viewAngle *= Rkey;
            break;

        case WXK_RIGHT:
            Rkey.RotateXZ(-PI/20.0);
            viewAngle *= Rkey;
            break;

        case WXK_UP:
            Rkey.RotateZY(PI/20.0);
            viewAngle *= Rkey;
            break;

        case WXK_DOWN:
            Rkey.RotateZY(-PI/20.0);
            viewAngle *= Rkey;
            break;

        case 'Z':
            zoom = true;
            if(1.0 < viewDist)
			{
				viewDist /= 1.1;
			}
            break;

        case 'X':
            zoom = true;
            viewDist *= 1.1;
            break;

        default:
            event.Skip();
            return;
    }
    Refresh(true);
}

void GLCanvas::SetSTLPath(wxString filename)
{
    STLPath = filename;
}

void GLCanvas::PushXYZ(std::vector <float> &vtx, float x, float y, float z)
{
	vtx.push_back(x);
	vtx.push_back(y);
	vtx.push_back(z);
}

void GLCanvas::PushRGBA(std::vector <float> &col, float r, float g, float b, float a)
{
	col.push_back(r);
	col.push_back(g);
	col.push_back(b);
	col.push_back(a);
}

void GLCanvas::GetBoundingBox(YsVec3 bbx[2], const std::vector <float> &vtx)
{
	bbx[0] = YsVec3::Origin();
	bbx[1] = YsVec3::Origin();
	if(3 <= vtx.size())
	{
		bbx[0].Set(vtx[0], vtx[1], vtx[2]);
		bbx[1].Set(vtx[0], vtx[1], vtx[2]);

		for(int i = 3; i + 3 <= vtx.size(); i += 3)
		{
			bbx[0].Set(
				std::min<double>(bbx[0].x(), vtx[i + 0]),
				std::min<double>(bbx[0].y(), vtx[i + 1]),
				std::min<double>(bbx[0].z(), vtx[i + 2]));
			bbx[1].Set(
				std::max<double>(bbx[1].x(), vtx[i + 0]),
				std::max<double>(bbx[1].y(), vtx[i + 1]),
				std::max<double>(bbx[1].z(), vtx[i + 2]));
		}
	}
}

bool GLCanvas::ReadBinarySTL(std::vector <float> &vtx, std::vector <float> &nom, const char fName[])
{
	vtx.clear();
	nom.clear();
	col.clear();
    
    std::cout << "Reading STL...\n";
    FILE *ifp = fopen(fName, "rb");
	if(nullptr != ifp)
	{
		unsigned char buf[80];

		fread(buf, 1, 80, ifp);

		fread(buf, 1, 4, ifp);
		unsigned int nPlg = *(uint32_t *)buf;

		printf("%d polygons detected in file\n", nPlg);

		for(int i = 0; i < nPlg; i++)
		{
			if(50 == fread(buf, 1, 50, ifp))
			{
				float *tri = (float *)buf;
				PushXYZ(nom, tri[0] ,tri[1] ,tri[2]);
				PushXYZ(nom, tri[0] ,tri[1] ,tri[2]);
				PushXYZ(nom, tri[0] ,tri[1] ,tri[2]);

				PushXYZ(vtx, tri[3], tri[4], tri[5]);
				PushXYZ(vtx, tri[6], tri[7], tri[8]);
				PushXYZ(vtx, tri[9], tri[10], tri[11]);
			}
			else
			{
				break;
			}
		}
        std::cout << "STL imported, closing file\n";
		fclose(ifp);
		return true;
	}
	return false;
}

void GLCanvas::AddColor(std::vector <float> &col, float red, float green, float blue)
{
    for(int face = 0; face < 6; face++)
    {
        for(int edge = 0; edge < 4; edge++)
        {
            PushRGBA(col, red, green, blue, 1);
        }
    }
}
void GLCanvas::MakeBlock(std::vector <float> &vtx, std::vector <float> &nom, Block &b, float layerHeight)
{
    PushXYZ(vtx,            b.x,            b.y, b.z);
    PushXYZ(vtx, b.GetSizeX() + b.x,            b.y, b.z);
    PushXYZ(vtx, b.GetSizeX() + b.x, b.GetSizeY() + b.y, b.z);
    PushXYZ(vtx,            b.x, b.GetSizeY() + b.y, b.z);

    PushXYZ(nom, 0, 0, -1);
    PushXYZ(nom, 0, 0, -1);
    PushXYZ(nom, 0, 0, -1);
    PushXYZ(nom, 0, 0, -1);

    PushXYZ(vtx,            b.x,            b.y, b.z + layerHeight);
    PushXYZ(vtx, b.GetSizeX() + b.x,            b.y, b.z + layerHeight);
    PushXYZ(vtx, b.GetSizeX() + b.x, b.GetSizeY() + b.y, b.z + layerHeight);
    PushXYZ(vtx,            b.x, b.GetSizeY() + b.y, b.z + layerHeight);

    PushXYZ(nom, 0, 0, 1);
    PushXYZ(nom, 0, 0, 1);
    PushXYZ(nom, 0, 0, 1);
    PushXYZ(nom, 0, 0, 1);

    PushXYZ(vtx,            b.x,      b.y,               b.z);
    PushXYZ(vtx, b.GetSizeX() + b.x,      b.y,               b.z);
    PushXYZ(vtx, b.GetSizeX() + b.x,      b.y, b.z + layerHeight);
    PushXYZ(vtx,            b.x,      b.y, b.z + layerHeight);

    PushXYZ(nom, 0, -1, 0);
    PushXYZ(nom, 0, -1, 0);
    PushXYZ(nom, 0, -1, 0);
    PushXYZ(nom, 0, -1, 0);

    PushXYZ(vtx,            b.x, b.GetSizeY() + b.y,               b.z);
    PushXYZ(vtx, b.GetSizeX() + b.x, b.GetSizeY() + b.y,               b.z);
    PushXYZ(vtx, b.GetSizeX() + b.x, b.GetSizeY() + b.y, b.z + layerHeight);
    PushXYZ(vtx,            b.x, b.GetSizeY() + b.y, b.z + layerHeight);

    PushXYZ(nom, 0, 1, 0);
    PushXYZ(nom, 0, 1, 0);
    PushXYZ(nom, 0, 1, 0);
    PushXYZ(nom, 0, 1, 0);

    PushXYZ(vtx,      b.x,            b.y,               b.z);
    PushXYZ(vtx,      b.x, b.GetSizeY() + b.y,               b.z);
    PushXYZ(vtx,      b.x, b.GetSizeY() + b.y, b.z + layerHeight);
    PushXYZ(vtx,      b.x,            b.y, b.z + layerHeight);

    PushXYZ(nom, -1, 0, 0);
    PushXYZ(nom, -1, 0, 0);
    PushXYZ(nom, -1, 0, 0);
    PushXYZ(nom, -1, 0, 0);

    PushXYZ(vtx, b.GetSizeX() + b.x,            b.y,               b.z);
    PushXYZ(vtx, b.GetSizeX() + b.x, b.GetSizeY() + b.y,               b.z);
    PushXYZ(vtx, b.GetSizeX() + b.x, b.GetSizeY() + b.y, b.z + layerHeight);
    PushXYZ(vtx, b.GetSizeX() + b.x,            b.y, b.z + layerHeight);

    PushXYZ(nom, 1, 0, 0);
    PushXYZ(nom, 1, 0, 0);
    PushXYZ(nom, 1, 0, 0);
    PushXYZ(nom, 1, 0, 0);
}

void GLCanvas::MakeShape(std::vector <float> &vtx, std::vector <float> &nom, std::vector <float> &col, std::vector <std::vector <Block>> &blocks, int sliderValue, int state)
{
    vtx.clear();
	nom.clear();
	col.clear();

    float layerHeight = 1;

    srand(time(NULL));

    if (state == 1)
    {
        for(int layer = 0; layer < blocks.size(); layer++)
        {
            for(auto &b : blocks[layer])
            {
                float red = ((float)rand())/RAND_MAX;
                float green = ((float)rand())/RAND_MAX;
                float blue = ((float)rand())/RAND_MAX;

                if (!b.isStable)
                {
                    red = 1.0;
                    green = 0.0;
                    blue = 0.0;
                }

                if (b.IsRect())
                {
                    AddColor(col, red, green, blue);
                }
                else
                {
                    for (int i = 0; i < b.GetSizeX(); ++i)
                    {
                        for (int j = 0; j < b.GetSizeY(); ++j)
                        {
                            if (!b.GetOccupied(i, j))
                            {
                                continue;
                            }
                            AddColor(col, red, green, blue);
                        }
                    }
                }
            }
        }
    }

    vtx.clear();
	nom.clear();

    for(int layer = 0; layer < (double)blocks.size()*((double)sliderValue/100); layer++)
    {
        for(auto &b : blocks[layer])
        {
            if (b.IsRect())
            {
                MakeBlock(vtx, nom, b, layerHeight);
            }
            else
            {
                for (int i = 0; i < b.GetSizeX(); ++i)
                {
                    for (int j = 0; j < b.GetSizeY(); ++j)
                    {
                        if (!b.GetOccupied(i, j))
                        {
                            continue;
                        }
                        Block stud(b.x+i, b.y+j, b.z, 1, 1, 0);
                        MakeBlock(vtx, nom, stud, layerHeight);
                    }
                }
            }
        }
    }
}

void GLCanvas::Clear()
{
    std::cout << "Clearing the glCanvas...\n";
    vtx.clear();
    nom.clear();
    col.clear();
}