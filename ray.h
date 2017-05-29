#ifndef RAY_H
#define RAY_H
#include "vec3.h"
typedef struct ray {
  vec3 A; /* origin */
  vec3 B; /* direction */
} ray;

vec3 ray_point_at_parameter(vec3 A, vec3 B, float t) {
  vec3 v2 = vec3_add_vec( A, vec3_multiply_float(B, t) );
  return v2;
}

#endif
