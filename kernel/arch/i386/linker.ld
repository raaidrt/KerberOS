/* The bootloader will look at this image and start execution at the symbol
   designated at the entry point. */
ENTRY(_loader)
OUTPUT_FORMAT(elf32-i386)

/* Tell where the various sections of the object files will be put in the final
   kernel image. */
SECTIONS
{
	/* Begin putting sections at 1 MiB, a conventional place for kernels to be
	   loaded at by the bootloader. */
	. = 1M;

	_kernel_physical_start = .;
	_kernel_virtual_start = . + 0xC0000000;

	/* First put the multiboot header, as it is required to be put very early
	   early in the image or the bootloader won't recognize the file format.
	   Next we'll put the .text section. */
	.multiboot BLOCK(4K) : ALIGN(4K)
	{
		*(.multiboot)
		*(.multiboot.text)
	}

	. += 0xC0000000;

	/* Read-only data. */
	.text ALIGN(4K) : AT(ADDR(.text) - 0xC0000000) {
		*(.text)
	}

	.rodata ALIGN(4K) : AT(ADDR(.rodata) - 0xC0000000)
	{
		*(.rodata)
	}

	/* Read-write data (initialized) */
	.data ALIGN(4K) : AT(ADDR(.data) - 0xC0000000)
	{
		*(.data)
		*(.tdata)
	}

	/* Read-write data (uninitialized) and stack */
	.bss ALIGN(4K) : AT(ADDR(.bss) - 0xC0000000)

	{
		*(COMMON)
		*(.bss)
		*(.tbss)
	}
	_kernel_physical_end = . - 0xC0000000;
	_kernel_virtual_end = .;
}
