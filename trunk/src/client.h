#ifndef CLIENT_H_784237487293874982374987298123111
#define CLIENT_H_784237487293874982374987298123111

#include "screen.h"

typedef struct CLIENT_ITEM {
    char *name;
    XWindowAttributes *attribs;
    Window window;
    Window frame;

    int *index;
    screen_item *screen;
} client_item;

void client_destroy(Display *display, client_item *client,
                    XContext *client_context);
struct LIST_HEADER *client_get_list(Display *display, screen_item *screen,
                                    XContext *client_context); 
#endif
