#include "WindowObject.h"


void WindowObject::init() {
    //dot.x=100;dot.y=100;
    dis=XOpenDisplay((char *)0);
    screen=DefaultScreen(dis);
    black=BlackPixel(dis,screen);
    white=WhitePixel(dis,screen);
    red=RGB(255,0,0);
    green=RGB(0,255,0);
    win=XCreateSimpleWindow(dis, DefaultRootWindow(dis), 0, 0, 300, 300, 5, green, green);

    

    XSetStandardProperties(dis, win, "Howdy", "Hi", None, NULL, 0, NULL);
    XSelectInput(dis, win, ExposureMask | ButtonPressMask | KeyPressMask);
    gc = XCreateGC(dis, win, 0, 0);
    //XSetBackground(dis,gc,blue);
    //XSetForeground(dis,gc,blue);
    XClearWindow(dis, win);
    XMapRaised(dis, win);
}

void WindowObject::close() {
    XFreeGC(dis, gc);
    XDestroyWindow(dis, win);
    XCloseDisplay(dis);
    exit(0);
}

void WindowObject::draw() {
    XClearWindow(dis, win);
}

unsigned long WindowObject::RGB(int r, int g, int b) {
    return b + (g<<8) + (r<<16);
}

void WindowObject::startWindow() {
 
    //while(1) {
        XNextEvent(dis, &event);
        if(event.type==Expose && event.xexpose.count==0) {
            draw();
        }
    //}
    
}



void WindowObject::setWindowRed() {
    XClearWindow(dis, win);
    XSetWindowBackground(dis, win, red); 
    
    //XNextEvent(dis, &event);
      //  if(event.type==Expose && event.xexpose.count==0) {
        //    draw();
       // }

    XClearWindow(dis, win);
}
