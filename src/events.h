#ifndef EVENTS_H
#define EVENTS_H

#include <stdbool.h>

bool should_close();
void poll_events();
double tick();

#endif // EVENTS_H
