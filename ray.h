#ifndef RAY_H
#define RAY_H
#include "vec3.h"
typedef struct ray {
  vec3 A; /* origin */
  vec3 B; /* direction */
} ray;

vec3 ray_point_at_parameter(const ray *r, float t);

#endif
