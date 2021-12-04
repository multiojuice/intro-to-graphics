#
# Created by gmakemake (Ubuntu Jul 25 2014) on Sat Dec  4 12:32:42 2021
#

#
# Definitions
#

.SUFFIXES:
.SUFFIXES:	.a .o .c .C .cpp .s .S
.c.o:
		$(COMPILE.c) $<
.C.o:
		$(COMPILE.cc) $<
.cpp.o:
		$(COMPILE.cc) $<
.S.s:
		$(CPP) -o $*.s $<
.s.o:
		$(COMPILE.cc) $<
.c.a:
		$(COMPILE.c) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.C.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%
.cpp.a:
		$(COMPILE.cc) -o $% $<
		$(AR) $(ARFLAGS) $@ $%
		$(RM) $%

CC =		gcc
CXX =		g++

RM = rm -f
AR = ar
LINK.c = $(CC) $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)
LINK.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS)
COMPILE.c = $(CC) $(CFLAGS) $(CPPFLAGS) -c
COMPILE.cc = $(CXX) $(CXXFLAGS) $(CPPFLAGS) -c
CPP = $(CPP) $(CPPFLAGS)
########## Flags from header.mak

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

# On the CS lab machines under Ubuntu 20.04, the standard version
# of SOIL has a bug which manifests itself when using the GPU that is
# integrated into the CPU package on Intel systems.  A version of
# SOIL that corrects this bug is in the course account; these macros
# tell the compiler and linker where to find it, and to use it instead
# of the default version.
#
# CHANGE THESE LINES if you are using 'gmakemake' on a macOS system!
# Contains compiled version of soil
# MAKE REAL CLEAN
INCLUDE = -I/home/course/cscix10/lib/SOIL/include
LIBDIRS = -L/home/course/cscix10/lib/SOIL

# uncomment this if you get "-lGL not found"
# errors when linking on the CS machines
# LIBDIRS += -L/home/course/cscix10/lib/links

# common linker options
# add "-lSOIL" if using that image library
LDLIBS = -lSOIL -lGL -lGLEW -lglfw -lm

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

########## End of flags from header.mak


CPP_FILES =	Application.cpp Buffers.cpp Canvas.cpp Lighting.cpp Materials.cpp Models.cpp ShaderSetup.cpp Utils.cpp Viewing.cpp main.cpp
C_FILES =	
PS_FILES =	
S_FILES =	
H_FILES =	Application.h Buffers.h Canvas.h CylinderData.h Lighting.h Materials.h Models.h QuadData.h ShaderSetup.h Types.h Utils.h Viewing.h
SOURCEFILES =	$(H_FILES) $(CPP_FILES) $(C_FILES) $(S_FILES)
.PRECIOUS:	$(SOURCEFILES)
OBJFILES =	Application.o Buffers.o Canvas.o Lighting.o Materials.o Models.o ShaderSetup.o Utils.o Viewing.o 

#
# Main targets
#

all:	main 

main:	main.o $(OBJFILES)
	$(CXX) $(CXXFLAGS) -o main main.o $(OBJFILES) $(CCLIBFLAGS)

#
# Dependencies
#

Application.o:	Application.h Buffers.h Canvas.h Lighting.h Materials.h Models.h ShaderSetup.h Types.h Utils.h Viewing.h
Buffers.o:	Buffers.h Canvas.h Types.h Utils.h
Canvas.o:	Canvas.h Types.h Utils.h
Lighting.o:	Buffers.h Canvas.h Lighting.h Models.h Types.h Utils.h
Materials.o:	Buffers.h Canvas.h Lighting.h Materials.h Models.h Types.h Utils.h
Models.o:	Buffers.h Canvas.h CylinderData.h Models.h QuadData.h Types.h
ShaderSetup.o:	ShaderSetup.h Utils.h
Utils.o:	Utils.h
Viewing.o:	Utils.h Viewing.h
main.o:	Application.h Utils.h

#
# Housekeeping
#

Archive:	archive.tgz

archive.tgz:	$(SOURCEFILES) Makefile
	tar cf - $(SOURCEFILES) Makefile | gzip > archive.tgz

clean:
	-/bin/rm -f $(OBJFILES) main.o core

realclean:        clean
	-/bin/rm -f main 
