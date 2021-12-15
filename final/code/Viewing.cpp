//
//  Viewing
//
//  Simple module for setting up the projection and camera transformations
//  for the Shading Assignment.
//
//  Created by Warren R. Carithers 2019/11/14.
//  Updates:  2021/11/07 by wrc
//  Copyright 2021 Rochester Institute of Technology.  All rights reserved.
//
//

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// need memcpy()
#include <cstring>

#include "Viewing.h"
#include "Utils.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>
#include <glm/geometric.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace std;

/// standard camera information
/// Edited to give a zoomed in camera shot effect
static glm::vec3 eye( 0.2f, 3.0f, 5.5f );
static glm::vec3 lookat( 0.0f, 1.0f, 0.0f );
static glm::vec3 up( 0.0f, 1.0f, 0.0f );

/// clipping boundaries: left, right, top, bottom, near, far
static GLfloat bounds[] = { -1.0f, 1.0f, 1.0f, -1.0f, 3.0f, 100.5f };

#define LEFT    bounds[0]
#define RIGHT   bounds[1]
#define TOP     bounds[2]
#define BOTTOM  bounds[3]
#define NEAR    bounds[4]
#define FAR     bounds[5]

///
/// This function sets up a frustum projection of the scene.
///
/// @param program - The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
///
void setProjection( GLuint program )
{
    glm::mat4 pmat = glm::frustum( LEFT, RIGHT, BOTTOM, TOP, NEAR, FAR );

    GLint loc = getUniformLoc( program, "projMat" );
    if( loc >= 0 ) {
        glUniformMatrix4fv( loc, 1, GL_FALSE, glm::value_ptr(pmat) );
    }
}

///
/// This function sets up the transformation parameters for the vertices
/// of the object.  The order of application is fixed: scaling, Z rotation,
/// Y rotation, X rotation, and then translation.
///
/// @param program - The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
/// @param scale  - scale factors for each axis
/// @param rotate - rotation angles around the three axes, in degrees
/// @param xlate  - amount of translation along each axis
///
void setTransforms( GLuint program, glm::vec3 scale,
                    glm::vec3 rotate, glm::vec3 xlate )
{
    // need an identity matrix
    glm::mat4 id(1.0f);

    // create translation and scale matrices
    glm::mat4 tMat = glm::translate( id, xlate );
    glm::mat4 sMat = glm::scale( id, scale );

    // create the three rotation matrices
    glm::vec3 rads( glm::radians( rotate ) );

    glm::mat4 xMat = glm::rotate( id, rads.x, glm::vec3(1.0f,0.0f,0.0f) );
    glm::mat4 yMat = glm::rotate( id, rads.y, glm::vec3(0.0f,1.0f,0.0f) );
    glm::mat4 zMat = glm::rotate( id, rads.z, glm::vec3(0.0f,0.0f,1.0f) );

    // combine the transformations
    glm::mat4 cm = tMat * xMat * yMat * zMat * sMat;

    GLint loc = getUniformLoc( program, "modelMat" );
    if( loc >= 0 ) {
        glUniformMatrix4fv( loc, 1, GL_FALSE, glm::value_ptr(cm) );
    }
}

///
/// This function sets up the camera parameters controlling the viewing
/// transformation.
///
/// @param program - The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
///
void setCamera( GLuint program )
{
    // calculate our camera matrix
    glm::mat4 vMat = glm::lookAt( eye, lookat, up );

    // copy it down to the shader program
    GLint loc = getUniformLoc( program, "viewMat" );
    if( loc >= 0 ) {
        glUniformMatrix4fv( loc, 1, GL_FALSE, glm::value_ptr(vMat) );
    }
}
