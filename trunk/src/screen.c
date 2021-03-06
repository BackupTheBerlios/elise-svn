#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/cursorfont.h>

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "client.h"
#include "screen.h"

/* the function we use to free a screen object */
static void free_screen(list_data *screen) {
    free(screen);
}

static void setup_root_win(Display *display, screen_item *screen) {
    XSetWindowAttributes root_attr;
    Cursor cursor;
    
#ifdef DEBUG
    screen->normal_cursor = XCreateFontCursor(display, XC_right_ptr);
#else
    screen->normal_cursor = XCreateFontCursor(display, XC_left_ptr);
#endif
    screen->move_cursor   = XCreateFontCursor(display, XC_fleur);

    /* set the cursor for the root window */
    XDefineCursor(display, screen->root_win, cursor);
    
    /* build up our event masks and tell root_win its attributes */
    root_attr.event_mask = SubstructureRedirectMask | SubstructureNotifyMask |
                           ButtonPressMask | ButtonReleaseMask |
                           PointerMotionMask;
    XChangeWindowAttributes(display, screen->root_win, CWEventMask,
                            &root_attr);
}

void screen_print_clients(screen_item *screen) {
    list_node *node;
    client_item *client;
    
    /* loop through our clients printing their information */
    list_foreach(screen->client_list, node) {
        client = list_get_data_from_node(node);
        warn(__FILE__, "Client %3d (%s)\n", *client->index,
             client->name);
    }
}

list_header *screen_get_list(Display *display, XContext root_context) {
    int i, screen_count;
    list_header *list;
    screen_item *screen;

    list = list_init(free_screen);
    screen_count = ScreenCount(display);

    /* loop the screens adding them to our list */
    for (i = 0; i < screen_count; i++) {
        screen = (screen_item *)malloc(sizeof(screen_item));

        /* fill in our screens details */
        screen->index          = i;
        screen->root_win       = RootWindow(display, i);
        screen->display_width  = DisplayWidth(display, i);
        screen->display_height = DisplayHeight(display, i);

        /* save our context so we can refer to root_win later */
        XSaveContext(display, screen->root_win, root_context,
                     (XPointer)screen);

        /* init the root win */
        setup_root_win(display, screen);
        
        /* if we have a mem error destroy the rest of the list */
        if (! list_append_node(list, screen)) {
            list_destroy(list);
            return ((list_header *)0);
        }
    }

    return (list);
}
