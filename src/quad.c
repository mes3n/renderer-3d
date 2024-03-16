#include "quad.h"
#include "interval.h"
#include "vec3.h"

bool quad_hit(const Quad *quad, const Ray *ray, const Interval *rayt,
              HitRecord *record) {
    double denom = vec3_dot(quad->normal, ray->direction);
    if (fabs(denom) < 1e-3) {
        return false;
    }
    double t = (quad->d - vec3_dot(quad->normal, ray->origin)) / denom;

    if (!surrounds(rayt, t)) {
        return false;
    }

    Vec3 intersection = ray_at(ray, t);
    Vec3 hit = vec3_sub(intersection, quad->origin);
    double a = vec3_dot(quad->w, vec3_cross(hit, quad->v));
    double b = vec3_dot(quad->w, vec3_cross(quad->u, hit));

    const Interval zero_to_one = {0.0, 1.0};
    // if (!contains(&zero_to_one, a + b)) {
    //     return false;
    // }
    // if (!surrounds(&zero_to_one, 4 * (a * a - a + b * b - b) + 2)) {
    //     return false;
    // }
    if (!surrounds(&zero_to_one, a) || !surrounds(&zero_to_one, b)) {
        return false;
    }

    record->t = t;
    record->point = intersection;
    record->normal = quad->normal;

    set_face_normal(record, ray, quad->normal);

    return true;
}

Quad quad_from(const Vec3 center, const Vec3 u, const Vec3 v, void *material) {
    Quad quad;
    quad.material = material;
    quad.hit = (ShapeHitFn)quad_hit;

    quad.origin =
        vec3_sub(center, vec3_add(vec3_scale(u, 0.5), vec3_scale(v, 0.5)));
    quad.u = u;
    quad.v = v;

    Vec3 normal = vec3_cross(u, v);
    quad.normal = vec3_unit(normal);

    quad.d = vec3_dot(quad.normal, quad.origin);
    quad.w = vec3_scale(normal, 1.0 / vec3_dot(normal, normal));

    return quad;
}
