#ifndef QUAD_H
#define QUAD_H

#include "hittables.h"
#include "interval.h"
#include "material.h"

#include <stdbool.h>

typedef struct Quad {
    Material *material;
    ShapeHitFn hit;

    Vec3 origin;
    Vec3 u, v;

    Vec3 normal;
    Vec3 w;
    double d;
} Quad;

Quad quad_from(const Vec3 center, const Vec3 u, const Vec3 v, void *material);

#endif // QUAD_H
