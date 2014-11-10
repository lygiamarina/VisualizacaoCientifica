.SUFFIXES: .C .cpp .cxx .h

include $(CGAL_MAKEFILE)

# flags for C++ compiler:

CFLAGS 		= 	-I/opt/local/include/ -frounding-math

# libraries to link with:

LIBPATH 	= 	$(CGAL_LIBPATH) -L/opt/local/lib -L/usr/X11/lib

LDFLAGS 	= 	-lCGAL

# ***********************************************************************************
all:	chull

chull:	chull.o
	$(CXX)  -o $@ $^ $(CFLAGS) $(LIBPATH) $(LDFLAGS) -lglut -lGLU -lGL -lXmu -lXi

.c.o:	$*.h
	$(CXX) -c $(CFLAGS) $*.c

.cpp.o:	$*.h
	$(CXX) -c $(CFLAGS) $*.cpp

clean:
	rm *.o terrain

.C.o:	$*.h
	$(CXX) -c $(CFLAGS) $*.C


