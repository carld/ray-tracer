#ifndef HIT_RECORD_H
#define HIT_RECORD_H

#include "vec3.h"

struct material;

typedef struct hit_record {
  float t;
  vec3 p;
  vec3 normal;
  struct material *mat;
} hit_record;

#endif
