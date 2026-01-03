/*---------------------------------------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

#ifndef PRAND_H
#define PRAND_H

#include <stdint.h>

typedef uint64_t prand_state;

extern prand_state __prand_state;

uint64_t prand_uint_s(prand_state* state, uint64_t min, uint64_t max);
uint64_t prand_uint(uint64_t min, uint64_t max);

int64_t prand_int_s(prand_state* state, int64_t min, int64_t max);
int64_t prand_int(int64_t min, int64_t max);

double prand_double_s(prand_state* state, double min, double max);
double prand_double(double min, double max);

float prand_float_s(prand_state* state, float min, float max);
float prand_float(float min, float max);

#ifdef PRAND_IMPL
#undef PRAND_IMPL

prand_state __prand_state = 0;

uint64_t prand_uint_s(prand_state* state, uint64_t min, uint64_t max)
{
    *state += 0x9e3779b97f4a7c15;
    uint64_t a = *state;
    a = (a ^ (a >> 30)) * 0xbf58476d1ce4e5b9;
    a = (a ^ (a >> 27)) * 0x94d049bb133111eb;
    return min + (a ^ (a >> 31)) % (max - min);
}

uint64_t prand_uint(uint64_t min, uint64_t max)
{
    return prand_uint_s(&__prand_state, min, max);
}

int64_t prand_int_s(prand_state* state, int64_t min, int64_t max)
{
    *state += 0x9e3779b97f4a7c15;
    uint64_t a = *state;
    a = (a ^ (a >> 30)) * 0xbf58476d1ce4e5b9;
    a = (a ^ (a >> 27)) * 0x94d049bb133111eb;
    return min + (int64_t)((a ^ (a >> 31)) % (uint64_t)(max - min));
}

int64_t prand_int(int64_t min, int64_t max)
{
    return prand_uint_s(&__prand_state, min, max);
}

double prand_double_s(prand_state* state, double min, double max)
{
    return min + (double)prand_int_s(state, 0, INT64_MAX) / (double)INT64_MAX * (max - min);
}

double prand_double(double min, double max)
{
    return prand_double_s(&__prand_state, min, max);
}

float prand_float_s(prand_state* state, float min, float max)
{
    return min + (float)prand_int_s(state, 0, INT32_MAX) / (float)INT32_MAX * (max - min);
}

float prand_float(float min, float max)
{
    return prand_float_s(&__prand_state, min, max);
}

#endif

#endif
