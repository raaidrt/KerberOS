#include <logger.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdio.h>
#include <bufwriter.h>
#include <serial.h>

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

void dbg_logf(const char* restrict format, ...) {
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
    fprintf(&buffer, format, parameters);
    return;
}