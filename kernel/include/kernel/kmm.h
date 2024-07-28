#ifndef _KMM_H
#define _KMM_H

/**
 * kmalloc allocates a specific number of bytes on the heap and
 * returns a pointer to it
 *
 * @param size is the number of bytes to allocate
 */
void *kmalloc(size_t size);

/**
 * kcalloc allocates [elem_size * num_elems] bytes on the heap and
 * returns a pointer to it, with all the bytes being initialized to 0
 *
 * @param elem_size is the size of each element
 * @param num_elems is the number of elements
 *
 * kcalloc is intended to be used for allocating space for an 
 * array of [num_elems] elements, each with size [elem_size]
 */
void *kcalloc(size_t elem_size, size_t num_elems);

/**
 * kfree frees the memory that is pointed to by pointer on the heap.
 * Note that free must be called on a ptr that has been returned by
 * either kmalloc or kcalloc, and free must be called at most once on
 * such a pointer.
 *
 * @param ptr points to the location in the heap that needs to be free'd
 */
void kfree(void *ptr);

#endif
