# Define the compiler and the linker. The linker must be defined since
# the implicit rule for linking uses CC as the linker. g++ can be
# changed to clang++.
CXX = g++
CC  = $(CXX)

# Generate dependencies in *.d files
DEPFLAGS = -MT $@ -MMD -MP -MF $*.d

# Define preprocessor, compiler, and linker flags. Uncomment the # lines
# if you use clang++ and wish to use libc++ instead of GNU's libstdc++.
# -g is for debugging.
CPPFLAGS = -std=c++17 -I. -I../external
CXXFLAGS =  -O2 -Wall -Wextra -pedantic-errors -Wold-style-cast 
CXXFLAGS += -std=c++17 
CXXFLAGS += -g
CXXFLAGS += $(DEPFLAGS)
LDFLAGS = -g -lstdc++fs
SDL2_CFLAGS := $(shell sdl2-config --cflags)
SDL2_LIBS   := $(shell sdl2-config --libs)
#CPPFLAGS += -stdlib=libc++
#CXXFLAGS += -stdlib=libc++
#LDFLAGS +=  -stdlib=libc++

# Targets
PROGS = chip8 sdl_test

all: $(PROGS)

# Targets rely on implicit rules for compiling and linking

chip8: testChip8.o chip8.o
sdl_test: CPPFLAGS += $(SDL2_CFLAGS)
sdl_test: LDLIBS   += $(SDL2_LIBS)
sdl_test: main.o chip8.o
	$(CXX) $(LDFLAGS) $^ -o $@ $(LDLIBS)


# Phony targets
.PHONY: all test clean distclean

# Standard clean
clean:
	rm -f *.o $(PROGS)

distclean: clean
	rm *.d


# Include the *.d files
SRC = $(wildcard *.cc)
-include $(SRC:.cc=.d)
