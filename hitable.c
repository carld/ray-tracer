#include <stdlib.h>
#include "hitable.h"

int sphere_hit(sphere *sp, const ray *r, float t_min ,float t_max, hit_record *hit) {
  vec3 oc = vec3_subtract_vec( r->A, sp->center);
  float a = vec3_dot(r->B, r->B);
  float b = vec3_dot(oc, r->B);
  float c = vec3_dot(oc, oc) - sp->radius * sp->radius;
  float discriminant = b*b - a * c;

  if (discriminant > 0) {
    float temp = (-b - sqrtf(b*b-a*c)) / a;
    if (temp < t_max && temp > t_min) {
      hit->t = temp;
      hit->p = ray_point_at_parameter(r, hit->t);
      hit->normal = vec3_divide_float(vec3_subtract_vec(hit->p, sp->center), sp->radius);
      return 1;
    }
    temp = (-b + sqrtf(b*b-a*c)) / a;
    if (temp < t_max && temp > t_min) {
      hit->t = temp;
      hit->p = ray_point_at_parameter(r, hit->t);
      hit->normal = vec3_divide_float(vec3_subtract_vec(hit->p, sp->center), sp->radius);
      return 1;
    }
  }
  return 0;
}

int world_hit(struct sphere *spheres, int n, const ray *r, float t_min, float t_max, hit_record *hit) {
  hit_record temp;
  int hit_anything = 0;
  float closest_so_far = t_max;
  int i;
  for (i = 0; i < n; i++) {
    if (sphere_hit(&spheres[i], r, t_min, closest_so_far, &temp)==1) {
      hit_anything = 1;
      closest_so_far = temp.t;
      *hit = temp;
      hit->mat = spheres[i].mat;
    }
  }
  return hit_anything;
}

vec3 random_in_unit_sphere() {
  vec3 p;
  do {
    p = vec3_subtract_vec(
          vec3_multiply_float( (vec3) { .x = drand48(), .y = drand48(), .z = drand48() }, 2.0),
          (vec3) { .x = 1, .y = 1, .z = 1 }
        );
  } while (vec3_squared_length(p) >= 1.0);
  return p;
}

