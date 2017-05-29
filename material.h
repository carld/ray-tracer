#ifndef MATERIAL_H
#define MATERIAL_H

#include "ray.h"
#include "hit_record.h"
#include "hitable.h"

typedef struct material {
  vec3 albedo;
  float ref_idx;
  int (*scatter) (struct material *mat, ray r, struct hit_record *rec, vec3 *attenuation, ray *scattered);
} material;

int metal_scatter(material *mat, ray r_in, struct hit_record *rec, vec3 *attenuation, ray *scattered);
int lambertian_scatter(material *mat, ray r_in, struct hit_record *rec, vec3 *attenuation, ray *scattered);
int dielectric_scatter(material *mat, ray r_in, hit_record *rec, vec3 *attenuation, ray *scattered);

#endif
