#include "material.h"
#include "ray.h"
#include "vec3.h"

bool lambertian_scatter(const Lambertian *lambertian, const Ray *ray,
                        const HitRecord *hr, Ray *scattered,
                        Vec3 *attenuation) {
    *scattered = (Ray){0};
    *attenuation = lambertian->albedo;

    return false;
}

bool metal_scatter(const Metal *metal, const Ray *ray, const HitRecord *hr,
                   Ray *scattered, Vec3 *attenuation) {
    Vec3 direction = vec3_reflect(vec3_normal(ray->direction), hr->normal);

    *scattered = ray_from(hr->point, direction);
    *attenuation = metal->albedo;

    return true;
}

static inline double reflectance(double cosine, double ri) {
    double r0 = (1.0 - ri) / (1.0 + ri);
    r0 = r0 * r0;
    return r0 + (1.0 - r0) * pow((1.0 - cosine), 5);
}

bool dielectric_scatter(const Dielectric *dielectric, const Ray *ray,
                        const HitRecord *hr, Ray *scattered,
                        Vec3 *attenuation) {
    *attenuation = vec3_from(1.0, 1.0, 1.0);
    double refraction_ratio = hr->front_face ? (1.0 / dielectric->refraction)
                                             : dielectric->refraction;

    Vec3 unit_direction = vec3_normal(ray->direction);
    double cos_theta =
        fmin(vec3_dot(vec3_scale(unit_direction, -1.0), hr->normal), 1.0);
    double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

    Vec3 direction;
    if (refraction_ratio * sin_theta > 1.0 ||
        reflectance(cos_theta, refraction_ratio) > random_double()) {
        direction = vec3_reflect(unit_direction, hr->normal);
    } else {
        direction = vec3_refract(unit_direction, hr->normal, refraction_ratio);
    }
    *scattered = ray_from(hr->point, direction);
    return true;
}
