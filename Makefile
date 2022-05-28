################################################################################
## OS
################################################################################
# OS = LINUX
OS = OSX

################################################################################
## GCC
################################################################################
CC = g++ -std=c++14
OPTS = -O3
FLAGS = -Wall -Werror
DEFS = -DOSX

################################################################################
## GL and QT
################################################################################
# Open gl
GL_LIBS = -lglfw -lglew -lsoil2-debug -framework Cocoa -framework OpenGL -framework IOKit -framework CoreVideo

################################################################################
## Rules
################################################################################
INCL = $(GL_INCL)
LIBS = $(GL_LIBS)

OBJS = \
       Camera.o \
	CompileShaders.o \
       Material.o \
       Mesh.o \
       InputParser.o \
       ParticleSystem.o \
       Plane.o \
       Rasterizer.o \
       RayTracer.o \
       Renderer.o \
       Sphere.o \
       main.o

EXECUTABLE = spiderling

default: $(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	$(CC) $(OPTS) $(FLAGS) $(DEFS) $(OBJS) $(LIBS) -o $(EXECUTABLE)

clean:
	rm -f $(EXECUTABLE) Dependencies $(OBJS)

.cpp.o:
	$(CC) $(OPTS) $(DEFS) -MMD $(INCL) -c $< -o $@
	cat $*.d >> Dependencies
	rm -f $*.d

-include Dependencies
