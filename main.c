#include     <stdio.h>
#include     <stdlib.h>
#include     <string.h>
#include     <X11/Xlib.h>
#include     <math.h>

#include "vec3.h"
#include "ray.h"
#include "hitable.h"
#include "camera.h"
#include "material.h"
#include <float.h>

int width = 1000, height = 500;
int sample_counter = 0;
unsigned char *image32;

#define BLACK (vec3){.x = 0, .y = 0, .z = 0}

struct material gray_metal = { .albedo.x = 0.8, .albedo.y = 0.8, .albedo.z = 0.8, .scatter = metal_scatter, .emit = BLACK };
struct material gold_metal = { .albedo.x = 0.8, .albedo.y = 0.6, .albedo.z = 0.2, .scatter = metal_scatter, .emit = BLACK  };
struct material black_metal = { .albedo.x = 0.1, .albedo.y = 0.1, .albedo.z = 0.1, .scatter = metal_scatter, .emit = BLACK  };
struct material red_ceramic = { .albedo.x = 0.65, .albedo.y = 0.1, .albedo.z = 0.1, .scatter = lambertian_scatter, .emit = BLACK };
struct material yellow_ceramic = { .albedo.x = 0.8, .albedo.y = 0.8, .albedo.z = 0.0, .scatter = lambertian_scatter, .emit = BLACK };
struct material blue_ceramic = { .albedo.x = 0.1, .albedo.y = 0.1, .albedo.z = 0.8, .scatter = lambertian_scatter, .emit = BLACK };
struct material green_ceramic = { .albedo.x = 0.1, .albedo.y = 0.8, .albedo.z = 0.1, .scatter = lambertian_scatter, .emit =BLACK };
struct material dielectric = { .ref_idx = 1.5, .scatter = dielectric_scatter };
struct material white_light = { .emit = (vec3) {.x = 0.9, .y = 0.9, .z = 0.9}, .scatter = lambertian_scatter};
struct material blue_light = { .albedo = (vec3) {.x = 0, .y =0, .z =0.5 }, .emit = (vec3) {.x = 0.1, .y = 0.1, .z = 0.5}, .scatter = metal_scatter};

struct sphere world[] = {
  { .center.x = 0, .center.y = 0, .center.z = -1, .radius = 0.1, .mat = & blue_ceramic },
  { .center.x = -1, .center.y = 0, .center.z = -1, .radius = 0.4, .mat = & gold_metal },
  { .center.x = 0, .center.y = 0, .center.z = 0, .radius = 0.2, .mat = & green_ceramic },
  { .center.x = 0, .center.y = 0, .center.z = -2, .radius = 0.5, .mat = & gray_metal },
  { .center.x = 1, .center.y = 0, .center.z = -1, .radius = 0.5, .mat = & dielectric },
  { .center.x = 1, .center.y = 0, .center.z = -1, .radius = -0.49, .mat = & dielectric },

  //{ .center.x = 0, .center.y = -100.5, .center.z = -1, .radius = 100, .mat = & red_ceramic  }

  { .center.x = -1, .center.y = 1, .center.z = -1, .radius = .5, .mat = & white_light }
};
vec3 lookfrom =  { .x = 3, .y = 2.0, .z = 3 };
vec3 lookat   =  { .x = 0, .y = 0, .z = -1 };
camera cam;

vec3 color(ray r, struct sphere *spheres, int nsph, int depth) {
  hit_record rec;
  if (world_hit(spheres, nsph, &r, 0.001, FLT_MAX, &rec)) {
    ray scattered;
    vec3 attenuation;

    if (depth < 16 && rec.mat->scatter(rec.mat, r, &rec, &attenuation, &scattered)) {
      return
        vec3_add_vec(rec.mat->emit,
          vec3_multiply_vec(attenuation, color(scattered, spheres, nsph, depth+1)));
    } else {
      return rec.mat->emit;
      //return (vec3) { .x = 0, .y = 0, .z = 0 };
    }
  } else { /* no intersection of ray with objects */
    vec3 unit_direction = vec3_unit_vector(r.B);
    float t = 0.5f * (unit_direction.y + 1.0);
    vec3 white = { .x = 1.0f, .y = 1.0f, .z = 1.0f };
    vec3 blue = { .x = 0.5f, .y = 0.7f, .z = 1.0f };
    vec3 v_ = vec3_add_vec(vec3_multiply_float(white, 1.0f - t),
                          vec3_multiply_float(blue, t));
    return v_;
    //return BLACK;
  }
}

float mix(float x, float y, float a) {
 return x * (1.0 - a) + y * a;
}

XImage *
UpdateTrueColorImage(Display * display, Visual * visual, unsigned char *image, int width, int height, int sample_number)
{
  int i, j, s;
  unsigned char  *p = image;
  int ns = 1; /* number of samples in this pass */

  float dist_to_focus = vec3_length(vec3_subtract_vec(lookfrom, lookat));
  float aperture = 1.0;
  camera_pos(
      &cam,
      lookfrom,
      lookat,
      (vec3) {.x = 0, .y = 1, .z = 0},  /* up */
      20,                       /* field of view */
      (float)width/(float)height, /* aspect ratio */
      aperture,
      dist_to_focus
      );

  for (i = 0; i < height; i++) {
    for (j = 0; j < width; j++) {
      vec3 col = { .x = 0, .y = 0, .z = 0 };
      for (s = 0 ; s < ns; s++ ) {
        float u = (j + drand48()) / (float) width;
        float v = ((height-i) + drand48()) / (float) height;
        ray r = camera_cast_ray(&cam, u, v);
        col = vec3_add_vec(col, color(r, world, sizeof(world) / sizeof(struct sphere), 0));
      }
      col = vec3_divide_float(col, ns);
      col = (vec3) { .x = sqrtf(col.x), .y = sqrtf(col.y), .z = sqrtf(col.z) };
      col = vec3_multiply_float(col, 255.99);

      if (sample_number == 0) {
        *p++ = (int)col.z;
        *p++ = (int)col.y;
        *p++ = (int)col.x;
        *p++ = 0;
      } else {
        *p = mix(*p, col.z, 1.0f / sample_number); p++;
        *p = mix(*p, col.y, 1.0f / sample_number); p++;
        *p = mix(*p, col.x, 1.0f / sample_number); p++;
        *p++ = 0;
      }

      //printf("\r%.02f%%", (float)  ((i*width+j) / (float) (height*width) * 100.0) );
      }
    }
  //printf("\n");
  return XCreateImage(display, visual, 24, ZPixmap, 0, (char*)image, width, height, 32, 0);
}

void
processEvent(Display * display, Window window, XImage * ximage, int width, int height)
{
  XEvent    ev;
  XNextEvent(display, &ev);
  switch (ev.type) {
  case Expose:
    /*if (ev.xexpose.count == 0) {*/
    XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, width, height);
    break;
  case ConfigureNotify:
    if (ev.xconfigure.width * ev.xconfigure.height > width * height) {
      free(image32);
      image32 = (unsigned char *)calloc(width * height * 4, sizeof(unsigned char));
    }
    width = ev.xconfigure.width;
    height = ev.xconfigure.height;
    sample_counter = 0;
    printf("Resize %d %d\n", ev.xconfigure.width, ev.xconfigure.height);
    //ximage = UpdateTrueColorImage(display, DefaultVisual(display, 0), image32, width, height, sample_counter);
    //XPutImage(display, window, DefaultGC(display, 0), ximage, 0, 0, 0, 0, width, height);
    break;
  case KeyPress:
    if(ev.xkey.keycode == 0x09)
      exit(0);
    break;
  case ButtonPress:
    exit(0);
  }
}

int
main(int argc, char **argv)
{
  XImage         *ximage = NULL;
  Display        *display = XOpenDisplay(NULL);
  Visual         *visual = DefaultVisual(display, 0);
  Window    window = XCreateSimpleWindow(display, RootWindow(display, 0), 0, 0, width, height, 1, 0, 0);
  if (visual->class != TrueColor) {
    fprintf(stderr, "Cannot handle non true color visual ...\n");
    exit(1);
  }
  XSelectInput(display, window,
    ButtonPressMask | KeyPressMask | KeyReleaseMask
    | ExposureMask
  //  | StructureNotifyMask
  );
  XMapWindow(display, window);

  image32 = calloc(width * height * 4, sizeof(unsigned char));
  ximage = UpdateTrueColorImage(display, visual, image32, width, height, sample_counter);
  while (1) {
    XEvent ev;
    ev.type = Expose;
    ev.xexpose.window = window;
    XSendEvent(display, window, False, ExposureMask, &ev);
    XFlush(display);
    processEvent(display, window, ximage, width, height);
    ximage = UpdateTrueColorImage(display, visual, image32, width, height, sample_counter++);
  }
}
