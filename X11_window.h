#pragma once
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <X11/Xlib.h>
#include <pthread.h>
#include <unistd.h>
#include "SyncDataArrivalAndPredicting.h"

unsigned long returnRGB(int r, int g, int b) {return b + (g<<8) + (r<<16);}

//bool gswitch = false;
bool global = false;
bool madeFirstDraw = false;

/* The window which contains the text. */
struct {
    int width;
    int height;
    char * text;
    int text_len;

    char * text2;
    int text_len2;

    /* X Windows related variables. */

    Display * display;
    int screen;
    Window root;
    Window window;
    GC gc;
    XFontStruct * font;
    unsigned long black_pixel;    
    unsigned long white_pixel;
    unsigned long red =returnRGB(255,0,0);
    unsigned long green =returnRGB(0,255,0);
    unsigned long blue =returnRGB(0,0,255);
}
text_box;

/* Connect to the display, set up the basic variables. */
static void x_connect ()
{
    text_box.display = XOpenDisplay (NULL);
    if (! text_box.display) {
        fprintf (stderr, "Could not open display.\n");
        exit (1);
    }
    text_box.screen = DefaultScreen (text_box.display);
    text_box.root = RootWindow (text_box.display, text_box.screen);
    text_box.black_pixel = BlackPixel (text_box.display, text_box.screen);
    text_box.white_pixel = WhitePixel (text_box.display, text_box.screen);
}

/* Create the window. */
static void create_window ()
{
    text_box.width = 200;
    text_box.height = 200;
    text_box.window = 
        XCreateSimpleWindow (text_box.display,
                             text_box.root,
                             1, /* x */
                             1, /* y */
                             text_box.width,
                             text_box.height,
                             0, /* border width */
                             text_box.black_pixel, /* border pixel */
                             text_box.green  /* background */);
    XSelectInput (text_box.display, text_box.window,
                  ExposureMask);
    XMapWindow (text_box.display, text_box.window);



    
    
}

/* Set up the GC (Graphics Context). */
static void set_up_gc ()
{
    text_box.screen = DefaultScreen (text_box.display);
    text_box.gc = XCreateGC (text_box.display, text_box.window, 0, 0);
    XSetBackground (text_box.display, text_box.gc, text_box.white_pixel); 
    XSetForeground (text_box.display, text_box.gc, text_box.black_pixel); 

    // XClearWindow (text_box.display, text_box.window);
    // XDrawString (text_box.display, text_box.window, text_box.gc,
    //              86, 155, text_box.text, text_box.text_len);
    // XSync(text_box.display, false);
}

/* Set up the text font. */
static void set_up_font ()
{
    const char * fontname = "-*-helvetica-*-r-*-*-14-*-*-*-*-*-*-*";
    text_box.font = XLoadQueryFont (text_box.display, fontname);
    /* If the font could not be loaded, revert to the "fixed" font. */
    if (! text_box.font) {
        fprintf (stderr, "unable to load font %s: using fixed\n", fontname);
        text_box.font = XLoadQueryFont (text_box.display, "fixed");
    }
    XSetFont (text_box.display, text_box.gc, text_box.font->fid);
}

/* Draw the window. */
static void draw_screen (SyncDataArrivalAndPredicting *syncSingleton)
{


    sleep(1);
    printf("entered draw_screen\n");

    //if(!madeFirstDraw)
    //{
        int x;
        int y;
        int direction;
        int ascent;
        int descent;
        XCharStruct overall;

        /* Centre the text in the middle of the box. */

        XTextExtents (text_box.font, text_box.text, text_box.text_len,
                    & direction, & ascent, & descent, & overall);
        x = (text_box.width - overall.width) / 2;
        y = text_box.height / 2 + (ascent - descent) / 2;

        XSetWindowBackground(text_box.display, text_box.window, text_box.green); 
        XClearWindow (text_box.display, text_box.window);
        printf("Right before XDrawString for green\n");
        XDrawString (text_box.display, text_box.window, text_box.gc,
                    x, y, text_box.text, text_box.text_len);
        XSync(text_box.display, false);
        //sleep(4);
        //madeFirstDraw = true;
    //}

    
    /* int x;
    int y;
    int direction;
    int ascent;
    int descent;
    XCharStruct overall;

    // Centre the text in the middle of the box.

    XTextExtents (text_box.font, text_box.text, text_box.text_len,
                  & direction, & ascent, & descent, & overall);
    x = (text_box.width - overall.width) / 2;
    y = text_box.height / 2 + (ascent - descent) / 2;
    XClearWindow (text_box.display, text_box.window);
    XDrawString (text_box.display, text_box.window, text_box.gc,
                 x, y, text_box.text, text_box.text_len); */

    

    
    //printf("Going to 4 secs sleep\n");
    //sleep(4);  
    //printf("Finished sleeping 4 in init()\n");

    //pthread_mutex_lock(&syncSingleton->condition_lock_color);

    int x2;
    int y2;
    //int direction;
    //int ascent;
    //int descent;
    //XCharStruct overall;

    /* Centre the text in the middle of the box. */

    XTextExtents (text_box.font, text_box.text, text_box.text_len,
                & direction, & ascent, & descent, & overall);
    x2 = (text_box.width - overall.width) / 2;
    y2 = text_box.height / 2 + (ascent - descent) / 2;

    printf("Here before wait line\n");
    if(!syncSingleton->condition_predicate_color)
    {
        pthread_cond_wait(&syncSingleton->condition_variable_color, &syncSingleton->condition_lock_color);

    
    //if(global)
      //  {
            printf("Here after wait line for color\n");
            
            //XSetBackground (display, gc, white_pixel); 
            //XSetForeground (display, gc, white_pixel);
            //XDrawString(display,window,gc,200,100,text,strlen(text));
            ////XClearWindow(display, window);
            ////XSync(display, false);


            XSetWindowBackground(text_box.display, text_box.window, text_box.red); 
            XClearWindow(text_box.display, text_box.window);
            printf("Right before XDrawString for red\n");
            XDrawString (text_box.display, text_box.window, text_box.gc,
                 x2, y2, text_box.text2, text_box.text_len2);
            


            XSync(text_box.display, false);
            //sleep(4);

            syncSingleton->condition_predicate_color = 1;
            pthread_mutex_unlock(&syncSingleton->condition_lock_color);


        //}
    }
    
    printf("Here before wait line for finished\n");
    pthread_cond_wait(&syncSingleton->condition_variable_finished, &syncSingleton->condition_lock_finished);
    pthread_mutex_unlock(&syncSingleton->condition_lock_finished);

    printf("exited draw_screen\n");
}

/* Loop over events. */

static void event_loop (SyncDataArrivalAndPredicting *syncSingleton)
{
    /*
    while (1) {
        XEvent e;
        XNextEvent (text_box.display, & e);
        if (e.type == Expose) {
            draw_screen ();
        }
    }
    */

    //sleep(4);
    //printf("Finished sleeping 4 in init()\n");
    
    //int quit = 0;

    //XClearWindow (text_box.display, text_box.window);
    //XDrawString (text_box.display, text_box.window, text_box.gc,
      //           86, 155, text_box.text, text_box.text_len);
    //XSync(text_box.display, false);

    printf("syncSingleton->condition_predicate_color is: %d\n",syncSingleton->condition_predicate_color);

    int x2;
    int y2;
    int direction;
    int ascent;
    int descent;
    XCharStruct overall;

    /* Centre the text in the middle of the box. */

    

    //while(!global)
    //while(1)
    //{
        printf("Now entered while(!global) loop\n");
        XEvent e;
        //XNextEvent (text_box.display, & e);
        //XPending (text_box.display);
        draw_screen (syncSingleton);


        // if (e.type == Expose) {
        //     printf("event type: expose\n");
        //     draw_screen (syncSingleton);
        // }


        //if(global) exit(0);
        
    //}



}

void *init15(void *arg) 
{
    text_box.text = "Pure ballistic";
    text_box.text_len = strlen (text_box.text);

    text_box.text2 = "Not pure ballistic";
    text_box.text_len2 = strlen (text_box.text2);

    SyncDataArrivalAndPredicting *syncSingleton = (SyncDataArrivalAndPredicting *) arg;

    x_connect ();
    create_window ();
    set_up_gc ();
    set_up_font ();
    event_loop (syncSingleton);
    return 0;
}


/*
int main()
{
   pthread_t thread1;
   SyncDataArrivalAndPredicting *syncSingleton = new SyncDataArrivalAndPredicting();
   pthread_mutex_init(&syncSingleton->condition_lock_color, NULL);
   pthread_cond_init(&syncSingleton->condition_variable_color, NULL);

   pthread_mutex_init(&syncSingleton->condition_lock_finished, NULL);
   pthread_cond_init(&syncSingleton->condition_variable_finished, NULL);


   pthread_create( &thread1, NULL, init15, (void*) syncSingleton);
   
   
   sleep(4);
   printf("Signaling color\n");
   pthread_cond_signal(&syncSingleton->condition_variable_color);
   pthread_mutex_unlock(&syncSingleton->condition_lock_color);

   printf("At this stage window should change color\n");

   sleep(4);
   
   

   printf("Signaling \"finished\"\n");
   pthread_cond_signal(&syncSingleton->condition_variable_finished);
   pthread_mutex_unlock(&syncSingleton->condition_lock_finished);

   pthread_join(thread1, NULL);
   return 0;
}
*/