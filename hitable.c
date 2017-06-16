#include <stdlib.h>
#include "hitable.h"
#include "material.h"

struct material gold_metal ;

#if 0
int sphere_bounding_box(sphere *sp, float t0, float t1, aarb *box) {
  vec3 radius = (vec3) { .x = sp.radius, .y = sp.radius, .z = sp.radius };
  *box = (aarb){
    vec3_subtract_vec(sp.center, radius),
    vec3_add_vec(sp.center, radius)
  };
  return 1;
}
#endif


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

int plane_hit(const ray *r, float t_min, float t_max, hit_record *hit) {
  float y = -0.5;
  float t = (y - r->A.y) / r->B.y;
  if (t < t_min || t > t_max) return 0;
  hit->t = t;
  hit->p = ray_point_at_parameter(r, t);
  hit->normal = (vec3) { .x = 0, .y = 1, .z = 0 };
  return 1;
}

int grid_hit(const ray *r, float t_min, float t_max, hit_record *hit) {
  float t = (-0.5 - r->A.y) / r->B.y;
  float fl;

  if (t < t_min || t > t_max) return 0; /* hit/miss plane? */

  hit->t = t;
  hit->p = ray_point_at_parameter(r, t);

  float r1 = fabsf(modff(hit->p.x, &fl));
  float r2 = fabsf(modff(hit->p.z, &fl));
  if (r1 < 0.1 || r2 < 0.1) {
    hit->normal = (vec3) { .x = 0, .y = 1, .z = 0 };
    return 1;
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

  if (grid_hit(r, t_min, closest_so_far, &temp)) {
    hit_anything = 1;
    *hit = temp;
    hit->mat = &gold_metal;
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
