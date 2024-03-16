#include "camera.h"
#include "hittables.h"
#include "material.h"
#include "quad.h"
#include "sphere.h"
#include "vec3.h"

#include "events.h"
#include "graphics.h"

#include <stdio.h>

int main() {
    Hittables *world = NULL;

    Lambertian lambertian = {
        (ScatterFn)lambertian_scatter,
        .albedo = vec3_from(0.7, 0.3, 0.3),
    };
    Lambertian gold = {
        (ScatterFn)lambertian_scatter,
        .albedo = vec3_from(0.9, 0.7, 0.1),
    };
    Metal metal = {
        (ScatterFn)metal_scatter,
        .albedo = vec3_from(0.8, 0.8, 0.8),
    };
    Lambertian mat_ground = {
        (ScatterFn)lambertian_scatter,
        .albedo = vec3_from(0.8, 0.8, 0.0),
    };

    const Sphere sphere_center =
        sphere_from(vec3_from(0.0, 0.0, -1.0), 0.5, &lambertian);
    hittables_add(&world, (Shape *)&sphere_center);

    const Sphere sphere_left =
        sphere_from(vec3_from(-1.0, 0.0, -1.0), 0.5, &metal);
    hittables_add(&world, (Shape *)&sphere_left);

    Sphere ground =
        sphere_from(vec3_from(0.0, -100.5, -1.0), 100.0, &mat_ground);
    hittables_add(&world, (Shape *)&ground);

    const Quad ceiling =
        quad_from(vec3_from(0.0, 2.0, -1.0), vec3_from(5.0, 0.0, 0.0),
                  vec3_from(0.0, 0.0, 5.0), &gold);
    hittables_add(&world, (Shape *)&ceiling);

    Camera camera;
    if (!init_camera(&camera)) {
        return 1;
    }

    double dt;
    while (!should_close()) {
        dt = tick();

        poll_events();
        render(&camera, world);

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
