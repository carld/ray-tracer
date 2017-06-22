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

typedef struct xy_rect {
  float x0, x1, y0, y1, k;
  struct material *mat;
} xy_rect;

typedef struct object {
  int (*hitfn) (const xy_rect * odata, const ray *, float, float, hit_record *);
  xy_rect *odata;
} object;

int sphere_hit(sphere *sp, const ray *r, float t_min ,float t_max, hit_record *hit);
int world_hit(struct sphere *spheres, int n, const ray *r, float t_min, float t_max, hit_record *hit);
vec3 random_in_unit_sphere();
int plane_hit(const ray *r, float t_min, float t_max, hit_record *hit);
int rect_hit(const xy_rect *rect, const ray *r, float t0, float t1, hit_record *hit);

int load_scene();
int scene_hit(const ray *, float t_min, float t_max, hit_record *hit);

#endif
