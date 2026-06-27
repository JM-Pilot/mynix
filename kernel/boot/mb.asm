; SPDX-License-Identifier: GPL-2.0-only
; kernel/boot/mb.asm Provides important stuff to the kernel

[BITS 32]

section .text.multiboot_hdr
	align 8
hdr_start:
	dd 0xE85250D6
	dd 0
	dd hdr_end - hdr_start
	dd 0x100000000 - (0xE85250D6 + 0 + (hdr_end - hdr_start))

; we want framebuffer cus we cool
framebuffer_tag_start:
	dw 5 	; defined in kernel/include/lib/multiboot.h line 74
	dw 0
	dd 1024
	dd 768
	dd 32
framebuffer_tag_end:
	dw 0
	dw 0
	dd 8
hdr_end:

section .bss
stack_bottom:
	resb 16384
stack_top:

section .text
global k_entry
k_entry:
	mov esp, stack_top

	extern kernel_main
	; pointer to multiboot info
	push ebx
	; magic 
	push eax 
	call kernel_main

; we just add this incase someone removed hang() in kernel_main
	cli
	hlt
	jmp $