#include "vec3.h"
#include "ray.h"

vec3 ray_point_at_parameter(const ray *r, float t) {
  vec3 v_ = vec3_add_vec( r->A, vec3_multiply_float(r->B, t) );
  return v_;
}
