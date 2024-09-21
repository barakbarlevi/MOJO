#pragma once
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <stdlib.h>
//#include <thread>
#include <pthread.h>

class WindowObject {
    public:
    
    Display *dis;
    int screen;
    Window win;
    GC gc;
    XEvent event;
    KeySym key;
    char text[255];
    unsigned long black, white, red, green;

    

    void init();
    void close();
    void draw();
    void startWindow();

    /*
    std::thread threadstartWindow() {
        return std::thread([=]{startWindow();});
    }
    */

    void setWindowRed();
    unsigned long RGB(int r, int g, int b); // XXXX move to utils...

};
