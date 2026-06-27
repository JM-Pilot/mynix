ENTRY(k_entry)

SECTIONS 
{
	. = 2M;
	_kernel_start = .;

	.text BLOCK(4K) : ALIGN(4K)
	{
		KEEP(*(.multiboot_hdr))
		*(.text)
	}

	.rodata BLOCK(4K) : ALIGN(4K)
	{
		*(.rodata)
	}

	.data BLOCK(4K) : ALIGN(4K)
	{
		*(.data)
	}

	.bss BLOCK(4K) : ALIGN(4K)
	{
		*(COMMON)
		*(.bss)
	}
	_kernel_end = .;
}