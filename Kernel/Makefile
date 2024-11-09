include Makefile.inc

KERNEL=kernel.bin

SOURCE_DIRS=. drivers interrupt_handlers memory_manager/buddy utils scheduler process synchro ipc
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
	rm -rf memory_manager/*/*.o memory_manager/*.o asm/*.o drivers/*.o interrupt_handlers/*.o scheduler/*.o process/*.o synchro/*.o ipc/*.o *.o *.bin *.elf */*.o */*/*.o

.PHONY: all clean
