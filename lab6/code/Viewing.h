//
//  Viewing.h
//
//  Simple module for setting up the projection and camera transformations
//  for the Shading Assignment.
//
//  Created by Warren R. Carithers 2019/11/18.
//  Updates: 2021/11/07 by wrc
//  Copyright 2021 Rochester Institute of Technology.  All rights reserved.
//
//  This file should not be modified by students.
//

#ifndef VIEWING_H_
#define VIEWING_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/vec3.hpp>

///
/// This function sets up a frustum projection of the scene.
///
/// @param program   The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
///
void setProjection( GLuint program );

///
/// This function sets up the transformation parameters for the vertices
/// of the object.  The order of application is fixed: scaling, Z rotation,
/// Y rotation, X rotation, and then translation.
///
/// @param program   The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
/// @param scale     Scale factors for each axis
/// @param rotate    Rotation angles around the three axes, in degrees
/// @param xlate     Amount of translation along each axis
///
void setTransforms( GLuint program, glm::vec3 scale,
                    glm::vec3 rotate, glm::vec3 xlate );

///
/// This function sets up the camera parameters controlling the viewing
/// transformation.
///
/// @param program   The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
///
void setCamera( GLuint program );

#endif
