#ifndef RAY_H
#define RAY_H
#include "vec3.h"
typedef struct ray {
  vec3 A; /* origin */
  vec3 B; /* direction */
} ray;

vec3 ray_point_at_parameter(const ray *r, float t) {
  vec3 v_ = vec3_add_vec( r->A, vec3_multiply_float(r->B, t) );
  return v_;
}

#endif
