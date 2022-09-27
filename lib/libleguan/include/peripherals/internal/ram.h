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
  *  @file ram.h
  *  @ingroup peripherals
  *  @author Nikolaij Saegesser
  *  @brief RAM Memory definitions
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Includes */
#include "common/types.h"
#include "common/attributes.h"
#include "common/mmio.h"

/* Macros */
#define RAM_DTCMRAM_BASE    0x20000000
#define RAM_D1_BASE         0x24000000
#define RAM_D2_BASE         0x30000000
#define RAM_D3_BASE         0x38000000
#define RAM_ITCMRAM_BASE    0x00000000

#define RAM_DTCMRAM         MAP(u8, DTCMRAM_BASE)
#define RAM_D1              MAP(u8, RAM_D1_BASE)
#define RAM_D2              MAP(u8, RAM_D2_BASE)
#define RAM_D3              MAP(u8, RAM_D3_BASE)
#define RAM_ITCMRAM         MAP(u8, ITCMRAM_BASE)


#if defined(__cplusplus)
}
#endif
