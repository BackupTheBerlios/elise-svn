#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include <stdio.h>
#include <stdlib.h>

#include "list.h"
#include "screen.h"
#include "client.h"

/* the function we use to free a screen object */
static void free_client(list_data *client) {
    free(client);
}

/* actually create the client */
static client_item *create_client(Display *display, Window window,
                                  XWindowAttributes *attribs,
                                  screen_item *screen,
                                  XContext *client_context) {
    client_item *client;
    XSetWindowAttributes set_attribs;

    client = (client_item *)malloc(sizeof(client_item));

    XGrabServer(display);

    /* set some values for clients */
    client->window  = window;
    client->attribs = attribs;
    XFetchName(display, window, &client->name);

    /* create the new clients parent */
    client->frame = XCreateSimpleWindow(display, screen->root_win,
                                        client->attribs->x,
                                        client->attribs->y,
                                        client->attribs->width,
                                        client->attribs->height + 18,
                                        0, BlackPixel(display, screen->num),
                                        WhitePixel(display, screen->num));

    /* set some attribtes for the new window */
    set_attribs.do_not_propagate_mask = ButtonPressMask | ButtonReleaseMask;
    set_attribs.event_mask            = EnterWindowMask;
    set_attribs.win_gravity           = StaticGravity;

    XReparentWindow(display, window, client->frame, 0, 15);
    XGrabButton(display, AnyButton, AnyModifier, client->frame, 1,
                ButtonPressMask, GrabModeSync, GrabModeAsync, None, None);
    XSelectInput(display, client->frame, SubstructureNotifyMask |
                 SubstructureRedirectMask);
    XChangeWindowAttributes(display, client->frame, CWEventMask, &set_attribs);
    XAddToSaveSet(display, window);

    XMapWindow(display, client->frame);
    XMapWindow(display, window);

    XSync(display, 0);
    XUngrabServer(display);

    /* save our contexts so we can grab them in our event_loop */
    XSaveContext(display, window, *client_context, (XPointer)client);
    XSaveContext(display, client->frame, *client_context, (XPointer)client);

    return (client);
}

list_header *client_get_list(Display *display, screen_item *screen,
                             XContext *client_context) {
    int i, window_count;
    list_header *list;
    client_item *client;
    Window dummy, *windows;
    XWindowAttributes attribs;

    list = list_init(free_client);

    /* get a list of clients from X */
    XQueryTree(display, screen->root_win, &dummy, &dummy, &windows,
               &window_count);

    /* loop throught the windows adding them to our list */
    for (i = 0; i < window_count; i++) {
        if (! XGetWindowAttributes(display, windows[i], &attribs))
            continue;

        /* if the window is viewable add it to our list */
        if (attribs.map_state == IsViewable ) {
            client = create_client(display, windows[i], &attribs, screen,
                                   client_context);
            list_append_node(list, client);
#ifdef DEBUG
            warn(__FILE__, "Creating client (%s) on screen %d.\n",
                 client->name, screen->num);
#endif
        }
    }
    XFree(windows);

    return (list);
}
