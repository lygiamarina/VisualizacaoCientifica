# For compiling for windows:
#    make OS="win32"
# For cross-compiling for windows:
#    make OS="cross"

ARCH = $(shell getconf LONG_BIT)
OS = linux
CC = c++
WARCH=w64

.SUFFIXES: .C .cpp .cxx .h

# include $(CGAL_MAKEFILE)

LDFLAGS = -lCGAL 
CXX     = g++

ifeq ($(OS),linux)
   # flags for C++ compiler:
   CFLAGS  = -I/usr/include/ -frounding-math
   # libraries to link with:
   LIBPATH = $(CGAL_LIBPATH) -L/usr/lib$(ARCH)
   EXT = "-linux$(ARCH)"
else ifeq ($(OS),osx)
   # flags for C++ compiler:
   CFLAGS  = -I/opt/local/include/ -frounding-math
   # libraries to link with:
   LIBPATH = $(CGAL_LIBPATH) -L/opt/local/lib -L/usr/X11/lib
   EXT=".osx"
endif

# **********************************************************************************
all:	terrain$(EXT)

terrain$(EXT):	terrain.o
	$(CXX)  -o $@ $^ $(CFLAGS) $(LIBPATH) $(LDFLAGS) -lglut -lGLU -lGL

.c.o:	$*.h
	$(CXX) -c $(CFLAGS) $*.c

.cpp.o:	$*.h
	$(CXX) -c $(CFLAGS) $*.cpp

clean:
	rm *.o terrain$(EXT)
	rm *~

.C.o:	$*.h
	$(CXX) -c $(CFLAGS) $*.C


