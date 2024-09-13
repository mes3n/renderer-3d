#ifndef EVENTS_H
#define EVENTS_H

#include "camera.h"
#include <stdbool.h>

bool should_close();
void init_events();
void poll_events(Camera *camera, const double dt);
double tick();

#endif // EVENTS_H
