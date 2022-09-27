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
  *  @file logger.h
  *  @ingroup common
  *  @author Nikolaij Saegesser
  *  @brief Logger
  */

#pragma once

#if defined(__cplusplus)
extern "C" {
#endif


/* Includes */
#include "common/types.h"

/* Macros */
#define LOGGER_MAX_CHARACTERS   0x200

/* Types */
typedef void (*LOG_Stream_t)(const char *data, bool new_line);

/* Functions */

/**
 * @brief Sets the new destination where log output should go to
 * @param stream Stream function of destination
 */
void LOG_SetDestination(LOG_Stream_t stream);

/**
 * @brief Gets the currently set logging destination
 * @return Logging stream
 */
LOG_Stream_t LOG_GetDestination(void);


/**
 * @brief Prints a string to the current stream
 * @param new_line Print a new line after this string
 * @param fmt printf format string
 * @param ... Format arguments
 */
void LOG_Print(bool new_line, const char *fmt, ...);

/**
 * @brief Prints a debug string to the current stream
 * @note Only prints in debug mode
 * @param fmt printf format string
 * @param ... Format arguments
 */
void LOG_Debug(const char *fmt, ...);

/**
 * @brief Prints a info string to the current stream
 * @param fmt printf format string
 * @param ... Format arguments
 */
void LOG_Info(const char *fmt, ...);

/**
 * @brief Prints a warning string to the current stream
 * @param fmt printf format string
 * @param ... Format arguments
 */
void LOG_Warn(const char *fmt, ...);

/**
 * @brief Prints a error string to the current stream
 * @param fmt printf format string
 * @param ... Format arguments
 */
void LOG_Error(const char *fmt, ...);

/**
 * @brief Prints a fatal error string to the current stream
 * @param fmt printf format string
 * @param ... Format arguments
 */
void LOG_Fatal(const char *fmt, ...);


#if defined(__cplusplus)
}
#endif
