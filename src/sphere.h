#ifndef SPHERE_H
#define SPHERE_H

#include "hit_record.h"
#include "hittables.h"
#include "interval.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

#include <stdbool.h>

typedef struct {
    Material *material;
    ShapeHitFn hit;

    Vec3 center;
    double radius;
} Sphere;

Sphere sphere_from(const Vec3 center, const double radius, void *material);

#endif // SPHERE_H
