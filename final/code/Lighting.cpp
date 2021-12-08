//
//  Lighting
//
//  Simple module for setting up the parameters for lighting and shading
//  for the Texture Mapping Assignment.
//
//  Created by Warren R. Carithers 2019/11/18.
//  Updated: 2021/11/07 by wrc
//  Copyright 2021 Rochester Institute of Technology.  All rights reserved.
//
//  Students should not modify this file.
//

#include "Lighting.h"

#include "Models.h"
#include "Utils.h"

#include <glm/vec4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;

// publicly-available lighting data
// light position (original and current)
glm::vec4 lpDefault( 15.0f, 18.0f, 20.0f, 1.0f );
glm::vec4 lightpos(  15.0f, 18.0f, 20.0f, 1.0f );

// private lighting data
static glm::vec4 lightcolor( 1.0f, 1.0f, 1.0f, 1.0f );
static glm::vec4 amblight(   0.7f, 0.7f, 0.7f, 1.0f );

///
/// This function sets up the lighting, material, and shading parameters
/// for the shaders.
///
/// @param program  The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
///
void setLighting( GLuint program )
{
    GLint loc;

    // Lighting parameters
    loc = getUniformLoc( program, "lightPosition" );
    if( loc >= 0 ) {
        glUniform4fv( loc, 1, glm::value_ptr(lightpos) );
    }

    loc = getUniformLoc( program, "lightColor" );
    if( loc >= 0 ) {
        glUniform4fv( loc, 1, glm::value_ptr(lightcolor) );
    }

    loc = getUniformLoc( program, "ambientLight" );
    if( loc >= 0 ) {
        glUniform4fv( loc, 1, glm::value_ptr(amblight) );
    }
}
