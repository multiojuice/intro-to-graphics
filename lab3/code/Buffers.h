//
//  Buffers.h
//
//  Vertex and element buffer management
//
//  Author:  Warren R. Carithers
//  Date:    2017/02/11 22:34:54
//  Updated: 2019/01/25 19:23:16
//
//  This file should not be modified by students.
//

#ifndef BUFFERS_H_
#define BUFFERS_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

using namespace std;

#include "Canvas.h"

//
// How to calculate an offset into the vertex buffer
//
#define BUFFER_OFFSET(i)        ((GLvoid *)(((char *)0) + (i)))

//
// All the relevant information needed to keep
// track of vertex and element buffers
//

class BufferSet {

public:
    // buffer handles
    GLuint vbuffer, ebuffer;

    // total number of vertices
    int numElements;

    // component sizes (bytes)
    long vSize, eSize, tSize, cSize, nSize;

    // have these already been set up?
    bool bufferInit;

public:

    ///
    /// Constructor
    ///
    BufferSet( void );

    ///
    /// initBuffer() - reset the supplied buffer to its "empty" state
    ///
    void initBuffer( void );

    ///
    /// dumpBuffer(which) - dump the contents of the BufferSet
    ///
    /// @param which   description of the buffer
    ///
    void dumpBuffer( const char *which );

    ///
    /// makeBuffer(target,data,size) - make a vertex or element array buffer
    ///
    /// @param target   which type of buffer to create
    /// @param data     source of data for buffer (or NULL)
    /// @param size     desired length of buffer
    ///
    /// @return the ID of the new buffer
    ///
    GLuint makeBuffer( GLenum target, const void *data, GLsizei size );

    ///
    /// createBuffers(canvas) - create a set of buffers for the object
    ///     currently held in 'canvas'.
    ///
    /// @param C     the Canvas we'll use for drawing
    ///
    void createBuffers( Canvas &C );

    ///
    /// selectBuffers() - bind the correct vertex and element buffers
    ///
    /// @param program   GLSL program object
    /// @param vp        name of the position attribute variable
    /// @param vc        name of the color attribute variable (or NULL)
    /// @param vn        name of the normal attribute variable (or NULL)
    /// @param vt        name of the texture coord attribute variable (or NULL)
    ///
    void selectBuffers( GLuint program,
        const char *vp, const char * vc, const char *vn, const char *vt );

};

#endif
