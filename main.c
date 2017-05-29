#include     <stdio.h>
#include     <stdlib.h>
#include     <string.h>
#include     <X11/Xlib.h>

#include "vec3.h"
#include "ray.h"

int hit_sphere(vec3 center, float radius, ray r) {
  vec3 oc = vec3_subtract_vec( r.A, center);
  float a = vec3_dot(r.B, r.B);
  float b = 2.0 * vec3_dot(oc, r.B);
  float c = vec3_dot(oc, oc) - radius * radius;
  float discriminant = b*b - 4 * a * c;
  return discriminant > 0;
}

vec3 color(const ray r) {
  vec3 v0 = { .x = 0, .y = 0, .z = -1 };
  if (hit_sphere(v0, 0.5, r)) {
    /* this returns red, x = r, y = g, z = b */
    vec3 v_ = { .x = 1, .y = 0, .z = 0 };
    return v_;
  }
  vec3 unit_direction = unit_vector(r.B);
  float t = 0.5 * (unit_direction.y + 1.0);
  vec3 v1 = { .x = 1.0, .y = 1.0, .z = 1.0 };
  vec3 v2 = { .x = 0.5, .y = 0.7, .z = 1.0 };
  vec3 v3 = vec3_add_vec(vec3_multiply_float(v1, 1.0 - t),
                         vec3_multiply_float(v2, t));
  return v3;
}

XImage *
CreateTrueColorImage(Display * display, Visual * visual, unsigned char *image, int width, int height)
{
  int    i         , j;
  unsigned char  *image32 = (unsigned char *)malloc(width * height * 4);
  unsigned char  *p = image32;

  vec3 lower_left_corner = { .x = -2.0, .y = -1.0, .z = -1.0 };
  vec3 horizontal        = { .x = 4.0, .y = 0.0, .z = 0.0 };
  vec3 vertical          = { .x = 0.0, .y = 2.0, .z = 0.0 };
  vec3 origin            = { .x = 0.0, .y = 0.0, .z = 0.0 };

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      float u = j / (float) width;
      float v = (height-i) / (float) height;
      vec3 dir = vec3_add_vec(lower_left_corner,
                              vec3_add_vec(vec3_multiply_float(horizontal, u),
                                           vec3_multiply_float(vertical, v)));
      ray r = { .A = origin, .B = dir };
      vec3 col = vec3_multiply_float(color(r), 255.99);

      *p++ = (int)col.z;
      *p++ = (int)col.y;
      *p++ = (int)col.x;
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
  int    width = 640, height = 320;
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
