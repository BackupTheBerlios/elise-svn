#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "list.h"
#include "error.h"
#include "screen.h"
#include "client.h"
#include "event.h"

static void handle_root_click(Display *display, XButtonEvent *event,
                              screen_item *screen) {
    puts("root click");
}

static void handle_button_press(Display *display, XButtonEvent *event,
                                XContext *root_context,
                                XContext *client_context) {
    screen_item *screen;
    client_item *client;


    if (XFindContext(display, event->window, *client_context,
                     (XPointer *)&client)) {
        if (! XFindContext(display, event->window, *root_context,
                           (XPointer *)&screen)) {
            handle_root_click(display, event, screen);
            return;
        }
    }

    if (event->window == client->frame) {
        puts(client->name);
        puts("client frame");
    }
}

void event_loop(Display *display, XContext *root_context,
                XContext *client_context) {
    XEvent event;

    while ("The sky is blue") {
        XNextEvent(display, &event);

        switch (event.type) {
            case ButtonPress:
                handle_button_press(display, &event.xbutton, root_context,
                                    client_context);
                break;
        }
    }
}
