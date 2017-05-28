#ifndef VEC3
#define VEC3

typedef struct vec3 {

  float v[3];

} vec3;

vec3 vec3_multiply(vec3 v1, int n) {
  vec3 v2 = { .v = { v1.v[0] * n,
                     v1.v[1] * n,
                     v1.v[2] * n } };
  return v2;
}

#endif
