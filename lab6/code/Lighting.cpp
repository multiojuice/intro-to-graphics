//
//  Lighting
//
//  Simple module for setting up the parameters for lighting and shading
//  for the Shading Assignment.
//
//  Created by Warren R. Carithers 2019/11/18.
//  Updated: 2021/11/07 by wrc
//  Copyright 2021 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  Owen Sullivan
//

#include "Lighting.h"

#include "Models.h"
#include "Utils.h"

using namespace std;

//////////////////////////////////////////////////////
// Add any global variables you need here.
//////////////////////////////////////////////////////

///
/// This function sets up the lighting, material, and shading parameters
/// for the shaders.
///
/// You will need to write this function, and maintain all of the values
/// needed to be sent to the vertex shader.
///
/// @param program  The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
/// @param obj      Which object is currently being drawn
///
void setLighting( GLuint program, Shape obj )
{
    // get the locations of all the shape data
    GLint shape_ambient = glGetUniformLocation( program, "shape_ambient" );
    GLint shape_amb_ref_coef = glGetUniformLocation( program, "shape_amb_ref_coef" );
    GLint shape_diffuse_color = glGetUniformLocation( program, "shape_diffuse_color" );
    GLint shape_diff_ref_coef = glGetUniformLocation( program, "shape_diff_ref_coef" );
    GLint shape_specular_color = glGetUniformLocation( program, "shape_specular_color" );
    GLint shape_spec_expo = glGetUniformLocation( program, "shape_spec_expo" );
    GLint shape_spec_ref_coef = glGetUniformLocation( program, "shape_spec_ref_coef" );

    // Set info for teapot
    if (obj == Teapot) {
        glUniform4f(shape_ambient, .5, .1, .9, 1.0);
        glUniform1f(shape_amb_ref_coef, .5);
        glUniform4f(shape_diffuse_color, .89, 0.0, 0.0, 1.0);
        glUniform1f(shape_diff_ref_coef, .7);
        glUniform4f(shape_specular_color, 1, 1, 1, 1);
        glUniform1f(shape_spec_expo, 10);
        glUniform1f(shape_spec_ref_coef, 1);
    // Set info for donut
    } else {
        glUniform4f(shape_ambient, 0.1, 0.85, 0.2, 1.0);
        glUniform1f(shape_amb_ref_coef, .5);
        glUniform4f(shape_diffuse_color, 0.1, 0.85, 0.2, 1.0);
        glUniform1f(shape_diff_ref_coef, .7);
        glUniform4f(shape_specular_color, 1, 1, 1, 1);
        glUniform1f(shape_spec_expo, 50);
        glUniform1f(shape_spec_ref_coef, 1);
    }

    // Set light attributes
    GLint light_color = glGetUniformLocation( program, "light_color" );
    GLint light_position = glGetUniformLocation( program, "light_position" );
    GLint amb_color = glGetUniformLocation( program, "amb_color" );
    glUniform4f(light_color, 1, 1, 0, 1);
    glUniform4f(light_position, 0, 5, 2, 1);
    glUniform4f(amb_color, .5, .5, .5, 1);
}
