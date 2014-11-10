.SUFFIXES: .C .cpp .cxx .h

# include $(CGAL_MAKEFILE)

# flags for C++ compiler:

CFLAGS 		=       -I/opt/local/include/ -frounding-math

# libraries to link with:

LIBPATH 	=      -L/opt/local/lib -L/usr/X11/lib

LDFLAGS 	= 	-lCGAL

CXX             =       g++

# ***********************************************************************************
all:	VoronoiDemo

VoronoiDemo:	VoronoiDemo.o
	$(CXX)  -o $@ $^ $(CFLAGS) $(LIBPATH) $(LDFLAGS) -lGLU -lGL -lglut

.c.o:	$*.h
	$(CXX) -c $(CFLAGS) $*.c

.cpp.o:	$*.h
	$(CXX) -c $(CFLAGS) $*.cpp

clean:
	rm *.o VoronoiDemo

.C.o:	$*.h
	$(CXX) -c $(CFLAGS) $*.C


