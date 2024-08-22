#ifndef _BUDDY_ALLOCATOR_H
#define _BUDDY_ALLOCATOR_H
#include <stdbool.h>
#include <stdint.h>

/*
 * Buddy Allocator System
 * ----------------------
 *  The buddy allocator system works by first reserving some memory where
 *  all the page frames can be allocated.
 *
 *  We initially start with all page frames existing as 4_MiB page frames.
 */

enum pf_order {
  NUM_ORDERS = 11,
  PF_4_MiB = 10,
  PF_2_MiB = 9,
  PF_1_MiB = 8,
  PF_512_KiB = 7,
  PF_256_KiB = 6,
  PF_128_KiB = 5,
  PF_64_KiB = 4,
  PF_32_KiB = 3,
  PF_16_KiB = 2,
  PF_8_KiB = 1,
  PF_4_KiB = 0
};

struct pf_node {
  bool free;
  enum pf_order order;
  uintptr_t base;
  struct pf_node *buddy;
  struct pf_node *next;
};

/* Note that a 32-bit system has 4GiB of Addressable Physical Memory */
#define MEM_4_GiB (1 << 22)

/** Set of all page frame blocks, both free and allocated */
struct pf_node pf_blocks[MEM_4_GiB / (1 << 12)];

/** Set of all free blocks */
struct pf_node *blocks_ll[NUM_ORDERS];
/** Blocks allocated for the kernel */
struct pf_node *kernel_pf;

/**
 * initialize - will remove excess page tables allocated for the kernel and
 * will set up the queue_sizes for each queue
 */
void initialize();

/**
 * pf_allocate - will allocate a collection of page frames with the
 * following size if it is available.
 *
 * @param size the size to allocate
 *
 * Returns the page frame node if the allocation succeeds, and NULL otherwise.
 * If the allocation fails then no memory will be allocated
 */
struct pf_node *pf_allocate(enum pf_order size);

/**
 * pf_free - will free the provided page frame
 *
 * @param pf is the page frame to free
 */
void pf_free(struct pf_node *pf);

#endif
