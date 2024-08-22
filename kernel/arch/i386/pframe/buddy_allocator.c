#include <kernel/global_values.h>
#include <kernel/pframe/buddy_allocator.h>
#include <panic.h>
#include <stddef.h>

/**
 * Calculates the offset of the page frame node of order [order] with index [i]
 * in [pf_blocks]
 *
 * @param order is the order of the page frame node
 * @param i is the index of the node relative to other nodes of the same index
 */
static inline size_t offset(enum pf_order order, size_t i) {
  switch (order) {
  case NUM_ORDERS:
    panic("offset function cannot be called with NUM_ORDERS");
    break;
  default:
    return i * (1 << order);
  }
}

/**
 * Initializes the initial page table with all page frames being coalesced into
 * blocks of size 4 MiB
 */
static void initialize_pf_without_kernel_location() {
  for (size_t i = 0; i < 1 << PF_4_MiB; i++) {
    struct pf_node pf;
    pf.free = true;
    pf.order = PF_4_MiB;
    pf.buddy = NULL;
    pf.next =
        i + 1 < 1 << PF_4_MiB ? &pf_blocks[offset(PF_4_MiB, i + 1)] : NULL;
    pf_blocks[offset(PF_4_MiB, i)] = pf;
    for (int i = 0; i < NUM_ORDERS; i++) {
      blocks_ll[i] = NULL;
    }
    blocks_ll[PF_4_MiB] = &pf_blocks[offset(PF_4_MiB, 0)];
  }
}

/**
 * Adds the page frame [pf] to the list of kernel page frames
 *
 * @param pf is the page frame node to add
 */
static void add_kernel_pf(struct pf_node *pf, uintptr_t base) {
  pf->free = false;
  pf->base = base;
  if (kernel_pf == NULL) {
    kernel_pf = pf;
    pf->next = NULL;
  } else {
    pf->next = kernel_pf;
    kernel_pf = pf;
  }
}

/**
 * Removes the page frame node [pf] from the list of free page frames in
 * [blocks_ll]
 */
static void remove_pf_ll(struct pf_node *pf) {
  if (blocks_ll[pf->order] == pf) {
    blocks_ll[pf->order] = pf->next;
    return;
  }
  struct pf_node *pfp = blocks_ll[pf->order];
  while (pfp->next != pf)
    pfp = pfp->next;
  pfp->next = pf->next;
}

/**
 * Reserves kernel page frames within the lower bound of [pf_lb] and the upper
 * bound of [pf_ub].
 *
 * @param pf_lb is the lower bound of page frame memory
 * @param pf_ub is the upper bound of page frame memory
 * @param order is the current order of page frame being looked at
 * @param i is the index of the page frame entry corresponding to the order
 * @param pf is a pointer to the current page frame being considered
 */
static void reserve_kernel_pf_bounds(size_t pf_lb, size_t pf_ub,
                                     enum pf_order order, size_t i,
                                     struct pf_node *pf, uintptr_t base) {
  if (mem_lower <= pf_lb && pf_ub < mem_upper) {
    add_kernel_pf(pf, base);
    return;
  }
  if (!(mem_upper < pf_lb || pf_ub <= mem_lower)) {
    if (order == PF_4_KiB) {
      add_kernel_pf(pf, base);
      return;
    }
    size_t mid = pf_lb + (pf_ub - pf_lb) / 2;
    size_t i1 = i * 2;
    size_t i2 = i1 + 1;
    size_t offset1 = offset(order - 1, i1);
    size_t offset2 = offset(order - 1, i2);
    pf_blocks[offset1].order = order - 1;
    pf_blocks[offset2].order = order - 1;
    pf_blocks[offset1].buddy = &pf_blocks[offset2];
    pf_blocks[offset2].buddy = &pf_blocks[offset2];
    uintptr_t base_offset = 1 << (order - 1 + 12);
    reserve_kernel_pf_bounds(pf_lb, mid, order - 1, i1, &pf_blocks[offset1],
                             base);
    reserve_kernel_pf_bounds(mid, pf_ub, order - 1, i2, &pf_blocks[offset2],
                             base + base_offset);
  }
}

/**
 * Reserves page frames for kernel memory
 */
static void reserve_kernel_pf() {
  for (size_t i = 0; i < MEM_4_GiB / (1 << (PF_4_MiB + 12)); i++) {
    size_t lower_bound = offset(PF_4_MiB, i) << 12;
    size_t upper_bound = offset(PF_4_MiB, i + 1) << 12;
    size_t off = offset(PF_4_MiB, i);
    reserve_kernel_pf_bounds(lower_bound, upper_bound, PF_4_MiB, i,
                             &pf_blocks[off], koffset);
  }
}

void initialize() {
  initialize_pf_without_kernel_location();
  reserve_kernel_pf();
}
struct pf_node *pf_allocate(enum pf_order order) {
  (void)order;
  return NULL;
}
void pf_free(struct pf_node *pf) { (void)pf; }
