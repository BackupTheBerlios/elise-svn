#ifndef XERROR_H_2136712847236847623847682734
#define XERROR_H_2136712847236847623847682734

int xerror_print(Display *display, XErrorEvent *event);
int init_xerror_print(Display *display, XErrorEvent *event);
void warn(const char *file, const char *fmt, ...);

#endif
