SHELL = /bin/bash

SRC = $(wildcard *.cc)
ESP = $(wildcard printf*.s)
ASM = $(filter-out $(ESP), $(wildcard *.s))
OBJ = $(ASM:.s=.o)
EXE = $(basename $(ESP) $(ASM) $(SRC) sum0 sum3)
ATT = $(EXE:=.att) $(OBJ:=.att)

ASFLAGS = --32 -g
LDFLAGS = -melf_i386
CFLAGS = -m32 -Wall
CXXFLAGS = $(CFLAGS)

default: $(OBJ) $(EXE)

all: default $(ATT)

clean:
	$(RM) $(ATT) $(EXE) $(OBJ) core.* *~

hola64.o: ASFLAGS=--64 -g

hola64: LDFLAGS=-melf_x86_64

sum0: CXXFLAGS+=-O0

sum3: CXXFLAGS+=-O3

%.o: %.s
	$(AS) $(ASFLAGS) $< -o $@

%: %.o
	$(LD) $(LDFLAGS) $< -o $@

%: %.s
	$(CC) $(CFLAGS) $< -o $@

%: %.c
	$(CC) $(CFLAGS) $< -o $@

%: %.cc
	$(CXX) $(CXXFLAGS) $< -o $@

%.att: %
	objdump -C -d $< > $@
