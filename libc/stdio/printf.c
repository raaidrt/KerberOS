#include <limits.h>
#include <stdbool.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <bufwriter.h>

enum PrintState { FORMAT_SPECIFIER, FORMAT_WRITE_ARG, CHAR_WRITE };
enum FormatWriteKind { NOT_FORMAT, INT, CHAR, STRING };

/** writen - writes `n` bytes to the display of the kernel
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

/** write_char - writes one character to the buffer
 * @buffer: the buffer to write to
 * @c: the character to write
 * 
 * Returns the number of bytes written, which will always be 1
 */
static size_t write_char(struct Buffer *buffer, char c) {
	buffer_putchar(buffer, c);
	return 1;
}

/** write_string - writes the string `s` to the buffer 
 * @buffer: the buffer to write to
 * @s: the string to write
 * 
 * Returns the number of bytes written.
*/
static size_t write_string(struct Buffer *buffer, const char *s) {
	size_t size_s = strlen(s);
	for (size_t i = 0; i < size_s; i++) {
		buffer_putchar(buffer, s[i]);
	}
	return size_s;
}

/** write_positive_int - writes a positive 32-bit integer to the buffer 
 * @buffer: the buffer to write to
 * @d: the positive integer to write
 * 
 * Returns the number of bytes written.
*/
static size_t write_positive_int(struct Buffer *buffer, int d) {
	if (d == 0) {
		return 0;
	}
	size_t result = write_positive_int(buffer, d / 10);
	write_char(buffer, "0123456789"[d % 10]);
	return result + 1;
}

/** write_int - writes a 32-bit integer to the buffer 
 * @buffer: the buffer to write to 
 * @d: the integer to write
 * 
 * Returns the number of bytes written.
*/
static size_t write_int(struct Buffer *buffer, int d) {
	if (d == INT_MIN) {
		const char *int_min_s = "-2147483648";
		return write_string(buffer, int_min_s);
	} else if (d == 0) {
		write_char(buffer, '0');
		return 1;
	}
	if (d < 0) {
		write_char(buffer, '-');
		return 1 + write_int(buffer, -d);
	}
	return write_positive_int(buffer, d);
}

int printf(const char* restrict format, ...) {
	// setup va_args
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

	char *ptr = (char *) format;
	enum PrintState state = CHAR_WRITE;
	enum FormatWriteKind write_kind = NOT_FORMAT;
	size_t total_written = 0;
	while (ptr[0] != '\0') {
		char c = ptr[0];
		switch (c) {
			// start of a new format specifier
		case '%':
			switch (state) {
				case FORMAT_SPECIFIER: // a second '%' character is seen
				state = CHAR_WRITE;
				break;
				case FORMAT_WRITE_ARG:
				case CHAR_WRITE:
				state = FORMAT_SPECIFIER; // switch to writing characters
			}
			break;
			// specifying the format of the argument
		case 'd':
			write_kind = INT;
			goto STATE_SWITCH;
		case 's':
			write_kind = STRING;
			goto STATE_SWITCH;
		case 'c':
			write_kind = CHAR;
		STATE_SWITCH:
			switch (state) {
			case FORMAT_SPECIFIER:
				state = FORMAT_WRITE_ARG;
				break;
			case FORMAT_WRITE_ARG:
			case CHAR_WRITE:
				state = CHAR_WRITE;
				write_kind = NOT_FORMAT;
			}
			break;
			// default case
		default:
			switch (state) {
				case FORMAT_SPECIFIER:
				return -1; // invalid format specifier
				case CHAR_WRITE:
				case FORMAT_WRITE_ARG:
				state = CHAR_WRITE;
				write_kind = NOT_FORMAT;
			}
		}
		size_t additional_written;
		switch (state) {
		case CHAR_WRITE:
			buffer_putchar(&buffer, c);
			total_written++;
			if (total_written > INT_MAX) {
				return -1; // overflow
			}
			break;

		case FORMAT_WRITE_ARG:
			additional_written = 0;
			switch (write_kind) {
			case INT:
				additional_written = write_int(&buffer, va_arg(parameters, int));
				break;
			case STRING:
				additional_written = write_string(&buffer, va_arg(parameters, const char *));
				break;
			case CHAR:
				additional_written = write_char(&buffer, (char) va_arg(parameters, int));
				break;
			case NOT_FORMAT:;
			}
			if (additional_written > INT_MAX - total_written) {
				return -1; // overflow
			}
			total_written += additional_written;
		case FORMAT_SPECIFIER:;
		}
		ptr++;
	}
	buffer_putchar(&buffer, '\0');
	flush(&buffer);
	va_end(parameters);
	return total_written;
}
