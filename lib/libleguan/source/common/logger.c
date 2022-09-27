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
  *  @file logger.c
  *  @ingroup common
  *  @author Nikolaij Saegesser
  *  @brief Logger implementation
  */

#include "common/logger.h"

#include <stdarg.h>
#include <stdio.h>

static LOG_Stream_t m_stream;

void LOG_SetDestination(LOG_Stream_t stream) {
	m_stream = stream;
}

LOG_Stream_t LOG_GetDestination(void) {
	return m_stream;
}


static void LOG_VPrint(bool new_line, const char *fmt, va_list args) {
	if (m_stream == NULL) return;

	char buffer[LOGGER_MAX_CHARACTERS + 1];
	vsnprintf(buffer, LOGGER_MAX_CHARACTERS, fmt, args);
	m_stream(buffer, new_line);
}

void LOG_Print(bool new_line, const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	LOG_VPrint(new_line, fmt, args);
	va_end(args);
}

void LOG_Debug(const char *fmt, ...) {
	#if defined(DEBUG)

		va_list args;
		va_start(args, fmt);
		LOG_Print(false, "[DEBUG] ");
		LOG_VPrint(true, fmt, args);
		va_end(args);

	#endif
}

void LOG_Info(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	LOG_Print(false, "[INFO]  ");
	LOG_VPrint(true, fmt, args);
	va_end(args);
}

void LOG_Warn(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	LOG_Print(false, "[WARN]  ");
	LOG_VPrint(true, fmt, args);
	va_end(args);
}

void LOG_Error(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	LOG_Print(false, "[ERROR] ");
	LOG_VPrint(true, fmt, args);
	va_end(args);
}

void LOG_Fatal(const char *fmt, ...) {
	va_list args;
	va_start(args, fmt);
	LOG_Print(false, "[FATAL] ");
	LOG_VPrint(true, fmt, args);
	va_end(args);
}
