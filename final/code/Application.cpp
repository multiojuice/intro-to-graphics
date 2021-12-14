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
#include <vector>

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
static const char *vs_p = "phong.vert";
static const char *fs_p = "phong.frag";
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

// which object(s) to texture map
static bool map_obj[N_OBJECTS];

// object transformations
static glm::vec3 quad_s(8,  5.5f,  1.75f );
static glm::vec3 quad_x( 0, 0, -0.3f );

static glm::vec3 bar_s(1,  2.0f,  2.0f );
static glm::vec3 bar_x( 0,2, 0.0f );

static glm::vec3 cyl_s( 0.55f,  0.55f,  0.55f );
static glm::vec3 cyl_x( 1.0f,  0.5f, 1.5f );

static glm::vec3 rect_s( 2.9f,  1.0f, 1.25f );
static glm::vec3 rect_x( 0.0f,  0.75f, -0.15f );

static glm::vec3 basket_s( 0.8f,  0.8f, 0.8f );
static glm::vec3 basket_x( 0.75f,  1.6f, 0.7f );

static glm::vec3 tea_s( 1.75f,  1.75f, 1.75f );
static glm::vec3 tea_x( -0.75f,  1.4f, 0.6f );


static vector<glm::vec3>* createScalars() {
    vector<glm::vec3> arr[N_OBJECTS];

    arr[Quad].push_back(glm::vec3( 0, 0, -0.3f ));
    glm::vec3 cyl = glm::vec3( 1.0f,  0.5f, 1.5f );
    arr[Cylinder].push_back(cyl);
    arr[Discs].push_back(cyl);
    arr[Basket].push_back(glm::vec3( 0.75f,  1.6f, 0.7f ));
    arr[Teapot].push_back(glm::vec3( -0.75f,  1.4f, 0.6f ));
    
    arr[Cube].push_back(glm::vec3( 0.0f,  0.75f, -0.15f ));
    arr[Cube].push_back(bar);
    arr[Cube].push_back(bar);
    arr[Cube].push_back(bar);

    return arr;
} 

static vector<glm::vec3>* createTranslate() {
    vector<glm::vec3> arr[N_OBJECTS];

    arr[Quad].push_back(glm::vec3(8,  5.5f,  1.75f ));
    glm::vec3 cyl = glm::vec3( 0.55f,  0.55f,  0.55f );
    arr[Cylinder].push_back(cyl);
    arr[Discs].push_back(cyl);
    arr[Basket].push_back(glm::vec3( 0.8f,  0.8f, 0.8f ));
    arr[Teapot].push_back(glm::vec3( 1.75f,  1.75f, 1.75f ));
    
    glm::vec3 bar = glm::vec3( 0.75f,  0.75f, 0.75f);
    arr[Cube].push_back(glm::vec3( 2.9f,  1.0f, 1.25f ));
    arr[Cube].push_back(glm::vec3( -1.0f,  3.0f, 0.0f));
    arr[Cube].push_back(glm::vec3(1.0f,  3.0f, 0.0f));
    arr[Cube].push_back(glm::vec3(0.0f,  3.0f, 0.0f));

    return arr;
} 

static vector<glm::vec3>* createRotations() {
    vector<glm::vec3> arr[N_OBJECTS];

    arr[Quad].push_back(glm::vec3( -87, 0, 0 ));
    arr[Discs].push_back(glm::vec3( 180, 0, 0 ));
    arr[Cylinder].push_back(glm::vec3( 180, 0, 0 ));
    arr[Teapot].push_back(glm::vec3( 3, 30, 0 ));

    arr[Cube].push_back(glm::vec3( 3, 0, 0 ));
    arr[Cube].push_back(glm::vec3( 3, 0, 0 ));
    arr[Cube].push_back(glm::vec3( 3, 0, 0 ));
    arr[Cube].push_back(glm::vec3( 3, 0, 0 ));

    arr[Basket].push_back(glm::vec3( 3, -25, 0 ));

    return arr;
} 




static vector<glm::vec3>* scale_vec = createScalars();
static vector<glm::vec3>* trans_vecs = createTranslate();
static vector<glm::vec3>* rot_vecs = createRotations();



static glm::vec3 lemons_s[3] = {
    glm::vec3( 1.75f,  1.75f, 1.75f),
    glm::vec3(1.75f,  1.75f, 1.75f),
    glm::vec3(1.75f,  1.75f, 1.75f)
};

static glm::vec3 lemons_x[3] = {
    glm::vec3( 0.0f,  0.0f, 0.0f),
    glm::vec3(0.0f,  0.0f, 0.0f),
    glm::vec3(0.0f,  0.0f, 0.0f)
};

static glm::vec3 bars_s[3] = {
    glm::vec3( 0.75f,  0.75f, 0.75f),
    glm::vec3(0.75f,  0.75f, 0.75f),
    glm::vec3(0.75f,  0.75f, 0.75f)
};

static glm::vec3 bars_x[3] = {
    glm::vec3( -1.0f,  3.0f, 0.0f),
    glm::vec3(1.0f,  3.0f, 0.0f),
    glm::vec3(0.0f,  3.0f, 0.0f)
};


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
const char *w_title = "Still Life Lemon Bars";

// GL context we're using (we assume 3.2, for GLSL 1.50)
int gl_maj = 3;
int gl_min = 2;

// our GLFWwindow
GLFWwindow *w_window;

//
// PRIVATE FUNCTIONS
//

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
    createObject( C, Basket,    buffers[Basket] );
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

    // light position

    case GLFW_KEY_I: // move the light into the scene
        lightpos[2] -= 2.0f;
        break;

    case GLFW_KEY_O: // move the light out from the scene
        lightpos[2] += 2.0f;
        break;

    // print out potentially useful information

    case GLFW_KEY_P: // light position
        cerr << "Light is at (" << lightpos[0] << "," << lightpos[1]
             << "," << lightpos[2] << ")" << endl;
        break;

    // Reset parameters

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
/// Display the current image
///
static void display( void )
{
    // clear the frame buffer
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

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
        glm::vec3 ang( 0.0f, 0.0f, 0.0f );

        // send all the transformation data
        switch( obj ) {
        case Quad:
            ang = glm::vec3( -87, 0, 0 );
            setTransforms( program, quad_s, ang, quad_x );
            break;
        case Cylinder:  // FALL THROUGH
        case Discs:
            ang = glm::vec3( 180, 0, 0 );
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
        case Basket:
            ang = glm::vec3( 3, -25, 0 );
            setTransforms( program, basket_s, ang, basket_x );
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
    // OWEN -> changed color
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
    for( int i = 0; i < N_OBJECTS; ++i ) {
        map_obj[i] = false;
    }

    // set up the objects and the scene
    if( !init() ) {
        return;
    }

    checkErrors( "after init" );

    // loop until it's time to quit
    while( !glfwWindowShouldClose(w_window) ) {
        if( updateDisplay ) {
            updateDisplay = false;
            display();
            glfwSwapBuffers( w_window );
            checkErrors( "event loop" );
        }
        glfwPollEvents();
    }
}
