#
# Makefile for mt32sound.dll
#

TARGET		= mt32sound.dll

#ARCH		= i686-pc-mingw32-
ARCH		= i686-w64-mingw32-
#ARCH		= x86_64-w64-mingw32-

CXX		= $(ARCH)g++
CXXFLAGS	= -Wall -O2
LDFLAGS		= -shared -static -Wl,--enable-stdcall-fixup -L./munt/mt32emu
LDLIBS		= -lmt32emu -lwinmm
RANLIB		= $(ARCH)ranlib
STRIP		= $(ARCH)strip
DEPEND		= $(ARCH)g++ -MM
IMPLIB		= lib
IMPLIBFLAGS	= /MACHINE:IX86

#CXXFLAGS	+= -DDEBUG_WITH_MIDI
#CXXFLAGS	+= -DDEBUG_OUTPUT_LOG
#CXXFLAGS	+= -DDEBUG_OUTPUT_MT32EMU_LOG


.PHONY: all install clean depend mt32emu

all: $(TARGET)

install: $(TARGET)
	cp $(TARGET) ../../

mt32sound.dll: mt32sound.o mt32sound.def munt/mt32emu/libmt32emu.a
	$(CXX) $(LDFLAGS) mt32sound.def mt32sound.o $(LDLIBS) -o mt32sound.dll
	$(STRIP) mt32sound.dll

mt32sound.lib: mt32sound.dll mt32sound.def
	$(IMPLIB) $(IMPLIBFLAGS) /DEF:mt32sound.def /OUT:mt32sound.lib

mt32emu:
	cd munt/mt32emu; \
	rm -f CMakeCache.txt; \
	cmake -DCMAKE_CXX_COMPILER=$(CXX) -Dlibmt32emu_SHARED=FALSE -Dlibmt32emu_C_INTERFACE=FALSE; \
	cmake -DCMAKE_BUILD_TYPE:STRING=Release .; \
	make clean all; \
	$(RANLIB) libmt32emu.a

clean:
	-rm -f mt32sound.dll
	-rm -f mt32sound.o

depend:
	$(DEPEND) $(CXXFLAGS) mt32sound.cpp > .depend

-include .depend
