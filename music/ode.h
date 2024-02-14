#pragma once
#include "tones.h"
// ode to joy
// 140 bpm
#define CTVRT 428  // v ms

const uint16_t ode[] PROGMEM = {
    h1, CTVRT, h1, CTVRT, c2, CTVRT, d2, CTVRT,
    d2, CTVRT, c2, CTVRT, h1, CTVRT, a1, CTVRT,
    g1, CTVRT, g1, CTVRT, a1, CTVRT, h1, CTVRT,
    h1, 3 * CTVRT / 2, a1, CTVRT / 2, a1, CTVRT * 2,
    h1, CTVRT, h1, CTVRT, c2, CTVRT, d2, CTVRT,
    d2, CTVRT, c2, CTVRT, h1, CTVRT, a1, CTVRT,
    g1, CTVRT, g1, CTVRT, a1, CTVRT, h1, CTVRT,
    a1, 3 * CTVRT / 2, g1, CTVRT / 2, g1, CTVRT * 2,
    a1, CTVRT, a1, CTVRT, h1, CTVRT, g1, CTVRT,
    a1, CTVRT, h1, CTVRT / 2, c2, CTVRT / 2, h1, CTVRT, g1, CTVRT,
    a1, CTVRT, h1, CTVRT / 2, c2, CTVRT / 2, h1, CTVRT, a1, CTVRT,
    g1, CTVRT, a1, CTVRT, d1, CTVRT, h1, CTVRT * 2,  // ligatura
    h1, CTVRT, c2, CTVRT, d2, CTVRT,
    d2, CTVRT, c2, CTVRT, h1, CTVRT, a1, CTVRT,
    g1, CTVRT, g1, CTVRT, a1, CTVRT, h1, CTVRT,
    a1, 3 * CTVRT / 2, g1, CTVRT / 2, g1, CTVRT * 2,
    a1, CTVRT, a1, CTVRT, h1, CTVRT, g1, CTVRT,
    a1, CTVRT, h1, CTVRT / 2, c2, CTVRT / 2, h1, CTVRT, g1, CTVRT,
    a1, CTVRT, h1, CTVRT / 2, c2, CTVRT / 2, h1, CTVRT, a1, CTVRT,
    g1, CTVRT, a1, CTVRT, d1, CTVRT, h1, CTVRT * 2,  // ligatura
    h1, CTVRT, c2, CTVRT, d2, CTVRT,
    d2, CTVRT, c2, CTVRT, h1, CTVRT, a1, CTVRT,
    g1, CTVRT, g1, CTVRT, a1, CTVRT, h1, CTVRT,
    a1, 3 * CTVRT / 2, g1, CTVRT / 2, g1, CTVRT * 2};