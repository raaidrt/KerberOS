#include <logger.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <bufwriter.h>
#include <serial.h>
#include <kernel/time/time.h>

static int serial_initialized = 0;

size_t log_writen(char *src, size_t n) {
	size_t n_written = 0;
	while (n_written < n) {	
		if (src[n_written] == '\0') {
			return n_written;
		}
		serial_putchar((int) src[n_written]);
		n_written++;
	}
	return n_written;
}

void buffer_write(struct Buffer *buffer, const char *restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);
	fprintf(buffer, format, parameters);
}

void dbg_logf(enum timing timing, enum log_kind kind, const char* restrict format, ...) {


	if (serial_initialized == 0) {
		serial_initialized = 1;
		serial_initialize();
	}
	va_list parameters;
	va_start(parameters, format);

	// Setup buffer
	size_t buf_len = 56;
	char buf[buf_len];
	struct Buffer buffer;
	buffer.buf = buf;
	buffer.buf_len = buf_len;
	buffer.n_written = 0;
	buffer.writen = &log_writen;
	
	if (timing == INCLUDE_TIME) {
		char time_buffer[20];
		iso_8601_format(time_buffer, get_time());

		// write the time
		buffer_write(&buffer, "[%s] ", time_buffer);
	}
	
	char *kind_str;
	switch (kind) {
	case WARN:
		kind_str = "[WARN] ";
		break;
	case DEBUG:
		kind_str = "[DEBUG] ";
		break;
	case INFO:
		kind_str = "[INFO] ";
		break;
	case ERROR:
		kind_str = "[ERROR] ";
		break;
	default:
		kind_str = "";
	}
	buffer_write(&buffer, "%s", kind_str);

	fprintf(&buffer, format, parameters);

	return;
}


