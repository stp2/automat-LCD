#pragma once
#include "tones.h"
// cancan
// 140 bpm
#define CTVRT 428  // v ms

const uint16_t cancan[] PROGMEM = {
    g1, CTVRT * 2,
    a1, CTVRT / 2, c2, CTVRT / 2, h1, CTVRT / 2, a1, CTVRT / 2,
    d2, CTVRT, d2, CTVRT,
    d2, CTVRT / 2, e2, CTVRT / 2, h1, CTVRT / 2, c2, CTVRT / 2,
    a1, CTVRT, a1, CTVRT,
    a1, CTVRT / 2, c2, CTVRT / 2, h1, CTVRT / 2, a1, CTVRT / 2,
    g1, CTVRT / 2, g2, CTVRT / 2, g2b, CTVRT / 2, e2, CTVRT / 2,
    d2, CTVRT / 2, c2, CTVRT / 2, h1, CTVRT / 2, a1, CTVRT / 2,
    g1, CTVRT * 2,
    a1, CTVRT / 2, c2, CTVRT / 2, h1, CTVRT / 2, a1, CTVRT / 2,
    d2, CTVRT, d2, CTVRT,
    d2, CTVRT / 2, e2, CTVRT / 2, h1, CTVRT / 2, c2, CTVRT / 2,
    a1, CTVRT, a1, CTVRT,
    a1, CTVRT / 2, c2, CTVRT / 2, h1, CTVRT / 2, a1, CTVRT / 2,
    g1, CTVRT / 2, d2, CTVRT / 2, a1, CTVRT / 2, h1, CTVRT / 2,
    g1, CTVRT, d1, CTVRT};