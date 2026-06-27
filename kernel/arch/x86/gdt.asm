; SPDX-License-Identifier: GPL-2.0-only
; kernel/arch/x86/gdt.asm - loads the gdt provided in gdt.c
; Copyright (c) 2026 JM-Pilot

[BITS 32]
section .text
global gdt_load

gdtr:	dw 0
		dd 0
gdt_load:
	; load the size first
	mov ax, [esp + 4]
	mov [gdtr], ax

	; then offset
	mov eax, [esp + 8]
	mov [gdtr + 2], eax

	; now load
	lgdt [gdtr]

	; move 0x10 (kernel data) to segment registers
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov fs, ax
	mov gs, ax
	mov ss, ax

	jmp 0x8:.reload_cs
.reload_cs:
	ret
