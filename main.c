#include<stdio.h>
#include<stdlib.h>
#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>

#define debug_break(...) __builtin_trap()
#define Assert(Condition) if(Condition); else debug_break()

int main(void)
{
	Display* MyDisplay = XOpenDisplay(0);
	Assert(MyDisplay);
	Window RootWindow = DefaultRootWindow(MyDisplay);
	Assert(RootWindow);
	GLint Attributes[] = { GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None };
	XVisualInfo* VisualInfo = glXChooseVisual(MyDisplay, 0, Attributes);
	Assert(VisualInfo);
	Colormap MyColorMap = XCreateColormap(MyDisplay, RootWindow, VisualInfo->visual, AllocNone);

	XSetWindowAttributes SWA = {0};
	SWA.colormap = MyColorMap;
	SWA.event_mask = ExposureMask | KeyPressMask;
	Window MainWindow = XCreateWindow(MyDisplay, RootWindow, 0, 0, 800, 600, 0, VisualInfo->depth, InputOutput, VisualInfo->visual, CWColormap | CWEventMask, &SWA);
	XMapWindow(MyDisplay, MainWindow);
	XStoreName(MyDisplay, MainWindow, "Title");

	GLXContext ContextGL = glXCreateContext(MyDisplay, VisualInfo, 0, GL_TRUE);
	glXMakeCurrent(MyDisplay, MainWindow, ContextGL);

	glClearColor(0.7, 0.3, 0.5, 1.0);
	glViewport(0, 0, 800, 600);

	for(;;)
	{
		XEvent MyEvent;
		XNextEvent(MyDisplay, &MyEvent);
		if(MyEvent.type == KeyPress)
		{
			break;
		}

		if(MyEvent.type == Expose)
		{
			glClear(GL_COLOR_BUFFER_BIT);
			glXSwapBuffers(MyDisplay, MainWindow);
		}
	}
	puts("All good");
	return 0;
}
