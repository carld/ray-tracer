#ifndef VEC3_H
#define VEC3_H

#include <math.h>

typedef struct vec3 {
  float x;
  float y;
  float z;
} vec3;

float vec3_length(vec3 v) {
  return sqrtf(v.x*v.x + v.y*v.y + v.z*v.z);
}

vec3 vec3_multiply(vec3 v1, int n) {
  vec3 v_ = { .x = v1.x * n, .y = v1.y * n, .z = v1.z * n };
  return v_;
}

vec3 vec3_multiply_float(vec3 v1, float n) {
  vec3 v_ = { .x = v1.x * n, .y = v1.y * n, .z = v1.z * n };
  return v_;
}

vec3 vec3_multiply_vec(vec3 v1, vec3 v2) {
  vec3 v_ = { .x = v1.x * v2.x, .y = v1.y * v2.y, .z = v1.z * v2.z };
  return v_;
}

vec3 vec3_divide(vec3 v1, int n) {
  vec3 v_ = { .x = v1.x / n, .y = v1.y / n, .z = v1.z / n };
  return v_;
}

vec3 vec3_divide_float(vec3 v1, float n) {
  vec3 v_ = { .x = v1.x / n, .y = v1.y / n, .z = v1.z / n };
  return v_;
}

vec3 vec3_add_vec(vec3 v1, vec3 v2) {
  vec3 v_ = { .x = v1.x + v2.x, .y = v1.y + v2.y, .z = v1.z + v2.z };
  return v_;
}

vec3 vec3_subtract_vec(vec3 v1, vec3 v2) {
  vec3 v_ = { .x = v1.x - v2.x, .y = v1.y - v2.y, .z = v1.z - v2.z };
  return v_;
}

vec3 unit_vector(vec3 v1) {
  vec3 v_ = vec3_divide_float(v1, vec3_length(v1));
  return v_;
}

float vec3_dot(vec3 v1, vec3 v2) {
  return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

#endif
