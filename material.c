#include "material.h"

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
  vec3 reflected = reflect(unit_vector(r_in.B), rec->normal);
  *scattered = (ray) { .A = rec->p, .B = reflected };
  *attenuation = mat->albedo;
  return vec3_dot(scattered->B, rec->normal) > 0;
}
