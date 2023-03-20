#ifndef GLCANVAS_IS_INCLUDED
#define GLCANVAS_IS_INCLUDED

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

#include "wx/wx.h"
#include "wx/glcanvas.h"


class GLCanvas : public wxGLCanvas 
{
private:
    bool initialized;

    wxTimer *timer;
    // DECLARE_EVENT_TABLE();
    
public:
    GLCanvas(wxPanel *parent, wxWindowID id = wxID_ANY, 
    const wxPoint &pos = wxDefaultPosition, 
    const wxSize &size = wxDefaultSize, long style = 0, 
    const wxString &name = "GLCanvas");

    ~GLCanvas();
    
    // GLvoid displayFPS(GLvoid);

    // Event handlers
    void OnIdle(wxIdleEvent &event);
    void OnEraseBackground(wxEraseEvent &event);
    void OnTimer(wxTimerEvent &event);

};

#endif 