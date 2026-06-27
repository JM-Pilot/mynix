CC = i686-elf-gcc
ASM = nasm
BIN = 

C_FLAGS = \
	-Wall \
	-Wextra \
	-nostdlib \
	-nostdinc \
	-ffreestanding \
	-O2 \
	-Iinclude/ \
	-Iinclude/lib \
	-std=c11 
LD_FLAGS = \
	-ffreestanding \
	-nostdlib \
	-T boot/linker.ld 

C_SRCS = $(shell find . -iname "*.c")
C_OBJS = $(patsubst %.c, $(BIN)/kernel/%.c.o, $(C_SRCS))

ASM_SRCS = $(shell find . -iname "*.asm") 
ASM_OBJS = $(patsubst %.asm, $(BIN)/kernel/%.asm.o, $(ASM_SRCS))

FONT_SRCS = $(shell find . -iname "*.psfu")
FONT_OBJS = $(patsubst %.psfu, $(BIN)/kernel/%.psfu.o, $(FONT_SRCS))
OBJS = $(C_OBJS) $(ASM_OBJS) $(FONT_OBJS)

$(BIN)/kernel/%.c.o: %.c
	@printf "CC $(notdir $@)\n"
	@mkdir -p $(dir $@)
	@$(CC) $(C_FLAGS) -c $< -o $@

$(BIN)/kernel/%.asm.o: %.asm
	@printf "NASM $(notdir $@)\n"
	@mkdir -p $(dir $@)
	@$(ASM) -f elf32 $< -o $@

$(BIN)/kernel/%.psfu.o: %.psfu
	@printf "OBJCOPY $(notdir $@)\n"
	@mkdir -p $(dir $@)
	@objcopy -O elf32-i386 -B i386 -I binary $< $@
#	@readelf -sW $@
$(BIN)/mynix_kernel.elf: $(OBJS)
	@$(CC) $(LD_FLAGS) $^ -o $@
	@printf "LD $(notdir $@)\n"