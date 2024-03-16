#include "hittables.h"

#include <stdlib.h>

bool hit_any(const Hittables *ht, const Ray *ray, const Interval *rayt,
             HitRecord *hit_record, Material **material) {

    HitRecord tmp_hr;
    double nearest = rayt->max;
    bool any_hit = false;

    const Hittables *next = ht;

    while (next != NULL) {
        if (next->shape == NULL) {
            continue;
        }
        if (next->shape->hit(next->shape, ray, &(Interval){rayt->min, nearest},
                             &tmp_hr)) {
            *hit_record = tmp_hr;
            nearest = hit_record->t;
            *material = next->shape->material;

            any_hit = true;
        }
        next = next->next;
    }

    return any_hit;
}

int hittables_add(Hittables **ht, const Shape *shape) {
    if (*ht == NULL) {
        Hittables *new = (Hittables *)malloc(sizeof(Hittables));

        if (new == NULL)
            return 0;

        new->next = NULL;
        new->shape = shape;

        *ht = new;
        return 1;
    }
    if ((*ht)->shape == shape) {
        return 0;
    }
    return hittables_add(&((*ht)->next), shape);
}

int hittables_remove(Hittables **ht, const Shape *shape) {
    if (*ht == NULL) {
        return 0;
    }
    if ((*ht)->shape == shape) {
        Hittables *next = (*ht)->next;
        free(*ht);
        *ht = next;
        return 1;
    }

    return hittables_remove(&((*ht)->next), shape);
}

int hittables_len(Hittables *ht) {
    if (ht == NULL) {
        return 0;
    }
    return 1 + hittables_len(ht->next);
}

void hittables_clear(Hittables *ht) {
    if (ht == NULL)
        return;

    hittables_clear(ht->next);
    free(ht);
}
