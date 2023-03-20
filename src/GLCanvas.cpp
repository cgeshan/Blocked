#include "wx/wx.h"
#include "GLCanvas.h"

GLCanvas::GLCanvas(wxPanel *parent, 
                   wxWindowID id,
                   const wxPoint &pos, 
                   const wxSize &size,
                   long style, 
                   const wxString &name) 
    : wxGLCanvas(parent, id, nullptr, pos, size, 
                 style | wxFULL_REPAINT_ON_RESIZE, name)
{
                        initialized = false;

                        timer = new wxTimer(this, wxID_ANY);
                        timer -> Start(1);

}

GLCanvas::~GLCanvas()
{
    // Destructor;
}

void GLCanvas::OnIdle(wxIdleEvent &event)
{
    Refresh();
}

void GLCanvas::OnEraseBackground(wxEraseEvent &event)
{
    // Empty function to prevent flickering
}  

void GLCanvas::OnTimer(wxTimerEvent &event)
{
    Refresh();
}