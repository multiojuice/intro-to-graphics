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

#include "Application.h"

#include "Buffers.h"
#include "Canvas.h"
#include "Models.h"
#include "ShaderSetup.h"
#include "Transforms.h"
#include "Types.h"
#include "Utils.h"

using namespace std;

//
// PRIVATE GLOBALS
//

// number of different images per projection type
#define N_IMAGES    4

//
// Transformation information
//

// flags controlling drawing options
static CamMode   camera = Camera1;
static XformMode transforms = Off;
static ViewMode  viewMode = Frustum;

// names of our GLSL shader files
static const char *vshader = "shader.vert";
static const char *fshader = "shader.frag";

// our Canvas
static Canvas *canvas;

// buffers for our shapes
static BufferSet shapes;

// shader program handle
static GLuint program;

// our VAO
static GLuint vao;

// do we need to do a display() call?
static bool updateDisplay = true;

// image to be displayed
static int which = 0;

//
// PUBLIC GLOBALS
//

//
// Drawing-related variables
//

// dimensions of the drawing window
int w_width  = 512;
int w_height = 512;

// drawing window title
const char *w_title = "Transformations";

// GL context we're using (we assume 3.2, for GLSL 1.50)
int gl_maj = 3;
int gl_min = 2;

// our GLFWwindow
GLFWwindow *w_window;

//
// PRIVATE FUNCTIONS
//

///
/// process the command-line arguments
///
/// @param argc   number of command-line arguments
/// @param argv   command-line argument strings
///
static void args( const int argc, char *argv[] ) {
    char *endptr;

    // default image number
    which = 0;
    
    // too many arguments: ignore them all
    if( argc > 2 ) {
        cerr << "Usage: " << argv[0] << " [ image_number ]" << endl;
        return;
    }

    // if there was an argument, try to convert it
    if( argc == 2 ) {
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
/// @param C  - the Canvas to use when drawing the objects
///
static void createImage( Canvas &C )
{
    // start with a clean canvas
    C.clear();

    // make the teapot
    createObject( C );

    // create the necessary buffers
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
    case GLFW_KEY_F:    // switch to a frustum view
        viewMode = Frustum;
        break;

    case GLFW_KEY_O:    // switch to an orthographic view
        viewMode = Ortho;
        break;

    case GLFW_KEY_0:    // select the first image
        which = 0; camera = Camera1; transforms = Off;
        break;

    case GLFW_KEY_1:    // select the second image
        which = 1; camera = Camera1; transforms = On;
        break;

    case GLFW_KEY_2:    // select the third image
        which = 2; camera = Camera2; transforms = Off;
        break;

    case GLFW_KEY_3:    // select the fourth image
        which = 3; camera = Camera2; transforms = On;
        break;

    case GLFW_KEY_H:    // help message
        cout << "  Key(s)             Action" << endl;
        cout << "=========   =======================" << endl;
        cout << "ESC, q, Q   Terminate the program" << endl;
        cout << "  h, H      Print this message" << endl;
        cout << "  f, F      Switch to a Frustum projection" << endl;
        cout << "  o, O      Switch to an Orthographic projection" << endl;
        cout << "  p, P      Print current configuration" << endl;
        cout << "  r, R      Reset to initial configuration" << endl;
        cout << "    0       Camera position 1, no transforms" << endl;
        cout << "    1       Camera position 1, transforms" << endl;
        cout << "    2       Camera position 2, no transforms" << endl;
        cout << "    3       Camera position 2, transforms" << endl;
        break;

    case GLFW_KEY_P:    // report current settings
        cout << "Projection: "
             << (viewMode == Frustum ? "Frustum" : "Ortho") << endl;
        cout << "Camera: Camera" << (camera == Camera1 ? "1" : "2") << endl;
        cout << "Xforms: " << (transforms == Off ? "Off" : "On") << endl;
        return;

    case GLFW_KEY_R:    // reset to default configuration
        viewMode = Frustum;
        camera = Camera1;
        transforms = Off;
        which = 0;
        break;

    case GLFW_KEY_ESCAPE:   // time to exit
    case GLFW_KEY_Q:
        glfwSetWindowShouldClose( window, 1 );
        break;
    }

    updateDisplay = true;
}

///
/// Handle mouse clicks to cycle through the various images
///
/// @param window   GLFW window being used
/// @param button   code for the mouse button that was pressed
/// @param action   type of event (press, release, etc.)
/// @param mods     modifiers in use (shift, etc.)
///
static void mouse( GLFWwindow *window, int button, int action, int mods )
{
    // only activate on a left-button press - ignore everything else
    if( action != GLFW_PRESS || button != GLFW_MOUSE_BUTTON_LEFT )
        return;

    // count this click
    ++which;

    // select the image to display
    switch( which % 4 ) {
    case 0:
        // default camera, no transforms
        camera = Camera1;
        transforms = Off;
        break;

    case 1:
        // default camera, turn on transformations
        camera = Camera1;
        transforms = On;
        break;

    case 2:
        // modified camera, no transforms
        camera = Camera2;
        transforms = Off;
        break;

    case 3:
        // modified camera, transformations
        camera = Camera2;
        transforms = On;
        break;
    }

    updateDisplay = true;
}

///
/// Set the titlebar in the display window
///
void setTitle( void ) {
    char buf[128];

    sprintf( buf, "%s projection, Camera %d, Transforms %s",
             viewMode == Frustum ? "Frustum" : "Orthographic",
             camera == Camera1 ? 1 : 2,
             transforms == Off ? "Off" : "On" );
    glfwSetWindowTitle( w_window, buf );
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
    checkErrors( "display 1" );

    // bind the object buffers
    shapes.selectBuffers( program, "vPosition", NULL, NULL, NULL );
    checkErrors( "display 2" );

    // set up viewing and projection parameters
    setProjection( program, viewMode );
    checkErrors( "display 3" );

    // set up the camera
    setCamera( program, camera );
    checkErrors( "display 4" );

    // set up the transformations
    //
    // transformations are applied in this order (if you are having
    // trouble recreating the solution image, check your order of
    // matrix multiplication):
    //
    //    scale Y by 2
    //    rotate around Z by 305 degrees
    //    rotate around Y by 330 degrees
    //    translate in X by -0.2
    //    translate in Y by 0.2
    //
    setTransforms( program, transforms );
    checkErrors( "display 5" );

    // set our titlebar
    setTitle();

    // draw the shapes
    glDrawElements( GL_TRIANGLES, shapes.numElements,
                    GL_UNSIGNED_INT, (void *)0 );
    checkErrors( "display 6" );
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
        cerr << "Warning - GLSL 1.50 shaders may not compile" << endl;
    }

    // Load shaders and use the resulting shader program
    ShaderError error;
    program = shaderSetup( vshader, fshader, &error );
    if( !program ) {
        cerr << "Error setting up shaders - " << errorString(error) << endl;
        return( false );
    }

    // check for (and report on) errors
    checkErrors( "init 1" );

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
    checkErrors( "init 2" );

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    checkErrors( "init 3" );

    // create the geometry for our shapes.
    createImage( *canvas );
    checkErrors( "init 4" );

    // register our callbacks
    glfwSetKeyCallback( w_window, keyboard );
    glfwSetMouseButtonCallback( w_window, mouse );
    checkErrors( "init 5" );

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
        args( argc, argv );
    } else {
        which = 0;
    }

    // set up the first image
    switch( which ) {
    case 0:  camera = Camera1; transforms = Off; break;
    case 1:  camera = Camera1; transforms = On;  break;
    case 2:  camera = Camera2; transforms = Off; break;
    case 3:  camera = Camera2; transforms = On;  break;
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
