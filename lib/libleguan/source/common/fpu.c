 /* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  *          _                                                      *
  *         | |                                                     *
  *         | |     ___  __ _ _   _  __ _ _ __                      *
  *         | |    / _ \/ _` | | | |/ _` | '_ \                     *
  *         | |___|  __/ (_| | |_| | (_| | | | |                    *
  *         \_____/\___|\__, |\__,_|\__,_|_| |_|                    *
  *        ============= __/ | ==================                   *
  *                     |___/           BFH 2021                    *
  *                                                                 *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
  * This software can be used by students and other personal of the *
  * Bern University of Applied Sciences under the terms of the MIT  *
  * license.                                                        *
  * For other persons this software is under the terms of the GNU   *
  * General Public License version 2.                               *
  *                                                                 *
  * Copyright &copy; 2021, Bern University of Applied Sciences.     *
  * All rights reserved.                                            *
  * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/**
  *  @file fpu.c
  *  @ingroup common
  *  @author Nikolaij Saegesser
  *  @brief Floating Point Unit access
  */

#include "common/fpu.h"

#include "common/processor.h"

float32_t FPU_abs32(float32_t x) {
    float32_t result;
    asm volatile("vabs.F32 %[result], %[input]" : [result] "=t"(result) : [input] "t"(x) : "cc");

    return result;
}

float64_t FPU_abs64(float64_t x) {
    float64_t result;
    asm volatile("vabs.F64 %P[result], %P[input]" : [result] "=w"(result) : [input] "w"(x) : "cc");

    return result;
}

float32_t FPU_sqrt32(float32_t x) {
    float32_t result;
    asm volatile("vsqrt.F32 %[result], %[input]" : [result] "=t"(result) : [input] "t"(x) : "cc");

    return result;
}

float64_t FPU_sqrt64(float64_t x) {
    float64_t result;
    asm volatile("vsqrt.F64 %P[result], %P[input]" : [result] "=w"(result) : [input] "w"(x) : "cc");

    return result;
}
