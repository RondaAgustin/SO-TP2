.PHONY: bootloader image collections kernel userland all clean toolchain

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

container:
	docker run -v "${PWD}:/root" --privileged -ti agodio/itba-so-multi-platform:3.0

container_gdb:
	docker run --add-host=host.docker.internal:host-gateway -v "${PWD}:/root" --privileged -ti agodio/itba-so-multi-platform:3.0

install_gdb:
	apt-get install gdb-multiarch

.PHONY: bootloader image collections kernel userland all clean
