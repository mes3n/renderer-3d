#include "interval.h"

double clamp(const Interval *interval, double x) {
    if (x < interval->min)
        return interval->min;
    if (x > interval->max)
        return interval->max;
    return x;
}
