#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "list.h"
#include "error.h"
#include "screen.h"
#include "client.h"
#include "event.h"
#include "action.h"

static void event_client_click(Display *display, XButtonEvent *event,
                               client_item *client) {
    action_move(display, client);
}

static void event_client_configure(Display *display,
                                   XConfigureRequestEvent *event,
                                   client_item *client) {
    XWindowChanges win_changes;

    if (event->value_mask & CWX)
        client->attribs->x = event->x;

    if (event->value_mask & CWY)
        client->attribs->y = event->y;

    if (event->value_mask & CWWidth)
        client->attribs->width = event->width;

    if (event->value_mask & CWHeight)
        client->attribs->height = event->height;

    warn(0, "config req\n");

    action_send_config_to_client(display, client);
}

void event_loop(Display *display, XContext *root_context,
                XContext *client_context, list_header *screen_list) {
    XEvent event;
    XAnyEvent *all_events;
    client_item *client;
    screen_item *screen;

    while ("SCO litigates") {
        XNextEvent(display, &event);
        all_events = &event.xany;

        client = (client_item *)0;
        screen = (screen_item *)0;

        /* find our client and screen, look ma, no globals */
        XFindContext(display, all_events->window, *client_context,
                     (XPointer *)&client);
        XFindContext(display, all_events->window, *root_context,
                     (XPointer *)&screen);

        /* preform and action based on event.type */
        switch (event.type) {
            case ConfigureRequest:
                event_client_configure(display, &event.xconfigurerequest,
                                       client);
            break;
            case ButtonPress:
                if (client != (client_item *)0)
                    event_client_click(display, &event.xbutton, client);

                XAllowEvents(display, ReplayPointer, CurrentTime);
            break;
        }
    }
}
