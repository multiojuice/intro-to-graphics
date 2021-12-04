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
#include <SOIL/SOIL.h>

using namespace std;

// reflective coefficients
static glm::vec3 k( 0.5, 0.7f, 1.0f );

// specular highlights (order depends upon the Object type in Models.h)
static GLfloat specExp[N_OBJECTS] = {
    50.0f,  // Quad
    20.0f,  // Cylinder
    20.0f   // Discs
};

// These variables are used in the framework only when doing Phong shading
static glm::vec4 specular(        1.00f, 1.00f, 1.00f, 1.00f );
static glm::vec4 cyl_ambient(     0.50f, 0.10f, 0.90f, 1.00f );
static glm::vec4 cyl_diffuse(     0.89f, 0.00f, 0.00f, 1.00f );
static glm::vec4 quad_ambdiffuse( 0.10f, 0.85f, 0.20f, 1.00f );

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

    // Load in front of coin
    GLuint obverseTexture = SOIL_load_OGL_texture(
        "obverse.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);
    
    // Load in back of coin
    GLuint reverseTexture = SOIL_load_OGL_texture(
        "reverse.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);

    // Assign front of coin to 0
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, obverseTexture);
    
    // Assign back of coin to 1
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, reverseTexture);

    // Load in back of coin
    GLuint discTexture = SOIL_load_OGL_texture(
        "disc.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);

            // Load in back of coin
    GLuint wallTexture = SOIL_load_OGL_texture(
        "wall.png",
        SOIL_LOAD_AUTO,
        SOIL_CREATE_NEW_ID,
        SOIL_FLAG_INVERT_Y | SOIL_FLAG_TEXTURE_REPEATS);

    // Assign front of coin to 0
    glActiveTexture(GL_TEXTURE2);
    glBindTexture(GL_TEXTURE_2D, discTexture);
    
    // Assign back of coin to 1
    glActiveTexture(GL_TEXTURE3);
    glBindTexture(GL_TEXTURE_2D, wallTexture);

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

    switch( obj ) {
    case Quad:
        glUniform1i(glGetUniformLocation(program, "texfront"), 0);
        glUniform1i(glGetUniformLocation(program, "texback"), 1);
        break;
    // case Discs:
    //     glUniform1i(glGetUniformLocation(program, "texfront"), 2);
    //     break;
    // case Cylinder:
    //     glUniform1i(glGetUniformLocation(program, "texfront"), 3);
    //     break;
    }
}
