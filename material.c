#include <stdlib.h>
#include "material.h"

int no_scatter(material *mat, ray r_in, struct hit_record *rec, vec3 *attenuation, ray *scattered) {
  return 0;
}

int lambertian_scatter(material *mat, ray r_in, struct hit_record *rec, vec3 *attenuation, ray *scattered) {
  vec3 target = vec3_add_vec(
                  rec->p,
                  vec3_add_vec(rec->normal,
                    random_in_unit_sphere()));
  *scattered = (ray) { .A = rec->p, .B = vec3_subtract_vec(target, rec->p) };
  *attenuation = mat->albedo;
  return 1;
}

vec3 reflect(vec3 v, vec3 n) {
  /* v - 2 * dot(v, n) * n */
  vec3 v_ = vec3_subtract_vec(v, vec3_multiply_float(n, 2 * vec3_dot(v, n)));
  return v_;
}

int metal_scatter(material *mat, ray r_in, struct hit_record *rec, vec3 *attenuation, ray *scattered) {
  vec3 reflected = reflect(vec3_unit_vector(r_in.B), rec->normal);
  *scattered = (ray) { .A = rec->p, .B = reflected };
  *attenuation = mat->albedo;
  return vec3_dot(scattered->B, rec->normal) > 0;
}

int refract(vec3 v, vec3 n, float ni_over_nt, vec3 *refracted) {
  vec3 uv = vec3_unit_vector(v);
  float dt = vec3_dot(uv, n);
  float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
  if (discriminant > 0) {
    /*refracted = ni_over_nt * (uv - n * dt) - n * sqrtf(discriminant);*/
    *refracted =
        vec3_subtract_vec(
          vec3_multiply_float(
            vec3_subtract_vec(uv, vec3_multiply_float(n, dt)), /* (uv - n * dt) */
            ni_over_nt),
          vec3_multiply_float(n, sqrtf(discriminant))       /* n * sqrtf(discriminant) */
          );
    return 1;
  } else {
    return 0;
  }
}

float schlick(float cosine, float ref_idx) {
  float r0 = (1-ref_idx) / (1+ref_idx);
  r0 = r0*r0;
  return r0 + (1-r0) * pow((1 - cosine), 5);
}

int dielectric_scatter(material *mat, ray r_in, hit_record *rec, vec3 *attenuation, ray *scattered) {
  vec3 outward_normal;
  vec3 reflected = reflect(r_in.B, rec->normal);
  float ni_over_nt;
  *attenuation = (vec3) { .x = 1.0, .y = 1.0, .z = 1.0 };
  vec3 refracted;
  float reflect_prob;
  float cosine;

  if (vec3_dot(r_in.B, rec->normal) > 0) {
    outward_normal = vec3_multiply_float(rec->normal, -1);
    ni_over_nt = mat->ref_idx;
    cosine = mat->ref_idx * vec3_dot(r_in.B, rec->normal) / vec3_length(r_in.B);
  } else {
    outward_normal = rec->normal;
    ni_over_nt = 1.0 / mat->ref_idx;
    cosine = - vec3_dot(r_in.B, rec->normal) / vec3_length(r_in.B);
  }
  if (refract(r_in.B, outward_normal, ni_over_nt, &refracted)) {
    reflect_prob = schlick(cosine, mat->ref_idx);
  } else {
    reflect_prob = 1.0;
  }
  if (drand48() < reflect_prob) {
    *scattered = (ray) { .A = rec->p, .B = reflected };
  } else {
    *scattered = (ray) { .A = rec->p, .B = refracted };
  }
  return 1;
}
