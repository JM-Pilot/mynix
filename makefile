MAKEFLAGS += --no-print-directory -j$(nproc)
BIN = $(shell pwd)/bin
ISO_BUILD = $(BIN)/iso_build
QEMU_FLAGS = \
	-no-reboot \
	-no-shutdown \
	-m 128M \
	-d int,cpu_reset -D $(BIN)/QEMU_LOG.txt \
	-serial stdio

$(BIN)/mynix_kernel.elf:
	@$(MAKE) -C kernel BIN="$(BIN)"
	@if grub-file --is-x86-multiboot2 $@; then \
		echo "Kernel is multboot"; \
	else \
		echo "Kernel is not multboot"; \
		exit 1; \
	fi

$(BIN)/mynix.iso: $(BIN)/mynix_kernel.elf
	@echo "Making ISO"
	@mkdir -p $(ISO_BUILD)/boot/grub
	@cp kernel/boot/grub.cfg $(ISO_BUILD)/boot/grub
	@cp $< $(ISO_BUILD)/
	@grub-mkrescue -o $@ $(ISO_BUILD) >/dev/null 2>&1

$(BIN):
	mkdir -p $(BIN)
	
run: $(BIN)/mynix.iso
	@echo "Running on QEMU"
	@qemu-system-i386 --cpu pentium3 -cdrom $< $(QEMU_FLAGS)
	@echo "Logs are in bin/QEMU_LOG.txt"
clean:
	@rm -rf $(BIN)