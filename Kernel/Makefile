include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard [^_]*.c)
SOURCES_ASM=$(wildcard asm/*.asm)
OBJECTS=$(SOURCES:%.c=obj/%.o)
OBJECTS_ASM=$(SOURCES_ASM:asm/%.asm=obj/%.asm.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:%.asm=obj/%.asm.o)
STATICLIBS=

all: $(KERNEL)
$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

obj/%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -c $< -o $@

obj/%.asm.o : asm/%.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	mkdir obj
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf obj

.PHONY: all clean
