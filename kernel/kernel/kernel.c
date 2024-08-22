#include <stdio.h>

#include <kernel/gdt/gdt.h>
#include <kernel/global_values.h>
#include <kernel/idt/idt.h>
#include <kernel/interrupt/flag.h>
#include <kernel/irq/irq.h>
#include <kernel/multiboot/multiboot.h>
#include <kernel/time/time.h>
#include <kernel/tty.h>
#include <logger.h>
#include <serial.h>

#define __is_kerberos_kernel 1

extern char _kernel_virtual_start, _kernel_virtual_end, _kernel_physical_start,
    _kernel_physical_end;

void kernel_main(uintptr_t ebx) {
  koffset = 0xC0000000;
  multiboot_info_t *mbinfo = (multiboot_info_t *)(ebx + koffset);
  uint32_t mb_mem_lower = mbinfo->mem_lower;
  uint32_t mb_mem_upper = mbinfo->mem_upper;
  mem_lower = mb_mem_lower;
  mem_upper = mb_mem_upper;
  dbg_logf(INCLUDE_TIME, INFO, "Multiboot mem_lower = %x, mem_upper = %x\n",
           mb_mem_lower, mb_mem_upper);
  uintptr_t kstart = (uintptr_t)&_kernel_virtual_start;
  uintptr_t kend = (uintptr_t)&_kernel_virtual_end;
  kmem_physical_start = (uintptr_t)&_kernel_physical_start;
  kmem_physical_end = (uintptr_t)&_kernel_physical_end;
  int ksize = (int)kend - (int)kstart;
  dbg_logf(INCLUDE_TIME, INFO,
           "kernel start = %x, kernel end = %x, kernel size = %d\n", kstart,
           kend, ksize);
  dbg_logf(INCLUDE_TIME, INFO, "Initializing the terminal...");
  terminal_initialize();
  dbg_logf(EXCLUDE_TIME, NONE, "DONE\n");
  printf("Hello, kernel word %x!\n", 0x42);
  dbg_logf(INCLUDE_TIME, INFO, "Initializing the Global Descriptor Table...");
  gdt_initialize();
  dbg_logf(EXCLUDE_TIME, NONE, "DONE\n");
  dbg_logf(INCLUDE_TIME, INFO,
           "Initializing the Interrupt Descriptor Table...");
  idt_initialize();
  dbg_logf(EXCLUDE_TIME, NONE, "DONE\n");
  dbg_logf(INCLUDE_TIME, INFO, "Remapping the IRQ lines...");
  pic_remap(0x20, 0x2F);
  dbg_logf(EXCLUDE_TIME, NONE, "DONE\n");
  set_interrupt_flag();
}
