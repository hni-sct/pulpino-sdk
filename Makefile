ZERORISCY=1
MICRORISCY=0
RISCY_FPU=0


ROOT=$(shell pwd)
BUILD_DIR=$(ROOT)/build/
PREFIX=$(ROOT)/prefix/

.PHONY: tools tool-folders gdb jtag_bridge
tools: tool-folders gdb jtag_bridge

tools-clean: 
	rm -rf $(BUILD_DIR)
	rm -rf $(PREFIX)

tool-folders:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(PREFIX)

gdb:
	# gdb
	mkdir -p $(BUILD_DIR)/build-gdb/
	cd $(BUILD_DIR)/build-gdb && \
		$(ROOT)/gdb/configure --target=riscv64-unknown-elf \
		--prefix=$(PREFIX) \
		--disable-werror \
		--with-expat=yes \
		--enable-gdb \
		--disable-gas \
		--disable-binutils \
		--disable-ld \
		--disable-gold \
		--disable-gprof && make -j16 && make install

jtag_bridge:
	cd $(ROOT)/jtag_bridge && PREFIX=$(PREFIX)/bin make clean && PREFIX=$(PREFIX)/bin make

