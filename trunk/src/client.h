#ifndef CLIENT_H_784237487293874982374987298123111
#define CLIENT_H_784237487293874982374987298123111

#include "screen.h"

typedef struct CLIENT_ITEM {
    char *name;
    XWindowAttributes *attribs;
    Window window;
    Window frame;
} client_item;

struct LIST_HEADER *client_get_list(Display *display, screen_item *screen,
                                    XContext *client_context); 
#endif
