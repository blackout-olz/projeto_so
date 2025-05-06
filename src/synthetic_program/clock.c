#include "clock.h"

static int current_time = 0;

void clock_init() {
    current_time = 0;
}

void clock_advance(int t) {
    if (t > 0) {
        current_time += t;
    }
}

int clock_get_time() {
    return current_time;
}
