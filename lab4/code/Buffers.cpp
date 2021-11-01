//
//  Buffers.cpp
//
//  Vertex and element buffer implementations.
//
//  Author:  Warren R. Carithers
//  Date:    2017/02/11 22:34:54
//  Updated: 2019/01/25 19:21:21
//
//  This file should not be modified by students.
//

#include <cstdlib>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

//
// GLEW and GLFW header files also pull in the OpenGL definitions
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Buffers.h"
#include "Utils.h"

///
/// Constructor
///
BufferSet::BufferSet( void ) {
    // do this the easy way
    initBuffer();
}

///
/// initBuffer() - reset the BufferSet to its "empty" state
///
void BufferSet::initBuffer( void ) {
    vbuffer = ebuffer = 0;
    numElements = 0;
    vSize = eSize = tSize = cSize = nSize = 0;
    bufferInit = false;
}

///
/// dumpBuffer(which) - dump the contents of the BufferSet
///
/// @param which   description of the buffer
///
void BufferSet::dumpBuffer( const char *which ) {
    cout << "Dumping buffer " << which << " (";
    if( !bufferInit ) {
        cout << "not ";
    }
    cout << "initialized)" << endl;
    cout << "  IDs: v " << vbuffer << " e " << ebuffer <<
        " #elements: " << numElements << endl;
    cout << "  Sizes:  v " << vSize << " e " << eSize <<
        " t " << tSize << " c " << cSize << " n " << nSize << endl;
}

///
/// makeBuffer(target,data,size) - make a vertex or element array buffer
///
/// @param target   which type of buffer to create
/// @param data     source of data for buffer (or NULL)
/// @param size     desired length of buffer
///
/// @return the ID of the new buffer
///
GLuint BufferSet::makeBuffer( GLenum target, const void *data, GLsizei size ) {
    GLuint buffer;

    glGenBuffers( 1, &buffer );
    glBindBuffer( target, buffer );
    glBufferData( target, size, data, GL_STATIC_DRAW );

    return( buffer );
}

///
/// createBuffers(canvas) create a set of buffers for the object
///     currently held in 'canvas'.
///
/// @param C     the Canvas we'll use for drawing
///
void BufferSet::createBuffers( Canvas &C ) {

    // reset this BufferSet if it has already been used
    if( bufferInit ) {
        // must delete the existing buffer IDs first
        glDeleteBuffers( 1, &(vbuffer) );
        glDeleteBuffers( 1, &(ebuffer) );
        // clear everything out
        initBuffer();
    }

    //
    // vertex buffer structure
    //
    // our vertex buffers will always have locations, but the
    // other fields may or may not be present; this depends on
    // how the shape was created
    //
    //             data        components   offset to beginning
    //          [ locations ]  XYZW         0
    //          [ colors    ]  RGBA         vSize
    //          [ normals   ]  XYZ          vSize+cSize
    //          [ t. coords ]  UV           vSize+cSize+nSize
    //

    // get the vertex count
    numElements = C.numVertices();

    // if there are no vertices, there's nothing for us to do
    if( numElements < 1 ) {
        return;
    }

    // OK, we have vertices!
    float *points = C.getVertices();
    // #bytes = number of elements * 4 floats/element * bytes/float
    vSize = numElements * 4 * sizeof(float);

    // accumulate the total vertex buffer size
    GLsizeiptr vbufSize = vSize;

    // get the color data (if there is any)
    float *colors = C.getColors();
    if( colors != NULL ) {
        cSize = numElements * 4 * sizeof(float);
        vbufSize += cSize;
    }

    // get the normal data (if there is any)
    float *normals = C.getNormals();
    if( normals != NULL ) {
        nSize = numElements * 3 * sizeof(float);
        vbufSize += nSize;
    }

    // get the (u,v) data (if there is any)
    float *uv = C.getUV();
    if( uv != NULL ) {
        tSize = numElements * 2 * sizeof(float);
        vbufSize += tSize;
    }

    // get the element data
    GLuint *elements = C.getElements();
    // #bytes = number of elements * bytes/element
    eSize = numElements * sizeof(GLuint);

    // first, create the connectivity data
    ebuffer = makeBuffer( GL_ELEMENT_ARRAY_BUFFER, elements, eSize );

    // next, the vertex buffer, containing vertices and "extra" data
    // note that we use glBufferSubData() calls to do the copying
    vbuffer = makeBuffer( GL_ARRAY_BUFFER, NULL, vbufSize );

    // copy in the location data
    glBufferSubData( GL_ARRAY_BUFFER, 0, vSize, points );

    // offsets to subsequent sections are the sum of
    // the preceding section sizes (in bytes)
    GLintptr offset = vSize;

    // add in the color data (if there is any)
    if( cSize > 0 ) {
        glBufferSubData( GL_ARRAY_BUFFER, offset, cSize, colors );
        offset += cSize;
    }

    // add in the normal data (if there is any)
    if( nSize > 0 ) {
        glBufferSubData( GL_ARRAY_BUFFER, offset, nSize, normals );
        offset += nSize;
    }

    // add in the (u,v) data (if there is any)
    if( tSize > 0 ) {
        glBufferSubData( GL_ARRAY_BUFFER, offset, tSize, uv );
        offset += tSize;
    }

    // sanity check!
    if( offset != vbufSize ) {
        cerr << "*** createBuffers: size mismatch, offset "
            << offset << " vbufSize " << vbufSize << endl;
    }

    // NOTE:  'points', 'colors', and 'elements' are dynamically allocated,
    // but we don't free them here because they will be freed at the next
    // call to clear() or the get*() functions

    // finally, mark it as set up
    bufferInit = true;
}

///
/// selectBuffers() - bind the correct vertex and element buffers
///
/// @param program   GLSL program object
/// @param vp        name of the position attribute variable
/// @param vc        name of the color attribute variable (or NULL)
/// @param vn        name of the normal attribute variable (or NULL)
/// @param vt        name of the texture coord attribute variable (or NULL)
///
void BufferSet::selectBuffers( GLuint program,
    const char *vp, const char *vc, const char *vn, const char *vt ) {

    // bind the buffers
    glBindBuffer( GL_ARRAY_BUFFER, vbuffer );
    glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, ebuffer );

    // set up the vertex attribute variables

    // we always want position data
#if defined(DEBUG)
    if( vSize == 0 ) {
        cerr << "selectBuffers(): Position data requested, but vSize is 0"
             << endl;
    }
#endif
    GLint loc = getAttribLoc( program , vp );
    if( loc >= 0 ) {
        glEnableVertexAttribArray( loc );
        glVertexAttribPointer( loc, 4, GL_FLOAT, GL_FALSE, 0,
                               BUFFER_OFFSET(0) );
    }

    // cumulative byte offset for subsequent data sections
    int offset = vSize;

    // do we also want color?
    if( vc != NULL ) {
#if defined(DEBUG)
        if( cSize == 0 ) {
            cerr << "selectBuffers(): Color data requested, but cSize is 0",
                 << endl;
        }
#endif
        loc = getAttribLoc( program, vc );
        if( loc >= 0 ) {
            glEnableVertexAttribArray( loc );
            glVertexAttribPointer( loc, 4, GL_FLOAT, GL_FALSE, 0,
                                   BUFFER_OFFSET(offset) );
        }
        offset += cSize;
    }

    // how about a surface normal?
    if( vn != NULL ) {
#if defined(DEBUG)
        if( nSize == 0 ) {
            cerr << "selectBuffers(): Normal data requested, but nSize is 0",
                 << endl;
        }
#endif
        loc = getAttribLoc( program, vn );
        if( loc >= 0 ) {
            glEnableVertexAttribArray( loc );
            glVertexAttribPointer( loc, 3, GL_FLOAT, GL_FALSE, 0,
                                   BUFFER_OFFSET(offset) );
        }
        offset += nSize;
    }

    // what about texture coordinates?
    if( vt != NULL ) {
#if defined(DEBUG)
        if( tSize == 0 ) {
            cerr << "selectBuffers(): Texture data requested, but tSize is 0",
                 << endl;
        }
#endif
        loc = getAttribLoc( program, vt );
        if( loc >= 0 ) {
            glEnableVertexAttribArray( loc );
            glVertexAttribPointer( loc, 2, GL_FLOAT, GL_FALSE, 0,
                                   BUFFER_OFFSET(offset) );
        }
        offset += tSize;
    }
}
