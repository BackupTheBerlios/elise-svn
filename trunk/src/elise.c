#include <stdlib.h>
#include <stdio.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

#include "list.h"
#include "event.h"
#include "error.h"
#include "screen.h"
#include "client.h"

static void print_list(list_header *list) {
    list_node *node;

    list_foreach(list, node) {
        printf("%s (%d)\nprev: %p\nself: %p\nnext: %p\n\n",
               (char *)node->data, node->index, node->prev, node, node->next);
    }
}

int main(int argc, char **argv) {
    list_header *list;
    list_node   *node;

    list = list_init(0);

    list_prepend_node(list, "hi-append");
    list_prepend_node(list, "hi-append1");
    list_prepend_node(list, "hi-append3");

    print_list(list);

    list_del_node_by_index(list, 1);

    print_list(list);

    list_destroy(list);
}

int main1(int argc, char **argv) {
    Display *display;
    list_header *screen_list;
    list_node *screen_node;

    /* our contexts which help us avoid globals */
    XContext root_context   = XUniqueContext();
    XContext client_context = XUniqueContext();

    /* ask X for the display */
    if (! (display = XOpenDisplay(0))) {
        warn(__FILE__, "Can't open X display.\n");
        exit(EXIT_FAILURE);
    }

    /* setup our error handler */
    XSetErrorHandler(init_xerror_print);

    screen_list = screen_get_list(display, root_context);

    /* if the list failed to init we must exit */
    if (! screen_list) {
        warn(__FILE__, "Sorry, there was an error initialising "
             "the screens. This error is fatal.\n");
        return (EXIT_FAILURE);
    }

    /* grab our clients for each screen */
    list_foreach(screen_list, screen_node) {
        screen_item *screen = list_get_data_from_node(screen_node);
        screen->client_list = client_get_list(display, screen, &client_context);
    }

    XSetErrorHandler(xerror_print);

    /* the main event loop */
    event_loop(display, &root_context, &client_context);

    /* free our clients */
    list_foreach(screen_list, screen_node) {
        screen_item *screen = list_get_data_from_node(screen_node);
        list_destroy(screen->client_list);
    }

    /* free our screens */
    list_destroy(screen_list);

    return (EXIT_SUCCESS);
}
