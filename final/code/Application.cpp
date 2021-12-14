//
//  Application.cpp
//
//  Assignment-specific code.
//
//  Created by Warren R. Carithers on 2019/09/09.
//  Updates: 2021/11/11 by wrc
//  Based on earlier versions created by Joe Geigel and Warren R. Carithers
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
//

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
#include "Materials.h"
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
static const char *vs_p = "p150.vert";
static const char *fs_p = "p150.frag";
static const char *vs_t = "texture.vert";
static const char *fs_t = "texture.frag";

// our Canvas
static Canvas *canvas;

// buffers for our shapes
static BufferSet buffers[N_OBJECTS];

// shader program handles
static GLuint phong, texture;

// our VAO
static GLuint vao;

// do we need to do a display() call?
static bool updateDisplay = true;

// animation control
static bool animating[N_OBJECTS];  // individual animation flags
static float angles[N_OBJECTS];    // individual rotation angles

// which object(s) to texture map
static bool map_obj[N_OBJECTS];

// object transformations
static glm::vec3 quad_s(8,  5.5f,  1.75f );
static glm::vec3 quad_x( 0, 0, -0.3f );


static glm::vec3 cyl_s( 0.5f,  0.5f,  0.5f );
static glm::vec3 cyl_x( 1.0f,  0.5f, 1.75f );


static glm::vec3 rect_s( 2.9f,  1.0f, 1.25f );
static glm::vec3 rect_x( 0.0f,  0.75f, -0.15f );

static glm::vec3 tea_s( 1.75f,  1.75f, 1.75f );
static glm::vec3 tea_x( -0.75f,  1.4f, 0.6f );


// light animation
static bool animateLight = false;
static float delta = 1.0f;
static const float lightMax = 50.0f, lightMin = -50.0f;

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
const char *w_title = "Texture Mapping";

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
    bool mapAll = true;

    // command-line arguments specify which objects to texture-map
    for( int i = 1; i < argc; ++i ) {
        switch( argv[i][0] ) {
        case 'q':
            mapAll = false;  map_obj[Quad] = true;
            break;
        case 'c':
            mapAll = false;  map_obj[Cylinder] = true;
            break;
        case 'd':
            mapAll = false;  map_obj[Discs] = true;
            break;
        case 'x':
            mapAll = false;
            break;
        default:
            cerr << "bad object character '" << argv[i][0]
                 << "' ignored" << endl;
        }
    }

    // if we're mapping everything, indicate that
    if( mapAll ) {
        for( int i = 0; i < N_OBJECTS; ++i ) {
            map_obj[i] = true;
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
    createObject( C, Quad,     buffers[Quad] );
    createObject( C, Cylinder, buffers[Cylinder] );
    createObject( C, Discs,    buffers[Discs] );
    createObject( C, Teapot,    buffers[Teapot] );
    createObject( C, Cube,    buffers[Cube] );
}

///
/// Increment or reset a rotation angle.
///
/// @param obj  the object being rotated
///
static void rotate( int obj, float amt ) {
    angles[obj] += amt;
    if( angles[obj] >= 360.0f ) {
        angles[obj] = 0.0f;
    }
}

///
/// Animation routine
///
static void animate( void ) {

    if( animating[Quad] ) {
        rotate( Quad, 2.0f );
        updateDisplay = true;
    }

    if( animating[Cylinder] ) {
        rotate( Cylinder, 1.0f );
        rotate( Discs, 1.0f );
        updateDisplay = true;
    }

    if( animateLight ) {
        if( (delta > 0.0f && lightpos.x >= lightMax) ||
            (delta < 0.0f && lightpos.x <= lightMin) ) {
            delta *= -1.0f;
        }
        lightpos.x += delta;
        updateDisplay = true;
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
    bool any_anim = false;

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

    // animation control

    case GLFW_KEY_A:    // start/stop all object animation
        // NOTE: does not affect light source animation!
        // are we currently animating anything?
        for( int i = 0; i < N_OBJECTS; ++i ) {
            any_anim = any_anim || animating[i];
        }
        // invert the condition
        any_anim = !any_anim;
        // propogate it
        for( int i = 0; i < N_OBJECTS; ++i ) {
            animating[i] = any_anim;
        }
        break;

    case GLFW_KEY_S: // start/stop animating the quad (square)
        animating[Quad] = !animating[Quad];
        break;

    case GLFW_KEY_C: // start/stop animating the cylinder and discs
        animating[Cylinder] = !animating[Cylinder];
        animating[Discs] = !animating[Discs];
        break;

    case GLFW_KEY_L:    // start/stop animating the light
        animateLight = !animateLight;
        break;

    // light position

    case GLFW_KEY_I: // move the light into the scene
        lightpos[2] -= 2.0f;
        break;

    case GLFW_KEY_O: // move the light out from the scene
        lightpos[2] += 2.0f;
        break;

    // print out potentially useful information

    case GLFW_KEY_R: // rotation angles
        cerr << "Rotation: quad " << angles[Quad]
             << ", cyl/disc " << angles[Cylinder] << endl;
        break;

    case GLFW_KEY_P: // light position
        cerr << "Light is at (" << lightpos[0] << "," << lightpos[1]
             << "," << lightpos[2] << ")" << endl;
        break;

    // Reset parameters

    case GLFW_KEY_1: // reset all object rotations
        angles[Quad] = angles[Cylinder] = angles[Discs] = 0.0f;
        break;

    case GLFW_KEY_2: // reset light position
        lightpos[0] = lpDefault[0];
        lightpos[1] = lpDefault[1];
        lightpos[2] = lpDefault[2];
        lightpos[3] = lpDefault[3];
        break;

    // help message
    case GLFW_KEY_H:
        cout << "  Key(s)             Action" << endl;
        cout << "=========   =======================" << endl;
        cout << "ESC, q, Q   Terminate the program" << endl;
        cout << "  h, H      Print this message" << endl;
        cout << "  a, A      Toggle all object animations" << endl;
        cout << "  s, S      Toggle square (quad) animation" << endl;
        cout << "  c, C      Toggle cylinder animation" << endl;
        cout << "  l, L      Toggle light animation" << endl;
        cout << "  i, I      Move light toward the objects" << endl;
        cout << "  o, O      Move light away from the objects" << endl;
        cout << "  p, P      Print light position" << endl;
        cout << "  r, R      Print rotation angles" << endl;
        cout << "   1        Reset all object rotations" << endl;
        cout << "   2        Reset light position" << endl;
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

    strcpy( buf, "Texture mapping:" );
    if( map_obj[Quad] )     strcat( buf, " Quad" );
    if( map_obj[Cylinder] ) strcat( buf, " Cylinder" );
    if( map_obj[Discs] )    strcat( buf, " Discs" );

    glfwSetWindowTitle( w_window, buf );
}

///
/// Display the current image
///
static void display( void )
{
    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    // set our titlebar
    setTitle();

    // check for any errors to this point
    checkErrors( "display init" );

    // draw the individual objects
    for( int obj = Quad; obj < N_OBJECTS; ++obj ) {

        // select the proper shader program
        GLuint program = map_obj[obj] ? texture : phong;
        glUseProgram( program );

        // set up the common transformations
        setCamera( program );
        setProjection( program );
        checkErrors( "display camera" );

        // and our lighting
        setLighting( program );
        checkErrors( "display lighting" );

        // set texture parameters OR material properties
        setMaterials( program, (Object) obj, map_obj[obj] );
        checkErrors( "display materials" );

        // select the correct rotation angles
        glm::vec3 ang( angles[obj], angles[obj], angles[obj] );

        // send all the transformation data
        switch( obj ) {
        case Quad:
            ang = glm::vec3( -87, 0, 0 );
            setTransforms( program, quad_s, ang, quad_x );
            break;
        case Cylinder:  // FALL THROUGH
        case Discs:
            setTransforms( program, cyl_s, ang, cyl_x );
            break;
        case Teapot:
            ang = glm::vec3( 3, 30, 0 );
            setTransforms( program, tea_s, ang, tea_x );
            break;
        case Cube:
            ang = glm::vec3( 3, 0, 0 );
            setTransforms( program, rect_s, ang, rect_x );
            break;
        }

        checkErrors( "display xforms" );

        // draw it
        buffers[obj].selectBuffers( program,
            "vPosition", NULL, "vNormal", 
            map_obj[obj] ? "vTexCoord" : NULL );
        checkErrors( "display select" );

        glDrawElements( GL_TRIANGLES, buffers[obj].numElements,
                        GL_UNSIGNED_INT, (void *) 0 );
        checkErrors( "display draw" );
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
    if( gl_maj < 3 || (gl_maj == 3 && gl_min < 2) ) {
        // select the other Phong shader
        vs_p = "p120.vert";
        fs_p = "p120.frag";
        // warn about GLSL versions
        cerr << "Caution: GL version may not allow GLSL 1.50+"
             << " code to compile!" << endl;
    }
    checkErrors( "init start" );

    // Load shaders and use the resulting shader program
    ShaderError error;
    phong = shaderSetup( vs_p, fs_p, &error );
    if( !phong ) {
        cerr << "Error setting up phong shader - "
             << errorString(error) << endl;
        return( false );
    }
    checkErrors( "init shaders 1" );

    texture = shaderSetup( vs_t, fs_t, &error );
    if( !texture ) {
        cerr << "Error setting up texture shader - "
             << errorString(error) << endl;
        return( false );
    }
    checkErrors( "init shaders 2" );

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
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
    glClearColor( 42.0f / 255.0f, 35.0f / 255.0f, 33.0f / 255.0f, 0.0f );
    glDepthFunc( GL_LEQUAL );
    glClearDepth( 1.0f );
    checkErrors( "init setup" );

    // create the geometry for our shapes.
    createImage( *canvas );
    checkErrors( "init image" );

    // initialize all texture-related things
    initTextures();
    checkErrors( "init textures" );

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
        for( int i = 0; i < N_OBJECTS; ++i ) {
            map_obj[i] = true;
        }
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
