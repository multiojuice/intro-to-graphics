//
//  Materials.cpp
//
//  Simple class for setting up material property parameters.
//
//  Created by Warren R. Carithers 2021/11/11
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Owen Sullivan
//

#include <iostream>

#include "Materials.h"

#include "Models.h"
#include "Lighting.h"
#include "Utils.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

// this is here in case you are using SOIL;
// if you're not, it can be deleted.
// #include <SOIL/SOIL.h>

using namespace std;

// reflective coefficients
static glm::vec3 k( 0.4, 0.7f, 0.8f );

// specular highlights (order depends upon the Object type in Models.h)
static GLfloat specExp[N_OBJECTS] = {
    50.0f,  // Quad
    20.0f,  // Cylinder
    20.0f,   // Discs
    50.0f,   // Teapot
    20.0f,   // Cube
    10.0f,   // Basket
};

// These variables are used in the framework only when doing Phong shading
static glm::vec4 specular(        1.00f, 1.00f, 1.00f, 1.00f );
static glm::vec4 cyl_ambient( 221.0f / 255.0f, 221.0f / 255.0f, 210.0f / 255.0f, 1.00f );
static glm::vec4 cyl_diffuse( 221.0f / 255.0f, 221.0f / 255.0f, 210.0f / 255.0f, 1.00f );

static glm::vec4 tea_ambient( 153.0f / 255.0f, 187.0f / 255.0f, 169.0f / 255.0f, 1.00f );
static glm::vec4 tea_diffuse( 153.0f / 255.0f, 187.0f / 255.0f, 169.0f / 255.0f, 1.00f );

static glm::vec4 quad_ambdiffuse( 150.0f / 255.0f, 110.0f / 255.0f, 69.0f / 255.0f, 1.00f );

static glm::vec4 cube_amb( 81.0f / 255.0f, 96.0f / 255.0f, 67.0f / 255.0f, 1.00f );
static glm::vec4 cube_diffuse( 124.0f / 255.0f, 150.0f / 255.0f, 89.0f / 255.0f, 1.00f );

static glm::vec4 basket_ambdiffuse( 141.0f / 255.0f, 112.0f / 255.0f, 90.0f / 255.0f, 1.00f );


// Add any global definitions and/or variables you need here.

///
/// This function initializes all texture-related data structures for
/// the program.  This is where texture buffers should be created, where
/// texture images should be loaded, etc.
///
/// You will need to write this function, and maintain all of the values
/// needed to be sent to the various shaders.
///
void initTextures( void )
{
    // // Set texture unit 0
    // glActiveTexture(GL_TEXTURE0);
    // // Load in front of coin
    // GLuint obverseTexture = SOIL_load_OGL_texture(
    //     "obverse.png",
    //     SOIL_LOAD_AUTO,
    //     SOIL_CREATE_NEW_ID,
    //     SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
    
    // // Assign front of coin to 0
    // glBindTexture(GL_TEXTURE_2D, obverseTexture);

    // // Set texture unit to 1
    // glActiveTexture(GL_TEXTURE1);
    // // Load in back of coin
    // GLuint reverseTexture = SOIL_load_OGL_texture(
    //     "reverse.png",
    //     SOIL_LOAD_AUTO,
    //     SOIL_CREATE_NEW_ID,
    //     SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
    // // Assign back of coin to 1
    // glBindTexture(GL_TEXTURE_2D, reverseTexture);

    // // Set texture unit to 2
    // glActiveTexture(GL_TEXTURE2);
    // // Load in disc texture
    // GLuint discTexture = SOIL_load_OGL_texture(
    //     "disc.png",
    //     SOIL_LOAD_AUTO,
    //     SOIL_CREATE_NEW_ID,
    //     SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
    // // Assign disc to 2
    // glBindTexture(GL_TEXTURE_2D, discTexture);


    // // Set texture unit to 3
    // glActiveTexture(GL_TEXTURE3);
    // // Load in wall texture
    // GLuint wallTexture = SOIL_load_OGL_texture(
    //     "wall.png",
    //     SOIL_LOAD_AUTO,
    //     SOIL_CREATE_NEW_ID,
    //     SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
    // // Assign wall of cyl to 3
    // glBindTexture(GL_TEXTURE_2D, wallTexture);
}

///
/// This function sets up the appearance parameters for the object.
///
/// @param program        The ID of an OpenGL (GLSL) shader program to which
///                       parameter values are to be sent
/// @param obj            The object type of the object being drawn
/// @param usingTextures  Are we texture-mapping this object?
///
void setMaterials( GLuint program, Object obj, bool usingTextures )
{
    ///////////////////////////////////////////////////
    // CODE COMMON TO PHONG SHADING AND TEXTURE MAPPING
    //
    // DO NOT REMOVE THIS SECTION OF CODE
    ///////////////////////////////////////////////////

    // Set the specular exponent for the object
    GLint loc  = getUniformLoc( program, "specExp" );
    switch( obj ) {
    case Quad:
        glUniform1f( loc, specExp[Quad] );
        break;
    case Cylinder:
        glUniform1f( loc, specExp[Cylinder] );
        break;
    case Discs:
        glUniform1f( loc, specExp[Discs] );
        break;
    case Teapot:
        glUniform1f( loc, specExp[Teapot] );
        break;
    case Cube:
        glUniform1f( loc, specExp[Cube] );
        break;
    }

    // Send down the reflective coefficients
    loc = getUniformLoc( program, "kCoeff" );
    if( loc >= 0 ) {
        glUniform3fv( loc, 1, glm::value_ptr(k) );
    }

    ///////////////////////////////////////////////////////////
    // CODE DIFFERING BETWEEN PHONG SHADING AND TEXTURE MAPPING
    ///////////////////////////////////////////////////////////

    //
    // Check to see whether or not we are using texture mapping
    // on the object being drawn at the moment.
    //

    if( !usingTextures ) {

        ///////////////////////////////////////////////////////////
        // THIS CODE IS USED ONLY IF TEXTURE MAPPING IS ***NOT***
        // BEING DONE ON THIS OBJECT
        ///////////////////////////////////////////////////////////

        // specular color is identical for the objects
        loc = getUniformLoc( program, "specularColor" );
        if( loc >= 0 ) {
            glUniform4fv( loc, 1, glm::value_ptr(specular) );
        }

        // ambient and diffuse vary from one object to another
        GLint dloc = getUniformLoc( program, "diffuseColor" );
        GLint aloc = getUniformLoc( program, "ambientColor" );

        switch( obj ) {
        case Quad:
            if( aloc >= 0 ) {
                glUniform4fv( aloc, 1, glm::value_ptr(quad_ambdiffuse) );
            }
            if( dloc >= 0 ) {
                glUniform4fv( dloc, 1, glm::value_ptr(quad_ambdiffuse) );
            }
            break;
        case Cylinder: // FALL THROUGH
        case Discs:
            if( aloc >= 0 ) {
                glUniform4fv( aloc, 1, glm::value_ptr(cyl_ambient) );
            }
            if( dloc >= 0 ) {
                glUniform4fv( dloc, 1, glm::value_ptr(cyl_diffuse) );
            }
            break;
        case Teapot:
            if( aloc >= 0 ) {
                glUniform4fv( aloc, 1, glm::value_ptr(tea_ambient) );
            }
            if( dloc >= 0 ) {
                glUniform4fv( dloc, 1, glm::value_ptr(tea_diffuse) );
            }
            break;

        case Cube:
            if( aloc >= 0 ) {
                glUniform4fv( aloc, 1, glm::value_ptr(cube_amb) );
            }
            if( dloc >= 0 ) {
                glUniform4fv( dloc, 1, glm::value_ptr(cube_diffuse) );
            }
            break;

        case Basket:
            if( aloc >= 0 ) {
                glUniform4fv( aloc, 1, glm::value_ptr(basket_ambdiffuse) );
            }
            if( dloc >= 0 ) {
                glUniform4fv( dloc, 1, glm::value_ptr(basket_ambdiffuse) );
            }
            break;
        }

        return;
    }

    ///////////////////////////////////////////////////////////
    // CODE FROM THIS POINT ON WILL BE USED ONLY IF TEXTURE
    // MAPPING IS BEING DONE ON THIS OBJECT
    ///////////////////////////////////////////////////////////

    //
    // You will need to add code here to implement texture mapping.
    //

    // Set everything based on the texture units in 
    // init_textures.
    // aditionally set everything to the texfront for front facing textures
    // and just the back of coin to texback to use for when openGL senses it is not frontfacing
    // switch( obj ) {
    // case Quad:
    //     glUniform1i(glGetUniformLocation(program, "texfront"), 0);
    //     glUniform1i(glGetUniformLocation(program, "texback"), 1);
    //     break;
    // case Discs:
    //     glUniform1i(glGetUniformLocation(program, "texfront"), 2);
    //     break;
    // case Cylinder:
    //     glUniform1i(glGetUniformLocation(program, "texfront"), 3);
    //     break;
    // }
}
