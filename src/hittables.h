#ifndef HITTABLES_H
#define HITTABLES_H

#include "hit_record.h"
#include "interval.h"
#include "material.h"
#include "ray.h"

#include <stdbool.h>

typedef bool (*ShapeHitFn)(const void *, const Ray *, const Interval *,
                           HitRecord *);

typedef struct {
    Material *material;
    ShapeHitFn hit;
} Shape;

typedef struct Hittables {
    const Shape *shape;

    struct Hittables *next;
} Hittables;

bool hit_any(const Hittables *hittables, const Ray *ray, const Interval *rayt,
             HitRecord *hit_record, Material **material);

int hittables_add(Hittables **hittables, const Shape *shape);
int hittables_remove(Hittables **hittables, const Shape *shape);

int hittables_len(Hittables *ht);
void hittables_clear(Hittables *hittables);

#endif // HITTABLES_H
