all: bootloader toolchain kernel userland image

bootloader:
	cd Bootloader; make all

kernel:
	cd Kernel; make all

userland:
	cd Userland; make all

toolchain:
	cd Toolchain; make all

image:
	cd Image; make all

clean:
	cd Bootloader; make clean
	cd Image; make clean
	cd Kernel; make clean
	cd Toolchain; make clean
	cd Userland; make clean

.PHONY: bootloader image collections kernel userland all clean
