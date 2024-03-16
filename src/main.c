#include "camera.h"
#include "hittables.h"
#include "material.h"
#include "sphere.h"
#include "vec3.h"

#include "events.h"
#include "graphics.h"

#include <stdio.h>
#include <stdlib.h>

int main() {
    Hittables *world = NULL;

    Lambertian material_center = {
        (ScatterFn)lambertian_scatter,
        .albedo = vec3_from(0.7, 0.3, 0.3),
    };
    Metal material_left = {
        (ScatterFn)metal_scatter,
        .albedo = vec3_from(0.8, 0.8, 0.8),
        .fuzz = 0.3,
    };
    Lambertian material_ground = {
        (ScatterFn)lambertian_scatter,
        .albedo = vec3_from(0.8, 0.8, 0.0),
    };

    const Sphere sphere_center = (Sphere){(Vec3){0.0, 0.0, -1.0}, .radius = 0.5,
                                          (Material *)&material_center};
    hittables_add(&world, &sphere_center, (ShapeHitFn)sphere_hit);

    const Sphere sphere_left = (Sphere){(Vec3){-1.0, 0.0, -1.0}, .radius = 0.5,
                                        (Material *)&material_left};
    hittables_add(&world, &sphere_left, (ShapeHitFn)sphere_hit);

    const Sphere ground = (Sphere){(Vec3){0.0, -100.5, -1.0}, .radius = 100.0,
                                   (Material *)&material_ground};
    hittables_add(&world, &ground, (ShapeHitFn)sphere_hit);

    Camera camera;
    if (!init_camera(&camera)) {
        return 1;
    }

    double dt = tick();
    while (!should_close()) {
        poll_events();
        dt = tick();

        render(&camera, world);
        update_render();

        camera.origin.x += 1.0 * dt;
        camera.origin.y += 1.0 * dt;
        camera.origin.z += 4.0 * dt;

        fflush(stdout);
        printf("\rFPS: %2.2f", 1.0 / dt);
    }
    printf("\n");

    hittables_clear(world);
    stop_graphics();

    return 0;
}
