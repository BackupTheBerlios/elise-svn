#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xproto.h>

#include "error.h"

void warn(const char *file, const char *fmt, ...) {
    va_list ap;

    va_start(ap, fmt);

    if (file)
        fprintf(stderr, "Elise (%s): ", file);
    else
        fprintf(stderr, "Elise: ");

    vfprintf(stderr, fmt, ap);
    va_end(ap);
}

int init_xerror_print(Display *display, XErrorEvent *event) {
    warn(0, "Cannot start Elise, is there another "
            "Window manager running? Sorry, this is fatal.");

    exit(EXIT_FAILURE);
}

int xerror_print(Display *display, XErrorEvent *event) {
    if (event->error_code != BadWindow &&
            event->error_code != BadMatch &&
            event->request_code != X_GetGeometry &&
            event->error_code != BadDrawable)
        warn(0, "An X error has occured details are:\n"
             "\tcode: %d\n\trequest: %d\n\tminor: %d\n", event->error_code,
             event->request_code, event->minor_code);

    return 0;
}
