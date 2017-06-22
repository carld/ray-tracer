#include <stdlib.h>
#include <stdio.h>
#include "hitable.h"
#include "material.h"

struct object scene[256] = { { 0, 0 } };
int num_objects = 0;

struct material gold_metal ;
struct material white_light ;
struct material blue_light ;
struct material black_metal ;
struct material gray_metal ;
struct material blue_metal;
struct material green_metal;
struct material white_metal;
struct material gray_ceramic;
struct material green_ceramic;
struct material black_ceramic;
struct material white_ceramic;

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
  float y = -0.0;
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
  if (r1 < 0.01 || r2 < 0.01) {
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
#if 1
  for (i = 0; i < n; i++) {
    if (sphere_hit(&spheres[i], r, t_min, closest_so_far, &temp)==1) {
      hit_anything = 1;
      closest_so_far = temp.t;
      *hit = temp;
      hit->mat = spheres[i].mat;
    }
  }
#endif
#if 1
  if (plane_hit(r, t_min, closest_so_far, &temp)) {
    hit_anything = 1;
    *hit = temp;
    hit->mat = &gray_metal;
  //  hit->mat = &gray_ceramic;
  }
#endif
#if 0
  if (grid_hit(r, t_min, closest_so_far, &temp)) {
    hit_anything = 1;
    *hit = temp;
    hit->mat = &blue_light;
  }
#endif

  if (scene_hit(r, t_min, closest_so_far, &temp)==1) {
    hit_anything = 1;
    *hit = temp;
  }

#if 0
  //xy_rect xyr = {.x0=-3, .y0=0, .x1=3, .y1=2, .k=-2, .mat= &white_light };
  xy_rect xyr1 = {.x0=-64, .y0=0, .x1=64, .y1=64, .k=64, .mat= &white_light };
  if(rect_hit(&xyr1, r, t_min, closest_so_far, &temp)==1) {
    hit_anything=1;
    *hit = temp;
    hit->mat = &white_light;
    hit->normal = (vec3){ 0, 0, -1 };
  }
#endif
  return hit_anything;
}

vec3 random_in_unit_sphere() {
  vec3 p;
  do {
    p = vec3_subtract_vec(
          vec3_multiply_float(
            (vec3) { .x = drand48(), .y = drand48(), .z = drand48() }, 2.0),
            (vec3) { .x = 1, .y = 1, .z = 1 }
        );
  } while (vec3_squared_length(p) >= 1.0);
  return p;
}

int rect_hit(const xy_rect *rect, const ray *r, float t0, float t1, hit_record *hit) {
  float t = (rect->k - r->A.z) / r->B.z;
  if (t < t0 || t > t1)
    return 0;
  float x = r->A.x + t * r->B.x;
  float y = r->A.y + t * r->B.y;
  if (x < rect->x0 || x > rect->x1 || y < rect->y0 || y > rect->y1)
    return 0;
  hit->u = (x - rect->x0)/(rect->x1 - rect->x0);
  hit->v = (y - rect->y0)/(rect->y1 - rect->y0);
  hit->t = t;
  hit->mat = rect->mat;
  hit->p = ray_point_at_parameter(r, t);
  hit->normal = (vec3){0, 0, 1};
  return 1;
}

#include "logo.c"

int load_scene() {
  int i = 0, j = 0, k = 0;
  int x = 0, y = 0;
  /* lines */
  for (i = 0; i < 128; i++) {
    if (logo[i][0] == '\0') break;

    /* chars */
    x = 0;
    for (j = 0; j < 64; j ++) {
      struct object *obj = &scene[k];
      if (logo[i][j]=='\0') break;
      if (logo[i][j]==' ') {
        x++; continue;
      }
      k++;
      num_objects++;
      obj->hitfn = rect_hit;
      obj->odata = malloc(sizeof (xy_rect));
      obj->odata->x0 = -6 + x;
      obj->odata->y0 = 14 - y;
      obj->odata->x1 = -6 + x + 1.0;
      obj->odata->y1 = 14 - y + 1.0;
      obj->odata->k  = 0;
      switch(logo[i][j]) {
      case 'W': obj->odata->mat = &white_ceramic; break;
      case 'B': obj->odata->mat = &black_ceramic; break;
      case 'G': obj->odata->mat = &green_ceramic; break;
      default: exit(0);
      }
      //printf("%d: %f,%f   ", k, obj->odata->x0, obj->odata->y0);
      x++;
    }
    y++;
  }
  return 0;
}

int scene_hit(const ray * r, float t_min, float t_max, hit_record *hit) {
  int i;
  int hit_anything = 0;
  float closest_so_far = t_max;
  hit_record temp;
  for (i = 0; i < num_objects; i++) {
//    if (scene[i].hitfn == 0) break;
    if (rect_hit(scene[i].odata, r, t_min, t_max, &temp) == 1) {
      hit_anything = 1;
      closest_so_far = temp.t;
      *hit = temp;
      hit->mat = scene[i].odata->mat;
    }
  }
  return hit_anything;
}

