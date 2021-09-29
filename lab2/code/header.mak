#
# This header.mak file sets up all the necessary options for compiling
# and linking C and C++ programs which use OpenGL, GLFW, and/or GLEW on
# the CS Ubuntu systems.  To use it, place it in the same directory as
# your source code, and run the command
#
#	gmakemake > Makefile
#
# To compile and link your program, just run "make".
#

# locations of important directories if the header files and library
# files aren't in the standard places - e.g., MacPorts typically
# puts things into /opt/local/include and /opt/local/lib
INCLUDE =
LIBDIRS =

# uncomment this if you get "-lGL not found"
# errors when linking on the CS machines
# LIBDIRS += -L/home/course/cscix10/lib/links

# common linker options
# add "-lSOIL" if using that image library
LDLIBS = -lGL -lGLEW -lglfw -lm

# language-specific linker options
# add "-lgsl -lgslcblas" if using GSL
CLDLIBS =
CCLDLIBS =

# frameworks for macos - if you are compiling on a Mac, uncomment the
# two "FMWKS +=" lines here
FMWKS =
# FMWKS += -framework OpenGL -framework Cocoa
# FMWKS += -framework IOKit -framework CoreVideo

# common compiler flags
CCFLAGS = -ggdb $(INCLUDE) -DGL_GLEXT_PROTOTYPES

# language-specific compiler flags
CFLAGS = -std=c99 $(CCFLAGS)
CXXFLAGS = $(CCFLAGS) -DGL_SILENCE_DEPRECATION

# common linker flags
LIBFLAGS = -ggdb $(LIBDIRS) $(LDLIBS)

# language-specific linker flags
CLIBFLAGS = $(LIBFLAGS) $(CLDLIBS)
CCLIBFLAGS = $(FMWKS) $(LIBFLAGS) $(CCLDLIBS)
