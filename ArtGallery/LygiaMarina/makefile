CC = gcc
DEBUG = -g
CPP_FILES = $(wildcard src/*.cpp)
OBJ_FILES = $(addprefix obj/,$(notdir $(CPP_FILES:.cpp=.o)))
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
GLFLAGS = -lGL -lGLU -lglut

ArtGallery : $(OBJ_FILES)
	$(CC) $(LFLAGS) $< $(GLFLAGS) -o $@

obj/%.o : src/%.cpp src/%.h
	$(CC) $(CFLAGS) $< $(GLFLAGS) -o $@

clean:
	rm *~ ArtGallery

tar:
	tar cfv ArtGallery.tar main.cpp PolygonVertex.h PolygonVertex.cpp

