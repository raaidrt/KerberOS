#include <stdarg.h>
#include <stdio.h>
#include <bufwriter.h>

/** 
 * writen - writes `n` bytes to the display of the kernel
 * @src: a `char` array from which we start writing
 * @n: the number of bytes we write
 * 
 * Note that if the nul character `'\0'` is encountered before `n` bytes 
 * are written, then the writing stops. 
 * 
 * Returns the number of bytes that have been written.
 */
size_t writen(char *src, size_t n) {
	size_t n_written = 0;
	while (n_written < n) {	
		if (src[n_written] == '\0') {
			return n_written;
		}
		putchar((int) src[n_written]);
		n_written++;
	}
	return n_written;
}



int printf(const char* restrict format, ...) {
	va_list parameters;
	va_start(parameters, format);

	// Setup buffer
	size_t buf_len = 56;
	char buf[buf_len];
	struct Buffer buffer;
	buffer.buf = buf;
	buffer.buf_len = buf_len;
	buffer.n_written = 0;
	buffer.writen = &writen;
	return fprintf(&buffer, format, parameters);
}
