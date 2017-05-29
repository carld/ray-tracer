#ifndef CAMERA_H
#define CAMERA_H

#include "ray.h"

typedef struct camera {
  vec3 origin;
  vec3 lower_left_corner;
  vec3 horizontal;
  vec3 vertical;
} camera;

ray camera_cast_ray(camera *cam, float u, float v);
camera camera_pos(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect);

#endif
