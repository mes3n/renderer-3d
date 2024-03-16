#ifndef INTERVAL_H
#define INTERVAL_H

typedef struct {
    double min;
    double max;
} Interval;

static inline Interval interval_from(double min, double max) {
    return (Interval){.min = min, .max = max};
}

static inline int contains(const Interval *interval, double x) {
    return interval->min <= x && x <= interval->max;
}

static inline int surrounds(const Interval *interval, double x) {
    return interval->min < x && x < interval->max;
}

double clamp(const Interval *interval, double x);

#endif // INTERVAL_H
