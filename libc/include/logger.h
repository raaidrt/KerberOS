#ifndef _LOGGER_H
#define _LOGGER_H 1

#ifdef DEBUG
#define _DEBUG_LOG 1
#endif

enum timing {
	INCLUDE_TIME,
	EXCLUDE_TIME
};

enum log_kind {
	WARN,
	DEBUG,
	INFO,
	ERROR,
	NONE
};

void dbg_logf(enum timing timing, enum log_kind kind, const char* restrict format, ...);

#endif
