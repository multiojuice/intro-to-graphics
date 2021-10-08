//
//  Application.cpp
//
//  Assignment-specific code.
//
//  Created by Warren R. Carithers on 2019/09/09.
//  Based on earlier versions created by Joe Geigel and Warren R. Carithers
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
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

#include "Types.h"
#include "Utils.h"
#include "ShaderSetup.h"
#include "Buffers.h"
#include "Models.h"
#include "Canvas.h"

#include "Application.h"

using namespace std;

//
// PRIVATE GLOBALS
//

// names of our GLSL shader files (we assume GLSL 1.50)
static const char *vshader = "v150.vert";
static const char *fshader = "v150.frag";

// our Canvas
static Canvas *canvas;

// buffers for our shapes
static BufferSet shapes;
static BufferSet regions;
static BufferSet outlines;

// shader program handle
static GLuint program;

// our VAO
static GLuint vao;

// do we need to do a display() call?
static bool updateDisplay = true;

// click counter for color selection
static int counter = 0;

//
// PUBLIC GLOBALS
//

//
// Drawing-related variables
//

// dimensions of the drawing window
int w_width  = 300;
int w_height = 300;

// drawing window title
const char *w_title = "S-H Polygon Clipping";

// GL context we're using (we assume 3.2, for GLSL 1.50)
int gl_maj = 3;
int gl_min = 2;

// our GLFWwindow
GLFWwindow *w_window;

//
// PRIVATE FUNCTIONS
//

///
/// iterate through argv converting numeric parameters
/// and setting test bits accordingly
///
/// @param argc   number of command-line arguments
/// @param argv   command-line argument strings
///
static void set_tests( const int argc, char *argv[] ) {
    char *endptr;

    which = 0;

    for( int i = 1; i < argc; ++i ) {
        long int n = strtol( argv[i], &endptr, 10 );
        if( endptr == argv[i] || *endptr != '\0' ) { // no/bad conversion
            cerr << "Bad test # '" << argv[i] << "' ignored" << endl;
        } else {
            if( n >= 0 && n < N_POLYS ) {
                which |= (1 << n);
            } else {
                cerr << "Bad test # '" << argv[i] << "' ignored" << endl;
            }
        }
    }

    if( which == 0 ) {
        which = TEST_ALL;
    }
}

///
/// Create the shapes we'll display
///
static void createImage( Canvas &C )
{
    Color white = { 1.0f, 1.0f, 1.0f, 1.0f };

    // start with a clean canvas
    C.clear();

    // first, create the clipping region buffers
    //
    // start by putting all the vertices for all
    // the regions into a single set of buffers

    // draw all of them in white
    C.setColor( white );
    makeClipOutlines( C );

    // collect the vertex, element, and color data for these
    regions.createBuffers( C );

    // next, our polygon outlines
    C.clear();
    makePolygonOutlines( C );
    outlines.createBuffers( C );

    // finally, our clippable polygons
    C.clear();
    for( int i = 0; i < N_POLYS; ++i ) {
        DOIF( i, createObject( i, C ); )
    }
    shapes.createBuffers( C );
}

//
// Event callback routines for this assignment
//

///
/// Handle keyboard input
///
/// @param window   GLFW window being used
/// @param key      code for the key that was pressed
/// @param scan     scan code of the key
/// @param action   type of event (press, release, etc.)
/// @param mods     modifiers in use (shift, etc.)
///
static void keyboard( GLFWwindow *window, int key, int scan,
                      int action, int mods )
{
    // only react to "press" events
    if( action != GLFW_PRESS ) {
        return;
    }

    switch( key ) {
    case GLFW_KEY_H:    // help message
        cout << "  Key(s)             Action" << endl;
        cout << "=========   =======================" << endl;
        cout << "ESC, q, Q   Terminate the program" << endl;
        cout << " h, H       Print this message" << endl;
        break;

    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_Q:
        glfwSetWindowShouldClose( window, 1 );
        break;
    }
}

///
/// Display the current image
///
static void display( void )
{
    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // ensure we have selected the shader program
    glUseProgram( program );

    // set up our scale factors for normalization
    GLuint sf = glGetUniformLocation( program, "sf" );
    glUniform2f( sf, 2.0f / (w_width - 1.0f), 2.0f / (w_height - 1.0f) );

    //
    // first, draw the polygon outlines
    //

    outlines.selectBuffers( program, "vPosition", "vColor", NULL, NULL );

    int skip = 0;
    for( int i = 0; i < N_POLYS; ++i ) {
        DOIF( i,
            glDrawArrays( GL_LINE_LOOP, skip, n_verts[i] );
            skip += n_verts[i];
        )
    }

    //
    // next, the clipped polygons
    //

    shapes.selectBuffers( program, "vPosition", "vColor", NULL, NULL );
    
    skip = 0;
    for( int i = 0; i < N_POLYS; ++i ) {
        if( nv[i] ) {
            glDrawArrays( GL_TRIANGLE_FAN, skip, nv[i] );
            skip += nv[i];
        }
    }

    //
    // finally, the clip region outlines (done last so that
    // they show up on top of everything else)
    //

    regions.selectBuffers( program, "vPosition", "vColor", NULL, NULL );

    for( int i = 0; i < n_regions; ++i ) {
        glDrawArrays( GL_LINE_LOOP, i * 4, 4 );
    }
}

///
/// OpenGL initialization
///
/// @return boolean indicating status of initialization
///
static bool init( void )
{
    // Check the OpenGL major version
    if( gl_maj < 3 ) {
        vshader = "v120.vert";
        fshader = "v120.frag";
    }

    // Load shaders and use the resulting shader program
    ShaderError error;
    program = shaderSetup( vshader, fshader, &error );
    if( !program ) {
        cerr << "Error setting up shaders - " << errorString(error) << endl;
        return( false );
    }

    // create our Canvas
    canvas = new Canvas( w_width, w_height );
    if( canvas == NULL ) {
        cerr << "Error - cannot create Canvas" << endl;
        return( false );
    }

    // need a VAO if we're using a core context;
    // doesn't hurt even if we're not using one
    glGenVertexArrays( 1, &vao );
    glBindVertexArray( vao );

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    // glEnable( GL_CULL_FACE );
    // glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );
    // glPolygonMode( GL_FRONT_AND_BACK, GL_POINT );

    // create the geometry for our shapes.
    createImage( *canvas );

    // register our callbacks

    // key callback - for when we only care about the physical key
    glfwSetKeyCallback( w_window, keyboard );

    return( true );
}

//
// PUBLIC FUNCTIONS
//

///
/// application-specific processing
///
/// @param argc   command-line argument count
/// @param argv   command-line argument strings
///
void application( int argc, char *argv[] )
{
    // process command-line arguments
    if( argc > 1 ) {
        set_tests( argc, argv );
    } else {
        which = TEST_ALL;
    }

    // set up the objects and the scene
    if( !init() ) {
        return;
    }

    // loop until it's time to quit
    while( !glfwWindowShouldClose(w_window) ) {
        display();
        glfwSwapBuffers( w_window );
        glfwWaitEvents();
    }
}
