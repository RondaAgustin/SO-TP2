include Makefile.inc

KERNEL=kernel.bin

SOURCE_DIRS=. drivers interruptHandlers memoryManager memoryManager/test utils scheduler process synchro
SOURCES=$(wildcard $(addsuffix /*.c, $(SOURCE_DIRS)))
SOURCES_ASM=$(wildcard asm/*.asm synchro/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -g -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(MEMORY_MANAGER_SRC:.c=.o) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -g -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(MEMORY_MANAGER_SRC:.c=.o) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf memoryManager/*.o memoryManager/test/*.o asm/*.o drivers/*.o interruptHandlers/*.o scheduler/*.o process/*.o synchro/*.o *.o *.bin *.elf

.PHONY: all clean
