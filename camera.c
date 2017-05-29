#include <math.h>
#include "camera.h"

camera camera_pos(vec3 lookfrom, vec3 lookat, vec3 vup, float vfov, float aspect) {
  camera cam;
  vec3 u, v, w;
  float theta = vfov * M_PI / 180;
  float half_height = tan(theta/2);
  float half_width = aspect * half_height;
  cam.origin = lookfrom;
  w = unit_vector( vec3_subtract_vec(lookfrom, lookat));
  u = unit_vector( vec3_cross(vup, w));
  v = vec3_cross(w, u);
  cam.lower_left_corner = (vec3) { .x = -half_width, .y = -half_height, .z = -1.0};
#if 0
  cam.lower_left_corner = vec3_subtract_vec(cam.origin,
                             vec3_subtract_vec(
                              vec3_subtract_vec(
                                vec3_multiply_float(u, half_width),
                                vec3_multiply_float(v, half_height)),
                              w));
#endif
  cam.horizontal  = vec3_multiply_float(u,  2* half_width);
  cam.vertical  = vec3_multiply_float(v, 2 * half_height);
  return cam;
}

ray camera_cast_ray(camera *cam, float u, float v) {
  return (ray) { .A = cam->origin,
                 .B = vec3_subtract_vec(
                                vec3_add_vec(
                                  cam->lower_left_corner,
                                    vec3_add_vec(
                                      vec3_multiply_float(cam->horizontal, u),
                                      vec3_multiply_float(cam->vertical, v))),
                                cam->origin) } ;
}
