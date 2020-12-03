ZERORISCY=1
MICRORISCY=0
RISCY_FPU=0

PROGRAM=helloworld

ROOT=$(shell pwd)
BUILD_DIR=$(ROOT)/build/
PREFIX=$(ROOT)/prefix/

.PHONY: tools tool-folders gdb gnu-toolchain jtag_bridge submodules
tools: submodules tool-folders gdb gnu-toolchain jtag_bridge

submodules:
	git config submodule."gdb".update none
	git submodule update --init
	cd riscv-gnu-toolchain && \
		git config submodule."qemu".update none && \
		git config submodule."riscv-gdb".update none  && \
		git submodule update --init

tools-clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(PREFIX)

tool-folders:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(PREFIX)

gnu-toolchain:
	mkdir -p $(BUILD_DIR)
	cd $(BUILD_DIR) && $(ROOT)/riscv-gnu-toolchain/configure \
		--prefix=$(PREFIX) \
		--disable-gdb \
		--with-arch=rv32g \
		--with-abi=ilp32 \
		&& make -j16

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
