#ifndef CAMERA_H
#define CAMERA_H

#include "hittables.h"
#include "vec3.h"

typedef struct {
    Vec3 dx, dy;
    Vec3 pos_at00;
} Viewport;

typedef struct {
    int image_width, image_height;
    int samples_per_pixel;
    int max_depth;

    Vec3 origin;
    double yaw, pitch;

    Viewport viewport;
} Camera;

typedef enum {
    CAMERA_FORWARD,
    CAMERA_BACKWARD,
    CAMERA_LEFT,
    CAMERA_RIGHT,
    CAMERA_UP,
    CAMERA_DOWN,
} CameraDirection;

int init_camera(Camera *camera);
void camera_turn(Camera *camera, const double dx, const double dy);
void camera_move(Camera *camera, const CameraDirection direction,
                 const double dt);
void render(const Camera *camera, const Hittables *world);

#endif // CAMERA_H
