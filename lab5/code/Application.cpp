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
#include <cstring>
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
#include "Types.h"
#include "Utils.h"

using namespace std;

//
// PRIVATE GLOBALS
//

// names of our GLSL shader files (GLSL 1.50)
static const char *vshader = "v150.vert";
static const char *fshader = "v150.frag";

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

// the shape currently being drawn
static Shape currentShape = Cube;

// subdivisions for tessellation
static int factor1 = 1;
static int factor2 = 1;

// maximum usable tessellation factor
static const int MAX_FACTOR = 20;

// are we animating?
static bool animating = false;

// rotation control
static const float anglesReset[3] = { 30.0, 30.0, 0.0 };
static float angles[3] = { 30.0, 30.0, 0.0 };
static float angleInc = 5.0;

// shader rotation variable ID
static GLint theta;

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
const char *w_title = "Tessellation";

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

    // default shape
    currentShape = Cube;
    
    // too many arguments: ignore them all
    if( argc > 2 ) {
        cerr << "Usage: " << argv[0] << " [ shape_number ]" << endl;
        return;
    }

    // if there was an argument, try to convert it
    if( argc == 2 ) {
        long int n = strtol( argv[1], &endptr, 10 );
        if( endptr == argv[1] || *endptr != '\0' ) { // no/bad conversion
            cerr << "Bad shape # '" << argv[1] << "' ignored" << endl;
        } else {
            if( n >= 0 && n < N_SHAPES ) {
                currentShape = (Shape) n;
            } else {
                cerr << "Bad shape # '" << argv[1] << "' ignored" << endl;
            }
        }
    }
}

///
/// Create a new shape to display
///
/// @param C  - the Canvas to use when drawing the objects
///
static void createImage( Canvas &C )
{
    // start with a clean canvas
    C.clear();

    // make the object
    createObject( C, currentShape, 0.5f, factor1, factor2 );

    // create the necessary buffers
    shapes.createBuffers( C );
}

///
/// Animation routine
///
/// Note: Can cause gimbal lock which also happened on Apollo 11
/// http://en.wikipedia.org/wiki/Gimbal_lock#Gimbal_lock_on_Apollo_11
/// Solution? Use Quaternions (Taught in Comp. Animation: Algorithms)
///
/// TIDBIT:
/// Quaternion plaque on Brougham (Broom) Bridge, Dublin, which says:
///
///    "Here as he walked by
///    on the 16th of October 1843
///
///    Sir William Rowan Hamilton
///
///    in a flash of genius discovered
///    the fundamental formula for
///    quaternion multiplication
///    i^2 = j^2 = k^2 = ijk = -1
///    & cut it on a stone of this bridge"
///
static void animate( void ) {
    int i;
    static int level = 0;

    // only want to animate 450 iterations, then stop
    if( level >= 450 ) {
        level = 0;
        animating = false;
    }

    if( !animating ) {
        return;
    }

    if( level < 150 ) {
        // first 150 rotate in X
        angles[0] -= angleInc / 3;
    } else if( level < 300 ) {
        // second 150 rotate in Y
        angles[1] -= angleInc / 3;
    } else {
        // final 150 rotate in Z
        angles[2] -= angleInc / 3;
    }

    ++level;
    updateDisplay = true;
}

//
// Event callback routines for this assignment
//

///
/// Handle keyboard input
///
/// Because we are using both uppercase and lowercase characters,
/// we need the actual characters, not just key names.
///
/// @param window   GLFW window being used
/// @param code     Unicode point for the character
///
static void keyboard( GLFWwindow *window, unsigned int code )
{
    bool new_shape = false;

    // look only at the low-order ASCII character value
    switch( code & 0x7f ) {

    // termination
    // case 033:  // DOES NOT WORK with 'char' callback function???
    case 'q':  // FALL THROUGH
    case 'Q':
        glfwSetWindowShouldClose( w_window, 1 );
        break;

    // automated animation
    case 'a':   // FALL THROUGH
    case 'A':
        animating = !animating;
        break;

    // incremental rotation along the axes
    case 'x':   angles[0] -= angleInc; break;
    case 'X':   angles[0] += angleInc; break;

    case 'y':   angles[1] -= angleInc; break;
    case 'Y':   angles[1] += angleInc; break;

    case 'z':   angles[2] -= angleInc; break;
    case 'Z':   angles[2] += angleInc; break;

    // shape selection
    case '0':   currentShape = Cube;     new_shape = true; break;
    case '1':   currentShape = Cone;     new_shape = true; break; 

    // tessellation factors
    // factor 1 is used by all shapes
    case 'p':   factor1++; new_shape = true; break;
    case 'P':   factor1--; new_shape = true; break;

    // factor 2 is used by all except the cube
    case 's':   factor2++; if( currentShape != Cube ) new_shape = true; break;
    case 'S':   factor2--; if( currentShape != Cube ) new_shape = true; break;

    // reset rotation angles
    case 'r':   memcpy( angles, anglesReset, sizeof(anglesReset) ); break;

    // reset tessellation factors
    case 'R':   factor1 = factor2 = 1; new_shape = true; break;

    // output rotation angles
    case 'o':
        cerr << "Rotation angles:  "
             << angles[0] << ", " << angles[1] << ", " << angles[2] << endl;
        // return without updating the display
        return;

    // output tessellation factors
    case 'O':
        cerr << "Tessellation factors:  "
             << factor1 << ", " << factor2 << endl;
        // return without updating the display
        return;

    // help message
    case 'h':   // FALL THROUGH
    case 'H':
        cout << "  Key(s)             Action" << endl;
        cout << "=========   =======================" << endl;
        cout << "  q, Q      Terminate the program" << endl;
        cout << "  h, H      Print this message" << endl;
        cout << "  0, 1      Select object to display" << endl;
        cout << "  p, P      Inc/dec primary tessellation factor" << endl;
        cout << "  s, S      Inc/dec primary tessellation factor" << endl;
        cout << "  a, A      Toggle animation" << endl;
        cout << "  o, O      Output current angles/factors" << endl;
        cout << "  r, R      Reset current angles/factors" << endl;
        cout << " x, y, z    Increment rotation around this axis" << endl;
        cout << " X, Y, Z    Decrement rotation around this axis" << endl;
        // return without updating the display
        return;

    default:
        cerr << "Unknown key input '" << ((char) code) << "' ignored" << endl;
        // return without updating the display
        return;
    }

    // sanity check the tessellation factors
    if( factor1 < 1 ) {
        factor1 = 1;
    } else if( factor1 > MAX_FACTOR ) {
        factor1 = MAX_FACTOR;
    }

    if( factor2 < 1 ) {
        factor2 = 1;
    } else if( factor2 > MAX_FACTOR ) {
        factor2 = MAX_FACTOR;
    }

    // do we need to recreate the shape?
    if( new_shape ) {
        createImage( *canvas );
    }

    updateDisplay = true;
}

///
/// Set the titlebar in the display window
///
void setTitle( void ) {
    char buf[128];
    const char *which;

    sprintf( buf, "%s tessellation: %d, %d",
             objects[currentShape], factor1, factor2 );
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

    // bind the object buffers
    shapes.selectBuffers( program, "vPosition", NULL, NULL, NULL );

    // set the rotation parameters
    glUniform3fv( theta, 1, angles );

    // set our titlebar
    setTitle();

    // draw the shapes
    glDrawElements( GL_TRIANGLES, shapes.numElements,
                    GL_UNSIGNED_INT, (void *)0 );
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

    // select the shader program, and record the
    // index of the rotation variable
    glUseProgram( program );
    theta = getUniformLoc( program, "theta" );

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
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0, 0.0, 0.0, 1.0 );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

    // create the geometry for our shapes.
    createImage( *canvas );

    // register our callbacks
    glfwSetCharCallback( w_window, keyboard );

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
        currentShape = Cube;
    }

    // set up the objects and the scene
    if( !init() ) {
        return;
    }

    // loop until it's time to quit
    while( !glfwWindowShouldClose(w_window) ) {
        animate();
        if( updateDisplay ) {
            updateDisplay = false;
            display();
            glfwSwapBuffers( w_window );
        }
        glfwPollEvents();
    }
}
