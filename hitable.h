#ifndef HITABLE_H
#define HITABLE_H

#include "ray.h"
#include "hit_record.h"

struct material;

typedef struct sphere {
  vec3 center;
  float radius;
  struct material *mat;
} sphere;

int sphere_hit(sphere *sp, const ray *r, float t_min ,float t_max, hit_record *hit);
int world_hit(struct sphere *spheres, int n, const ray *r, float t_min, float t_max, hit_record *hit);
vec3 random_in_unit_sphere();

#endif
