#ifndef SCREEN_H_1237812747628746823764827874382
#define SCREEN_H_1237812747628746823764827874382

typedef struct SCREEN_ITEM {
    Window root_win;

    int index;

    int display_width;
    int display_height;

    Cursor normal_cursor;
    Cursor move_cursor;

    struct LIST_HEADER *client_list;
} screen_item;

struct LIST_HEADER *screen_get_list(Display *display, XContext root_context);
void screen_print_clients(screen_item *screen);

#endif
