#include "wx/wx.h"
#include "GLCanvas.h"

BEGIN_EVENT_TABLE(GLCanvas, wxGLCanvas)
    EVT_MOTION(GLCanvas::mouseMoved)
    EVT_LEFT_DOWN(GLCanvas::mouseDown)
    EVT_LEFT_UP(GLCanvas::mouseReleased)
    EVT_RIGHT_DOWN(GLCanvas::rightClick)
    EVT_LEAVE_WINDOW(GLCanvas::mouseLeftWindow)
    EVT_SIZE(GLCanvas::resized)
    EVT_KEY_DOWN(GLCanvas::keyPressed)
    EVT_KEY_UP(GLCanvas::keyReleased)
    EVT_MOUSEWHEEL(GLCanvas::mouseWheelMoved)
    EVT_PAINT(GLCanvas::render)
END_EVENT_TABLE()

GLCanvas:: GLCanvas(wxPanel* parent, int* args) :
    wxGLCanvas(parent, wxID_ANY, args, wxDefaultPosition, wxDefaultSize, wxFULL_REPAINT_ON_RESIZE)
{
	glContext = new wxGLContext(this);
    // prepare a simple cube to demonstrate 3D render
    // source: http://www.opengl.org/resources/code/samples/glut_examples/examples/cube.c
    v[0][0] = v[1][0] = v[2][0] = v[3][0] = -1;
    v[4][0] = v[5][0] = v[6][0] = v[7][0] = 1;
    v[0][1] = v[1][1] = v[4][1] = v[5][1] = -1;
    v[2][1] = v[3][1] = v[6][1] = v[7][1] = 1;
    v[0][2] = v[3][2] = v[4][2] = v[7][2] = 1;
    v[1][2] = v[2][2] = v[5][2] = v[6][2] = -1;    

    // To avoid flashing on MSW
    SetBackgroundStyle(wxBG_STYLE_CUSTOM);
}

GLCanvas::~GLCanvas()
{
    delete glContext;
}

void GLCanvas::resized(wxSizeEvent &evt)
{
    Refresh();
}

int GLCanvas::getWidth()
{
    return GetSize().x;
}

int GLCanvas::getHeight()
{
    return GetSize().y;
}

void GLCanvas::render(wxPaintEvent &evt)
{
    if(!IsShown()) return;
    
    wxGLCanvas::SetCurrent(*glContext);
    wxPaintDC(this); // only to be used in paint events. use wxClientDC to paint outside the paint event
	
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
    
    
    // ------------- draw some 3D ----------------
    prepare3DViewport(getWidth()/2,0,getWidth(), getHeight());
    glLoadIdentity();
	
    glColor4f(0,0,1,1);
    glTranslatef(0,0,-5);
    glRotatef(50.0f, 0.0f, 1.0f, 0.0f);
    
    glColor4f(1, 0, 0, 1);
    for (int i = 0; i < 6; i++)
    {
        glBegin(GL_LINE_STRIP);
        glVertex3fv(&v[faces[i][0]][0]);
        glVertex3fv(&v[faces[i][1]][0]);
        glVertex3fv(&v[faces[i][2]][0]);
        glVertex3fv(&v[faces[i][3]][0]);
        glVertex3fv(&v[faces[i][0]][0]);
        glEnd();
    }
    
    glFlush();
    SwapBuffers();
}

void GLCanvas::prepare3DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{
 glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Black Background
    glClearDepth(1.0f);	// Depth Buffer Setup
    glEnable(GL_DEPTH_TEST); // Enables Depth Testing
    glDepthFunc(GL_LEQUAL); // The Type Of Depth Testing To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	
    glEnable(GL_COLOR_MATERIAL);
	
    glViewport(topleft_x, topleft_y, bottomrigth_x-topleft_x, bottomrigth_y-topleft_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	
    float ratio_w_h = (float)(bottomrigth_x-topleft_x)/(float)(bottomrigth_y-topleft_y);
    gluPerspective(45 /*view angle*/, ratio_w_h, 0.1 /*clip close*/, 200 /*clip far*/);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

}

void GLCanvas::prepare2DViewport(int topleft_x, int topleft_y, int bottomrigth_x, int bottomrigth_y)
{

}

void GLCanvas::mouseMoved(wxMouseEvent &event)
{

}

void GLCanvas::mouseDown(wxMouseEvent &event)
{

}

void GLCanvas::mouseWheelMoved(wxMouseEvent &event)
{

}

void GLCanvas::mouseReleased(wxMouseEvent &event)
{

}

void GLCanvas::rightClick(wxMouseEvent &event)
{

}

void GLCanvas::mouseLeftWindow(wxMouseEvent &event)
{

}

void GLCanvas::keyPressed(wxKeyEvent &event)
{

}

void GLCanvas::keyReleased(wxKeyEvent &event)
{
    
}
