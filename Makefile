ZERORISCY=1
MICRORISCY=0
RISCY_FPU=0

USE_LLVM=0
#PROGRAM=helloworld
PROGRAM=led-bmi160-demo


JTAG_BRIDGE=$(PREFIX)/bin/jtag_bridge
RISCV_GDB=$(PREFIX)/bin/riscv64-unknown-elf-gdb

PROGRAM_DIR=$(ROOT)/build/sw/apps/$(PROGRAM)

ROOT=$(shell pwd)
BUILD_DIR=$(ROOT)/build/
PREFIX=$(ROOT)/prefix/

.PHONY: tools tool-folders gdb gnu-toolchain jtag_bridge submodules

ifeq ($(USE_LLVM), 1)
tools: submodules tool-folders gdb gnu-toolchain llvm-toolchain jtag_bridge software-env

submodules:
	git config submodule."riscv-llvm".update none  && \
	git submodule update --init
	cd riscv-gnu-toolchain && \
		git config submodule."qemu".update none && \
		git config submodule."riscv-gdb".update none  && \
		git submodule update --init

else 
tools: submodules tool-folders gdb gnu-toolchain jtag_bridge software-env

submodules:
	git config submodule."riscv-llvm".update none  && \
	git submodule update --init
	cd riscv-gnu-toolchain && \
		git config submodule."qemu".update none && \
		git config submodule."riscv-gdb".update none  && \
		git submodule update --init
endif


tools-clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(PREFIX)

tool-folders:
	mkdir -p $(BUILD_DIR)
	mkdir -p $(PREFIX)

gnu-toolchain:
	mkdir -p $(BUILD_DIR)/gnu-toolchain
	cd $(BUILD_DIR)/gnu-toolchain && $(ROOT)/riscv-gnu-toolchain/configure \
		--prefix=$(PREFIX) \
		--disable-gdb \
		--with-arch=rv32g \
		--with-abi=ilp32 \
		&& make -j`nproc`

llvm-toolchain:
	mkdir -p $(BUILD_DIR)/llvm/
	ln -s $(ROOT)/riscv-llvm/clang $(ROOT)/riscv-llvm/llvm/tools || true
	cd $(BUILD_DIR)/llvm && \
		cmake -DCMAKE_BUILD_TYPE="Release" \
			  -DBUILD_SHARED_LIBS=True -DLLVM_USE_SPLIT_DWARF=True \
			  -DCMAKE_INSTALL_PREFIX="$(PREFIX)" \
			  -DLLVM_OPTIMIZED_TABLEGEN=True -DLLVM_BUILD_TESTS=False \
			  -DDEFAULT_SYSROOT="$(PREFIX)/riscv64-unknown-elf" \
			  -DLLVM_DEFAULT_TARGET_TRIPLE="riscv64-unknown-elf" \
			  -DLLVM_TARGETS_TO_BUILD="RISCV" \
			  $(ROOT)/riscv-llvm/llvm/ 
	cd $(BUILD_DIR)/llvm/ && make -j`nproc` install

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

software-env:
	mkdir -p $(BUILD_DIR)/sw
	cd $(BUILD_DIR)/sw/ && \
		PREFIX=$(PREFIX)/bin/ $(ROOT)/sw/cmake_configure.zeroriscy.gcc.sh

software: software-env
	cd $(BUILD_DIR)/sw && make $(PROGRAM).elf

GDB_UPLOAD_ARGS ?= --batch

GDB_UPLOAD_CMDS += -ex "target remote localhost:1234"
GDB_UPLOAD_CMDS += -ex "load"
GDB_UPLOAD_CMDS += -ex "detach"
GDB_UPLOAD_CMDS += -ex "quit"

upload:
	$(JTAG_BRIDGE) 1>/dev/null & \
	$(RISCV_GDB) $(PROGRAM_DIR)/$(PROGRAM).elf $(GDB_UPLOAD_ARGS) $(GDB_UPLOAD_CMDS)
