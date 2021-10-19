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
#include "Pipeline.h"

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
static BufferSet polyBuffers;

// shader program handle
static GLuint program;

// our VAO
static GLuint vao;

// do we need to do a display() call?
static bool updateDisplay = true;

// our Pipeline
static Pipeline *pipeline;

//
// PUBLIC GLOBALS
//

//
// Drawing-related variables
//

// dimensions of the drawing window
int w_width  = 501;
int w_height = 501;

// drawing window title
const char *w_title = "2D Pipeline";

// GL context we're using (we assume 3.2, for GLSL 1.50)
int gl_maj = 3;
int gl_min = 2;

// our GLFWwindow
GLFWwindow *w_window;

//
// PRIVATE FUNCTIONS
//

///
/// process the command-line arguments (if present)
///
/// @param argc   number of command-line arguments
/// @param argv   command-line argument strings
///
static void set_tests( const int argc, char *argv[] ) {
    char *endptr;

    which = 0;

    if( argc > 1 ) {
        long int n = strtol( argv[1], &endptr, 10 );
        if( endptr == argv[1] || *endptr != '\0' ) { // no/bad conversion
            cerr << "Bad image # '" << argv[1] << "' ignored" << endl;
        } else {
            if( n >= 0 && n < N_IMAGES ) {
                which = n;
            } else {
                cerr << "Bad image # '" << argv[1] << "' ignored" << endl;
            }
        }
    }
}

///
/// Create the shapes we'll display
///
static void createImage( Pipeline &P )
{
    // add all the polygons to the pipeline
    createObject( P );
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
        cout << "=========   ===========================" << endl;
        cout << "ESC, q, Q   Terminate the program" << endl;
        cout << " 0, p, P    Display the original image" << endl;
        cout << " 1, c, C    Use a different clip window" << endl;
        cout << " 2, t, T    Simple model transformations" << endl;
        cout << " 3, x, X    Multiple transformations" << endl;
        cout << " 4, v, V    Use alternate viewports" << endl;
        cout << "  h, H      Print this message" << endl;
        break;

    // basic polygons
    case GLFW_KEY_P:  // FALL THROUGH
    case GLFW_KEY_0:
        which = 0;
        updateDisplay = true;
        break;

    // altered clipping window
    case GLFW_KEY_C:  // FALL THROUGH
    case GLFW_KEY_1:
        which = 1;
        updateDisplay = true;
        break;
                            
    // object transformations 1
    case GLFW_KEY_T:  // FALL THROUGH
    case GLFW_KEY_2:
        which = 2;
        updateDisplay = true;
        break;

    // object transformations 2
    case GLFW_KEY_X:  // FALL THROUGH
    case GLFW_KEY_3:
        which = 3;
        updateDisplay = true;
        break;

    // altered viewports
    case GLFW_KEY_V:  // FALL THROUGH
    case GLFW_KEY_4:
        which = 4;
        updateDisplay = true;
        break;

    case GLFW_KEY_ESCAPE:
    case GLFW_KEY_Q:
        glfwSetWindowShouldClose( window, 1 );
        break;
    }
}

///
/// Handle mouse clicks
///
/// @param window   GLFW window being used
/// @param button   code for the mouse button that was pressed
/// @param action   type of event (press, release, etc.)
/// @param mods     modifiers in use (shift, etc.)
///
static void mouse( GLFWwindow *window, int button, int action, int mods )
{
    if( button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS ) {
        ++which;
        updateDisplay = true;
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

    // draw the objects and create the buffers
    drawObjects( *pipeline );
    polyBuffers.createBuffers( *pipeline );

    // bind our buffers
    polyBuffers.selectBuffers( program, "vPosition", "vColor", NULL, NULL );

    if( pipeline->isOutline() ) {

        // draw the objects as separate line loops

        int skip = 0;
        for( int i = 0; i < pipeline->countNum; ++i ) {
            glDrawArrays( GL_LINE_LOOP, skip, pipeline->outlineCounts[i] );
            skip += pipeline->outlineCounts[i];
        }

    } else {

        // draw all the individual points
        glDrawElements( GL_POINTS, polyBuffers.numElements,
                        GL_UNSIGNED_INT, 0 );
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

    // create our Pipeline
    pipeline = new Pipeline( w_width, w_height );
    if( pipeline == NULL ) {
        cerr << "Error - cannot create Pipeline" << endl;
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
    createImage( *pipeline );

    // register our callbacks

    // key callback - for when we only care about the physical key
    glfwSetKeyCallback( w_window, keyboard );

    // mouse click handler
    glfwSetMouseButtonCallback( w_window, mouse );

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
        which = 0;
    }

    // set up the objects and the scene
    if( !init() ) {
        return;
    }

    // loop until it's time to quit
    while( !glfwWindowShouldClose(w_window) ) {
        if( updateDisplay ) {
            updateDisplay = false;
            display();
            glfwSwapBuffers( w_window );
        }
        glfwWaitEvents();
    }
}
