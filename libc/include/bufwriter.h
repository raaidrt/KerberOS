#ifndef _BUFWRITER_H 
#define _BUFWRITER_H 1
#include <stddef.h>

#ifdef __cplusplus 
extern "C" { 
#endif
typedef size_t (*fn_writen)(char*, size_t);

/** 
 *  Buffer - a data structure that stores a char pointer `buf`, as well as 
 *  the number of characters that have been written to in the buffer. 
 *  It also wrotes a function `writen` that is capable of writing 
 *  a specified number of bytes from a buffer. 
 *  
 *  Invariants: 
 *  - `n_written` should always be at most `buf_len` 
 *  - `buf_len` > 0
*/
struct Buffer {
	size_t buf_len;
    size_t n_written;
    char *buf;
    fn_writen writen;
};

/** 
 * flush - Flushes the buffer with stdout
 * @buffer: the buffer that needs to be flushed
 *
 * After flush is called, `buffer->n_written` will be 0.
*/
void flush(struct Buffer *buffer);

/**
 *  putchar Pushes a character into the buffer
 *  @buffer: the buffer into which we push the character
 *  @c: the character that needs to be pushed. If the character is `'\n'`, then 
 *      the buffer will be flushed after the character has been pushed to the buffer. 
 *
 *  If, prior to putchar we have that `buffer->n_written >= buffer->buf_len`, 
 *  then the buffer will be flushed before the character is pushed
*/
void buffer_putchar(struct Buffer *buffer, char c);

#ifdef __cplusplus
}
#endif

#endif // _BUFWRITER
