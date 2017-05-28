#include     <stdio.h>
#include     <stdlib.h>
#include     <string.h>
#include     <X11/Xlib.h>

#include "vec3.h"

XImage *
CreateTrueColorImage(Display * display, Visual * visual, unsigned char *image, int width, int height)
{
  int    i         , j;
  unsigned char  *image32 = (unsigned char *)malloc(width * height * 4);
  unsigned char  *p = image32;
  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      vec3 col = { .v = { i / (float) height,
                          j / (float) width,
                          0.2} };
      col = vec3_multiply(col, 255);
      *p++ = col.v[2];
      *p++ = col.v[1];
      *p++ = col.v[0];
      *p++ = 0;
      }
    }
  return XCreateImage(display, visual, 24, ZPixmap, 0, (char*)image32, width, height, 32, 0);
}

void
processEvent(Display * display, Window window, XImage * ximage, int width, int height)
{
  XEvent    ev;
  XNextEvent(display, &ev);
  switch (ev.type) {
  case Expose:
    XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, width, height);
    break;
  case ConfigureNotify:
    width = ev.xconfigure.width;
    height = ev.xconfigure.height;
    ximage = CreateTrueColorImage(display, DefaultVisual(display, 0), 0, width, height);
    XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, width, height);
    break;
  case ButtonPress:
    exit(0);
  }
}

int
main(int argc, char **argv)
{
  XImage         *ximage;
  int    width = 640, height = 480;
  Display        *display = XOpenDisplay(NULL);
  Visual         *visual = DefaultVisual(display, 0);
  Window    window = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, width, height, 1, 0, 0);
  if (visual->class != TrueColor) {
    fprintf(stderr, "Cannot handle non true color visual ...\n");
    exit(1);
  }
  ximage = CreateTrueColorImage(display, visual, 0, width, height);
  XSelectInput(display, window, ButtonPressMask | ExposureMask );
  XMapWindow(display, window);
  while (1) {
    processEvent(display, window, ximage, width, height);
  }
}
