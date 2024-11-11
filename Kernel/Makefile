include Makefile.inc

KERNEL=kernel.bin


ifeq ($(MEMORY_MANAGER), buddy_memory_manager)
	MEMORY_MANAGER_SRC=memory_manager/buddy/buddy_memory_manager.c
	MEMORY_MANAGER_DIR=memory_manager/buddy

else
	MEMORY_MANAGER_SRC=memory_manager/bitmap/bitmap_memory_manager.c
	MEMORY_MANAGER_DIR=memory_manager/bitmap
endif

MEMORY_MANAGER_SRC=memory_manager/$(MEMORY_MANAGER).c

SOURCE_DIRS=. drivers interrupt_handlers utils scheduler process synchro ipc $(MEMORY_MANAGER_DIR)
SOURCES=$(wildcard $(addsuffix /*.c, $(SOURCE_DIRS)))
SOURCES_ASM=$(wildcard asm/*.asm synchro/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -g -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)
	$(LD) $(LDFLAGS) -g -T kernel.ld --oformat=elf64-x86-64 -o kernel.elf $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf memory_manager/*/*.o memory_manager/*.o asm/*.o drivers/*.o interrupt_handlers/*.o scheduler/*.o process/*.o synchro/*.o ipc/*.o *.o *.bin *.elf */*.o */*/*.o

.PHONY: all clean
