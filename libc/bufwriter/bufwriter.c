#include <bufwriter.h>

void flush(struct Buffer *buffer) {
    (buffer->writen)(buffer->buf, buffer->n_written);
    buffer->n_written = 0;
}

void buffer_putchar(struct Buffer *buffer, char c) {
    if (buffer->n_written >= buffer->buf_len) {
        flush(buffer);
    }

    buffer->buf[buffer->n_written] = c;
    buffer->n_written++;

    if (c == '\n') {
        flush(buffer);
    }
    
}