#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

typedef struct camera {
  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
} camera;

ray camera_cast_ray(camera *cam, float u, float v) {
  return (ray) { .A = cam->origin,
                 .B = vec3_subtract_vec(
                                vec3_add_vec(
                                  cam->lower_left_corner,
                                    vec3_add_vec(
                                      vec3_multiply_float(cam->horizontal, u),
                                      vec3_multiply_float(cam->vertical, v))),
                                cam->origin) } ;
}

#endif
