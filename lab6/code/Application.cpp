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

#include <cstdio>
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

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Application.h"

#include "Buffers.h"
#include "Canvas.h"
#include "Lighting.h"
#include "Models.h"
#include "ShaderSetup.h"
#include "Types.h"
#include "Utils.h"
#include "Viewing.h"

using namespace std;

//
// PRIVATE GLOBALS
//

// names of our GLSL shader files
static const char *vs_g = "gouraud.vert";
static const char *fs_g = "gouraud.frag";
static const char *vs_p = "phong.vert";
static const char *fs_p = "phong.frag";

// our Canvas
static Canvas *canvas;

// buffers for our shapes
static BufferSet torus, teapot;

// shader program handle
static GLuint program, gouraud, phong;

// our VAO
static GLuint vao;

// do we need to do a display() call?
static bool updateDisplay = true;

// are we animating?
static bool animating = false;

// object animation rotation angles
static glm::vec2 angles( 0.0f, 0.0f );

// torus transformations
static glm::vec3 tor_scale(  2.5f, 2.5f,  2.5f );
static glm::vec3 tor_xlate( -1.5f, 0.5f, -1.5f );

// teapot transformations
static glm::vec3 tea_scale( 2.0f, 2.0f,  2.0f );
static glm::vec3 tea_xlate( 1.0f, 0.5f, -1.5f );

// current shading model
static Shader currentShader;

//
// PUBLIC GLOBALS
//

//
// Drawing-related variables
//

// dimensions of the drawing window
int w_width  = 600;
int w_height = 600;

// drawing window title
const char *w_title = "Shading";

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
    currentShader = Gouraud;
    
    // too many arguments: ignore them all
    if( argc > 2 ) {
        cerr << "Usage: " << argv[0] << " [ shader_number ]" << endl;
        return;
    }

    // if there was an argument, try to convert it
    if( argc == 2 ) {
        long int n = strtol( argv[1], &endptr, 10 );
        if( endptr == argv[1] || *endptr != '\0' ) { // no/bad conversion
            cerr << "Bad shader # '" << argv[1] << "' ignored" << endl;
        } else {
            if( n >= 0 && n < N_SHADERS ) {
                currentShader = (Shader) n;
            } else {
                cerr << "Bad shader # '" << argv[1] << "' ignored" << endl;
            }
        }
    }
}

///
/// Create our shapes
///
/// @param C  - the Canvas to use when drawing the objects
///
static void createImage( Canvas &C )
{
    // make the torus
    C.clear();
    createObject( C, Torus );
    torus.createBuffers( C );

    // make the teapot
    C.clear();
    createObject( C, Teapot );
    teapot.createBuffers( C );

}

///
/// Animation routine
///
static void animate( void ) {
    static unsigned int calls = 0;   // track calls while animating

    if( animating ) {

        // only trigger after every four calls
        ++calls;
        if( (calls & 3) != 0 ) {
            return;
        }

        angles[Torus] += 2.0f;
        if( angles[Torus] >= 360.0f ) {
            angles[Torus] -= 360.0f;
        }

        angles[Teapot] += 1.0f;
        if( angles[Teapot] >= 360.0f ) {
            angles[Teapot] -= 360.0f;
        }

        updateDisplay   = true;
    }
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

    // termination
    case GLFW_KEY_ESCAPE:  // FALL THROUGH
    case GLFW_KEY_Q:
        glfwSetWindowShouldClose( w_window, 1 );
        // return without updating the display
        return;
        // NOTREACHED

    // automated animation
    case GLFW_KEY_A:
        animating = !animating;
        break;

    // report angles of rotation
    case GLFW_KEY_O:
        cout << "Angles:  Torus "    << angles[Torus]
             << ", Teapot " << angles[Teapot] << endl;
        // return without updating the display
        return;
        // NOTREACHED

    // reset angles of rotation
    case GLFW_KEY_R:
        angles = glm::vec2(0.0f);
        break;

    // select shading type
    case GLFW_KEY_G:    // Gouraud
        program = gouraud;
        currentShader = Gouraud;
        break;

    case GLFW_KEY_P:    // Phong
        program = phong;
        currentShader = Phong;
        break;

    // help message
    case GLFW_KEY_H:
        cout << "  Key(s)             Action" << endl;
        cout << "=========   =======================" << endl;
        cout << "ESC, q, Q   Terminate the program" << endl;
        cout << "  h, H      Print this message" << endl;
        cout << "  g, G      Select Gouraud shading" << endl;
        cout << "  p, P      Select Phong shading" << endl;
        cout << "  a, A      Toggle animation" << endl;
        cout << "  o, O      Print the current rotation angles" << endl;
        cout << "  r, R      Reset rotation angles" << endl;
        // return without updating the display
        return;
        // NOTREACHED

    default:
        cerr << "Unknown keycode " << key << " ignored" << endl;
        // return without updating the display
        return;
        // NOTREACHED
    }

    updateDisplay = true;
}

///
/// Set the titlebar in the display window
///
void setTitle( void ) {
    char buf[128];

    sprintf( buf, "%s shading", shaders[currentShader] );
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

    // set our titlebar
    setTitle();

    // check for any errors to this point
    checkErrors( "display init" );

    // set up the common transformations
    setProjection( program );
    checkErrors( "display projection" );
    setCamera( program );
    checkErrors( "display camera" );

    // first, draw the torus

    // set up our object transformations
    glm::vec3 ang( angles[Torus] );
    setTransforms( program, tor_scale, ang, tor_xlate );
    checkErrors( "display torus xforms" );

    // and our lighting
    setLighting( program, Torus );
    checkErrors( "display torus lighting" );

    // bind the object buffers
    torus.selectBuffers( program, "vPosition", NULL, "vNormal", NULL );
    checkErrors( "display torus select" );

    glDrawElements( GL_TRIANGLES, torus.numElements,
            GL_UNSIGNED_INT, (void *) 0 );
    checkErrors( "display torus select" );

    // now, draw the teapot

    ang = glm::vec3( angles[Teapot] );
    setTransforms( program, tea_scale, ang, tea_xlate );
    checkErrors( "display teapot xforms" );

    setLighting( program, Teapot );
    checkErrors( "display teapot lighting" );

    teapot.selectBuffers( program, "vPosition", NULL, "vNormal", NULL );
    checkErrors( "display teapot select" );

    glDrawElements( GL_TRIANGLES, teapot.numElements,
                    GL_UNSIGNED_INT, (void *)0 );
    checkErrors( "display teapot draw" );
}

///
/// OpenGL initialization
///
/// @return boolean indicating status of initialization
///
static bool init( void )
{
    // Check the OpenGL major version
    if( gl_maj < 3 || (gl_maj == 3 && gl_min < 2) ) {
        cerr << "Caution: GL version may not allow GLSL 1.50+"
             << " code to compile!" << endl;
    }

    // Load shaders and use the resulting shader program
    ShaderError error;
    gouraud = shaderSetup( vs_g, fs_g, &error );
    if( !gouraud ) {
        cerr << "Error setting up gouraud shaders - "
             << errorString(error) << endl;
        return( false );
    }

    phong = shaderSetup( vs_p, fs_p, &error );
    if( !phong ) {
        cerr << "Error setting up phong shaders - "
             << errorString(error) << endl;
        return( false );
    }

#ifdef DEBUG
    // Define the CPP symbol 'DEBUG' and recompile to enable the compilation
    // of this code into your program for debugging purposes

    // Dump the list of active global variables in the shader programs
    cout << "Shader actives" << endl;
    cout << "----------" << endl << "Gouraud shader " << gouraud << endl;
    dumpActives( gouraud );
    cout << "----------" << endl << "Phong shader " << phong << endl;
    dumpActives( phong );
#endif

    // select the shader program
    switch( currentShader ) {
    case Gouraud:   program = gouraud;  break;
    case Phong:     program = phong;    break;
    default:
        cerr << "error - bad shader code " << currentShader << endl;
        return( false );
    }
    glUseProgram( program );

    checkErrors( "init program" );

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
    checkErrors( "init vao" );

    // OpenGL state initialization
    glEnable( GL_DEPTH_TEST );
    glEnable( GL_CULL_FACE );
    glCullFace( GL_BACK );
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );
    checkErrors( "init setup" );

    // create the geometry for our shapes.
    createImage( *canvas );
    checkErrors( "init image" );

    // register our callbacks
    glfwSetKeyCallback( w_window, keyboard );
    checkErrors( "init callback" );

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
        currentShader = Gouraud;
    }

    // set up the objects and the scene
    if( !init() ) {
        return;
    }

    checkErrors( "after init" );

    // loop until it's time to quit
    while( !glfwWindowShouldClose(w_window) ) {
        animate();
        if( updateDisplay ) {
            updateDisplay = false;
            display();
            glfwSwapBuffers( w_window );
            checkErrors( "event loop" );
        }
        glfwPollEvents();
    }
}
