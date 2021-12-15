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
#include <stdio.h>
#include <stdlib.h>

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
    // I use a factory method setup to generate the structures i needed for a simpler code segment
static vector<glm::vec3>* createTranslate() {
    vector<glm::vec3>* arr = (vector<glm::vec3>*) malloc(sizeof(vector<glm::vec3>) * N_OBJECTS);

    arr[Quad].push_back(glm::vec3( 0, 0, -0.3f ));
    glm::vec3 cyl = glm::vec3( 1.0f,  0.5f, 1.5f );
    arr[Cylinder].push_back(cyl);
    arr[Discs].push_back(cyl);
    arr[Basket].push_back(glm::vec3( 0.75f,  1.6f, 0.35f ));
    arr[Teapot].push_back(glm::vec3( -0.75f,  1.4f, 0.6f ));
    
    arr[Sphere].push_back(glm::vec3( 0.95f,  1.95f, .625f ));
    arr[Sphere].push_back(glm::vec3( 0.575f,  2.0f, 0.35f ));
    arr[Sphere].push_back(glm::vec3( 0.6f,  1.75f, 0.55f ));

    arr[Cube].push_back(glm::vec3( 0.0f,  0.75f, -0.15f ));
    // Plate
    arr[Cube].push_back(glm::vec3( -0.25f,  .6f, 1.75f ));

    // Napkin
    arr[Cube].push_back(glm::vec3( -0.85f,  .6f, 1.75f ));

    // Lemon Bars
    arr[Cube].push_back(glm::vec3( -0.1f,  1.0f, 2.3f));
    arr[Cube].push_back(glm::vec3(-0.35f,  .8f, 1.6f));
    arr[Cube].push_back(glm::vec3(-0.45f,  1.0f, 1.85f));

    return arr;
} 

static vector<glm::vec3>* createScalars() {
    vector<glm::vec3>* arr = (vector<glm::vec3>*) malloc(sizeof(vector<glm::vec3>) * N_OBJECTS);

    arr[Quad].push_back(glm::vec3(8,  5.5f,  1.75f ));
    glm::vec3 cyl = glm::vec3( 0.55f,  0.55f,  0.55f );
    arr[Cylinder].push_back(cyl);
    arr[Discs].push_back(cyl);
    arr[Basket].push_back(glm::vec3( 0.8f,  0.8f, 0.8f ));
    arr[Teapot].push_back(glm::vec3( 1.75f,  1.75f, 1.75f ));

    // Lemons
    arr[Sphere].push_back(glm::vec3( 0.5f,  .4f, .4f ));
    arr[Sphere].push_back(glm::vec3( 0.5f,  .4f, .4f ));
    arr[Sphere].push_back(glm::vec3( 0.5f,  .4f, .4f ));

    
    arr[Cube].push_back(glm::vec3( 2.9f,  1.0f, 1.25f ));
    // Plate
    arr[Cube].push_back(glm::vec3( 0.5f,  0.15f, 1.0f ));

    // Napkin
    arr[Cube].push_back(glm::vec3( 0.3f,  0.10f, 1.2f ));

    // Lemon Bars
    glm::vec3 bar = glm::vec3( 0.2f,  0.075f, 0.5f );
    arr[Cube].push_back(glm::vec3( 0.2f,  0.075f, 0.4f ));
    arr[Cube].push_back(bar);
    arr[Cube].push_back(bar);

    return arr;
} 

static vector<glm::vec3>* createRotations() {
    vector<glm::vec3>* arr = (vector<glm::vec3>*) malloc(sizeof(vector<glm::vec3>) * N_OBJECTS);

    arr[Quad].push_back(glm::vec3( -87, 0, 0 ));
    arr[Discs].push_back(glm::vec3( 180, 0, 0 ));
    arr[Cylinder].push_back(glm::vec3( 180, 0, 0 ));
    arr[Teapot].push_back(glm::vec3( 3, 30, 0 ));
    arr[Sphere].push_back(glm::vec3( 1.0f,  1.0f, 45.0f ));
    arr[Sphere].push_back(glm::vec3( 1.0f,  40.0f, 1.0f ));
    arr[Sphere].push_back(glm::vec3( 1.0f,  1.0f, 1.0f ));


    arr[Cube].push_back(glm::vec3( 3, 0, 0 ));
    // Plate
    arr[Cube].push_back(glm::vec3( 3, 32, 0 ));

    // Napkin
    arr[Cube].push_back(glm::vec3( 3, 28, 0 ));

    // Lemon bars
    arr[Cube].push_back(glm::vec3( 12, 25, -5 ));
    arr[Cube].push_back(glm::vec3( 3, 90, 0 ));
    arr[Cube].push_back(glm::vec3( 3, 80, 0 ));

    arr[Basket].push_back(glm::vec3( 3, -25, 0 ));

    return arr;
} 

static vector<glm::vec3>* scale_vecs= createScalars();
static vector<glm::vec3>* trans_vecs = createTranslate();
static vector<glm::vec3>* rot_vecs = createRotations();

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
    createObject( C, Sphere,    buffers[Sphere] );
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
        
        for (int i = 0; i < scale_vecs[obj].size(); i++) {
            setMaterials( program, (Object) obj, map_obj[obj] , i);
            setTransforms( program, scale_vecs[obj][i], rot_vecs[obj][i], trans_vecs[obj][i] );

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

    map_obj[Cylinder] = true;
    map_obj[Discs] = true;
    map_obj[Quad] = true;

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
