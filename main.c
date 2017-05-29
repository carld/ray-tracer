#include     <stdio.h>
#include     <stdlib.h>
#include     <string.h>
#include     <X11/Xlib.h>
#include     <math.h>

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "camera.h"

vec3 color(ray *r, struct sphere *spheres, int nsph) {
  hit_record rec;
  if (world_hit(spheres, nsph, r, 0.0, MAXFLOAT, &rec)) {
    vec3 v_ = { .x = rec.normal.x + 1, .y = rec.normal.y + 1, .z = rec.normal.z + 1 };
    return vec3_multiply_float(v_, 0.5f);
  } else {
    vec3 unit_direction = unit_vector(r->B);
    float t = 0.5f * (unit_direction.y + 1.0);
    vec3 white = { .x = 1.0f, .y = 1.0f, .z = 1.0f };
    vec3 blue = { .x = 0.5f, .y = 0.7f, .z = 1.0f };
    vec3 v_ = vec3_add_vec(vec3_multiply_float(white, 1.0f - t),
                          vec3_multiply_float(blue, t));
    return v_;
  }
}

XImage *
CreateTrueColorImage(Display * display, Visual * visual, unsigned char *image, int width, int height)
{
  int i, j, s;
  unsigned char  *image32 = (unsigned char *)malloc(width * height * 4);
  unsigned char  *p = image32;
  int ns = 20;

  vec3 origin            = { .x = 0.0, .y = 0.0, .z = 0.0 };

  /* all rays have the same origin */
  ray r = { .A = origin };

  struct sphere world[] = {
    { .center.x = 0, .center.y = 0, .center.z = -1, .radius = 0.5 },
    { .center.x = 0, .center.y = -100.5, .center.z = -1, .radius = 100 }
  };

  camera cam = {
    .lower_left_corner.x = -2, .lower_left_corner.y = -1, .lower_left_corner.z = -1,
    .horizontal.x = 4.0, .horizontal.y = 0, .horizontal.z = 0,
    .vertical.x = 0.0, .vertical.y = 2.0, .vertical.z = 0.0,
    .origin.x = 0.0, .origin.y = 0.0, .origin.z = 0.0
  };

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      vec3 col = { .x = 0, .y = 0, .z = 0 };
      for (s = 0; s < ns; s++) {
        float u = (j + drand48()) / (float) width;
        float v = ((height-i) + drand48()) / (float) height;
        r = camera_cast_ray(&cam, u, v);
        vec3 p = ray_point_at_parameter(&r, 2.0f);
        col = vec3_add_vec(col, color(&r, world, 2));
      }
      col = vec3_divide_float(col, ns);
      col = vec3_multiply_float(col, 255.99);
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
