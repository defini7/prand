/*---------------------------------------------------------------------------------------------
 *  Copyright 2026 defini7. All rights reserved.
 *  Licensed under the MIT License. See LICENSE in the project root for license information.
 *--------------------------------------------------------------------------------------------*/

/* NOTE: These tests were written with AI assistance. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>

#define PRAND_IMPL
#include "prand.h"

/* simple helpers */
static void expect_range_u64(uint64_t v, uint64_t min, uint64_t max) {
    assert(v >= min && v < max);
}
static void expect_range_i64(int64_t v, int64_t min, int64_t max) {
    assert(v >= min && v < max);
}
static void expect_range_double(double v, double min, double max) {
    assert(v >= min && v < max);
}
static void expect_range_float(float v, float min, float max) {
    assert(v >= min && v < max);
}

/* Test 1: basic ranges for many samples */
static void test_ranges(void) {
    for (uint64_t i = 0; i < 10000; ++i) {
        uint64_t u = prand_uint(0, 1000);
        expect_range_u64(u, 0, 1000);

        int64_t s = prand_int(-500, 500);
        expect_range_i64(s, -500, 500);

        double d = prand_double(0.0, 1.0);
        expect_range_double(d, 0.0, 1.0);

        float f = prand_float(-1.0f, 1.0f);
        expect_range_float(f, -1.0f, 1.0f);
    }
}

/* Test 2: edge min+1 == max should always return min */
static void test_min_plus_one(void) {
    for (int i = 0; i < 1000; ++i) {
        assert(prand_uint(5, 6) == 5);
        assert(prand_int(10, 11) == 10);
    }
}

/* Test 3: reproducibility using manual state */
static void test_reproducible_sequence(void) {
    prand_state s1 = 12345;
    prand_state s2 = 12345;
    for (int i = 0; i < 1000; ++i) {
        uint64_t a = prand_uint_s(&s1, 0, 0xFFFFFFFF);
        uint64_t b = prand_uint_s(&s2, 0, 0xFFFFFFFF);
        assert(a == b);
    }
}

/* Test 4: global-state deterministic behavior: reset global and check repeatability */
static void test_global_repeatable(void) {
    __prand_state = 0xDEADBEEF;
    uint64_t seq1[16];
    for (int i = 0; i < 16; ++i) seq1[i] = prand_uint(0, 1000000);

    __prand_state = 0xDEADBEEF;
    for (int i = 0; i < 16; ++i) {
        uint64_t v = prand_uint(0, 1000000);
        assert(v == seq1[i]);
    }
}

/* Test 5: sanity checks for double/float endpoints */
static void test_double_float_endpoints(void) {
    for (int i = 0; i < 1000; ++i) {
        double d = prand_double(5.0, 5.0 + 1e-12);
        assert(d >= 5.0 && d < 5.0 + 1e-12);

        float f = prand_float(-2.0f, -2.0f + 1e-6f);
        assert(f >= -2.0f && f <= -2.0f + 1e-6f);
    }
}

/* Test 6: check that signed wrapper didn't accidentally call wrong function */
static void test_signed_wrapper(void) {
    prand_state st = 1;
    int64_t a = prand_int_s(&st, -100, 100);
    assert(a >= -100 && a < 100);
}

/* Run all tests */
int main(void) {
    printf("Starting prand tests...\n");
    test_ranges();
    printf("test_ranges OK\n");
    test_min_plus_one();
    printf("test_min_plus_one OK\n");
    test_reproducible_sequence();
    printf("test_reproducible_sequence OK\n");
    test_global_repeatable();
    printf("test_global_repeatable OK\n");
    test_double_float_endpoints();
    printf("test_double_float_endpoints OK\n");
    test_signed_wrapper();
    printf("test_signed_wrapper OK\n");
    printf("All tests passed.\n");
    return 0;
}
