.DEFAULT_GOAL = pacific

top_dir = $(shell pwd)

CROSS_AARCH64_GCC = aarch64-linux-gnu-gcc-8
CROSS_AARCH64_QEMU = $(top_dir)/install/bin/qemu-aarch64

ENTRY_ADDR = LHelloWorld_3B_7Cmain_7C_28ALjava_2Flang_2FString_3B_29V

V ?= 0

Q ?= @

aarch64-qemu: $(top_dir)/install/bin/qemu-aarch64
	bash $(top_dir)/script/build-qemu-aarch64.sh

pacific:
	$(CROSS_AARCH64_GCC) -O2 \
	-Wl,-rpath=$(top_dir)/prebuilt/aarch64 \
	-Wl,-dynamic-linker=$(top_dir)/prebuilt/aarch64/ld-linux-aarch64.so.1 \
	$(top_dir)/src/pacific.c -o $(top_dir)/src/pacific

sample:
	$(Q)$(CROSS_AARCH64_GCC) -c $(top_dir)/sample/HelloWorld.VtableImpl.s -o $(top_dir)/sample/HelloWorld.VtableImpl.o
	$(Q)$(CROSS_AARCH64_QEMU) $(top_dir)/src/pacific $(top_dir)/sample/HelloWorld.VtableImpl.o $(ENTRY_ADDR) $(V)

help:
	@echo "The following make targets are supported:"
	@echo "  <null>:         build toy runtime"
	@echo "  aarch64-qemu:   build cross qemu"
	@echo "  sample:         run HelloWorld sample generated from ark compiler"
	@echo "  help:           show help"
	@echo ""
	@echo "NOTE: please make sure you have installed aarch64 linux gnu cross toolchain"
	@echo "      for example, please \"apt-get install gcc-8-aarch64-linux-gnu\" on ubuntu 18.04"

clean:
	rm -f $(top_dir)/src/pacific
	rm -f $(top_dir)/sample/HelloWorld.VtableImpl.o

prepare-for-public:
	git archive --format=tar.gz \
		-o "../pacific-`git describe`.tar.gz" \
		--prefix=pacific/ HEAD

.PHONY: help aarch64-qemu pacific sample clean prepare-for-public
