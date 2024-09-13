#include "camera.h"
#include "graphics.h"
#include "hittables.h"
#include "material.h"
#include "ray.h"
#include "vec3.h"

#include <math.h>
#include <stdio.h>

static const Vec3 vup = {0.0, 1.0, 0.0};
static const Vec3 light = {0.0, 20.0, 11.0};

int init_camera(Camera *camera) {
    const int image_width = 200u;
    const double aspect_ratio = 16.0 / 9.0;

    const int image_height = (int)((double)image_width / aspect_ratio);

    if (image_height < 1) {
        fprintf(stderr, "Image width cannot be less than 1.\n");
        return false;
    }

    camera->image_width = image_width;
    camera->image_height = image_height;

    camera->samples_per_pixel = 2;
    camera->max_depth = 2;

    camera->origin = vec3_from(0.0, 0.0, 5.0);
    Vec3 look_at = vec3_from(0.0, 0.0, 0.0);

    Vec3 facing = vec3_unit(vec3_sub(camera->origin, look_at));
    camera->yaw = atan2(facing.x, facing.z);
    camera->pitch = asin(facing.y);

    camera_turn(camera, 0.0, 0.0);

    if (init_graphics(image_width, image_height) < 0) {
        fprintf(stderr, "Failed to initialize graphics.\n");
        return false;
    }

    return true;
}

void camera_turn(Camera *camera, const double dx, const double dy) {
    camera->yaw -= dx;
    camera->pitch += dy;

    // "Constants"
    double vfov = M_PI / 9.0;
    double focus_dist = 10.0;
    double viewport_height = 2.0 * tan(vfov * 0.5) * focus_dist;
    double viewport_width = viewport_height * ((double)camera->image_width /
                                               (double)camera->image_height);

    Vec3 w = vec3_from_rotation(camera->yaw, camera->pitch);
    Vec3 u = vec3_unit(vec3_cross(vup, w));
    Vec3 v = vec3_cross(w, u);

    Vec3 viewport_u = vec3_scale(u, viewport_width);
    Vec3 viewport_v = vec3_scale(v, -viewport_height);

    camera->viewport.dx =
        vec3_scale(viewport_u, 1.0 / (double)camera->image_width);
    camera->viewport.dy =
        vec3_scale(viewport_v, 1.0 / (double)camera->image_height);

    Vec3 viewport_top_left =
        vec3_sub(vec3_sub(vec3_sub(camera->origin, vec3_scale(w, focus_dist)),
                          vec3_scale(viewport_u, 0.5)),
                 vec3_scale(viewport_v, 0.5));
    camera->viewport.pos_at00 = vec3_add(
        viewport_top_left,
        vec3_scale(vec3_add(camera->viewport.dx, camera->viewport.dy), 0.5));
}

void camera_move(Camera *camera, const CameraDirection direction,
                 const double dt) {
    Vec3 facing_xz = vec3_from(sin(camera->yaw), 0.0, cos(camera->yaw));

    Vec3 left = vec3_unit(vec3_cross(vup, facing_xz));

    switch (direction) {
    case CAMERA_FORWARD:
        camera->origin = vec3_sub(camera->origin, facing_xz);
        break;
    case CAMERA_BACKWARD:
        camera->origin = vec3_add(camera->origin, facing_xz);
        break;
    case CAMERA_LEFT:
        camera->origin = vec3_sub(camera->origin, left);
        break;
    case CAMERA_RIGHT:
        camera->origin = vec3_add(camera->origin, left);
        break;
    case CAMERA_UP:
        camera->origin = vec3_add(camera->origin, vup);
        break;
    case CAMERA_DOWN:
        camera->origin = vec3_sub(camera->origin, vup);
        break;
    }

    printf("\rCamera position: (%f, %f, %f)\n", camera->origin.x, camera->origin.y, camera->origin.z);
}

Vec3 ray_color(const Ray *ray, const int depth, const Hittables *world) {
    if (depth <= 0) {
        return vec3_zero();
    }
    HitRecord hr;
    Material *material;
    if (hit_any(world, ray, &(Interval){1e-3, INFINITY}, &hr, &material)) {
        Ray scattered;
        Vec3 attenuation;
        if (material->scatter(material, ray, &hr, &scattered, &attenuation)) {
            return vec3_scale_from_vec3(ray_color(&scattered, depth - 1, world),
                                        attenuation);
        }
        if (hit_any(world, &(Ray){hr.point, vec3_sub(light, hr.point)},
                    &(Interval){1e-3, 10.0}, &hr, &material)) {
            return vec3_scale(attenuation, 0.5);
        }
        return attenuation;
    }
    Vec3 unit = vec3_unit(ray->direction);
    double s = 0.5 * (unit.y + 1.0);
    return vec3_add(vec3_scale(vec3_from(1.0, 1.0, 1.0), 1.0 - s),
                    vec3_scale(vec3_from(0.5, 0.7, 1.0), s));
}

Vec3 pixel_sample(const Camera *camera, const double px, const double py) {
    return vec3_add(
        vec3_scale(camera->viewport.dx, px / (double)camera->samples_per_pixel),
        vec3_scale(camera->viewport.dy,
                   py / (double)camera->samples_per_pixel));
}

Ray get_ray(const Camera *camera, const int x, const int y, const double px,
            const double py) {
    Vec3 vp_pixel =
        vec3_add(camera->viewport.pos_at00,
                 vec3_add(vec3_scale(camera->viewport.dx, (double)x),
                          vec3_scale(camera->viewport.dy, (double)y)));
    vp_pixel = vec3_add(vp_pixel, pixel_sample(camera, px, py));

    // Vec3 ray_origin = camera->defocus_angle <= 0.0
    //                       ? camera->origin
    //                       : defocus_disk_sample(camera);
    Vec3 ray_origin = camera->origin;

    Vec3 ray_direction = vec3_sub(vp_pixel, ray_origin);
    return ray_from(ray_origin, ray_direction);
}

void render(const Camera *camera, const Hittables *world) {
    for (int y = 0; y < camera->image_height; y++) {
        for (int x = 0; x < camera->image_width; x++) {
            Vec3 rgb = vec3_zero();
            for (int i = 0; i < camera->samples_per_pixel; i++) {
                for (int j = 0; j < camera->samples_per_pixel; j++) {
                    Ray ray = get_ray(camera, x, y, (double)i, (double)j);
                    rgb = vec3_add(rgb,
                                   ray_color(&ray, camera->max_depth, world));
                }
            }
            rgb = vec3_scale(rgb, 1.0 / (camera->samples_per_pixel *
                                         camera->samples_per_pixel));
            set_pixel(x, y, rgb);
        }
    }
    update_render();
}
