//
//  ShaderSetup.h
//
//  Set up a GLSL shader based on supplied source files.
//

#ifndef SHADERSETUP_H_
#define SHADERSETUP_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

//
// GLEW and GLFW header files also pull in the OpenGL definitions
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>

//
// Error codes returned by ShaderSetup()
//

typedef enum sError {
    // generic
    E_NO_ERROR, E_NO_STRING, E_NO_OBJECTS,
    E_US_ALLOC, E_US_LOAD, E_US_COMPILE,
    // vertex shader-specific
    E_VS_ALLOC, E_VS_LOAD, E_VS_COMPILE, E_NO_VSHADER,
    // fragment shader-specific
    E_FS_ALLOC, E_FS_LOAD, E_FS_COMPILE,
    // geometry shader-specific
    E_GS_ALLOC, E_GS_LOAD, E_GS_COMPILE,
    // program-specific
    E_PROG_ALLOC, E_PROG_LINK
} ShaderError;

///
/// readTextFile(name)
///
/// Read the text file given as 'name'.
///
/// Returns the contents of the file in a dynamically-allocated
/// string buffer, or NULL if an error occurs.
///
GLchar *readTextFile( const char *name );

///
/// printShaderInfoLog(shader)
///
/// Print the information log from a shader compilation attempt
///
void printShaderInfoLog( GLuint shader );

///
/// printProgramInfoLog(shader)
///
/// Print a program information log
///
/// This is identical to printShaderInfoLog(), except that it uses
/// glGetProgramiv() and glGetProgramInfoLog() instead of the *Shader*()
/// versions.
///
void printProgramInfoLog( GLuint shader );

///
/// errorString(code)
///
/// Returns a const char* with a text version of the supplied error code.
///
const char *errorString( ShaderError code );

///
/// shaderCreate( const GLchar **src, GLenum type, ShaderError *err )
///
/// Create a GLSL shader object from a set of one or more source strings.
///
/// If more than one source string is provided, care should be taken
/// that only the first string contains a #version directive.
///
/// @param  src  array of GLchar * containing shader source code
/// @param  type type of shader object to create
/// @param  err  pointer to status variable
/// @return      id of the new shader object, or 0 on error, with
///              an error code in err
///
GLuint shaderCreate( const GLchar **src, GLenum type, ShaderError *err );

///
/// shaderLink( GLuint ids[], size_t num, ShaderError *err )
///
/// Link a collection of shaders into a shader program.
///
/// @param ids   array of shader object ids
/// @param num   number of elements in the array
/// @param err   pointer to status variable
/// @return      id of the new shader program, or 0 on error, with
///              an error code in err
///
GLuint shaderLink( GLuint ids[], size_t num, ShaderError *err );

///
/// shaderSetupStr(vertex,fragment,geometryerr)
///
/// Set up a GLSL shader program.
///
/// Requires strings containing the source code for GLSL shaders.
/// Returns a program ID for the linked program.
///
/// @param  vsrc   vertex shader program source code
/// @param  fsrc   fragment shader program source code, or NULL
/// @param  gsrc   geometry shader program source code, or NULL
/// @param  err    pointer to status variable
///
/// On success:
///      Returns the GLSL shader program handle, and sets the 'err'
///      parameter to E_NO_ERROR.
///
/// On failure:
///      Returns 0, and assigns an error code to 'err'.
///
GLuint shaderSetupStr( const GLchar *vsrc, const GLchar *fsrc,
                       const GLchar *gsrc, ShaderError *err );

///
/// shaderSetup(vertex,fragment,err)
///
/// Set up a GLSL shader program.
///
/// Requires the names of vertex and fragment program source
/// files.  Returns a handle to the created GLSL program.
///
/// Arguments:
/// @param vert   vertex shader program source file
/// @param frag   fragment shader program source file, or NULL
/// @param err    pointer to status variable
///
/// On success:
///      Returns the GLSL shader program handle, and sets the 'err'
///      parameter to E_NO_ERROR.
///
/// On failure:
///      Returns 0, and assigns an error code to 'err'.
///
GLuint shaderSetup( const char *vert, const char *frag, ShaderError *err );

///
/// shaderSetup(vertex,fragment,geometry,err)
///
/// Set up a GLSL shader program.
///
/// Requires the names of vertex and fragment program source
/// files.  Returns a handle to the created GLSL program.
///
/// Arguments:
/// @param vert   vertex shader program source file
/// @param frag   fragment shader program source file, or NULL
/// @param geom   geometry shader program source file, or NULL
/// @param err    pointer to status variable
///
/// On success:
///      Returns the GLSL shader program handle, and sets the 'err'
///      parameter to E_NO_ERROR.
///
/// On failure:
///      Returns 0, and assigns an error code to 'err'.
///
GLuint shaderSetup( const char *vert, const char *frag,
                     const char *geom, ShaderError *err );

#endif
