//
//  Lighting.h
//
//  Simple module for setting up the parameters for lighting and shading
//  for the Shading Assignment.
//
//  Created by Warren R. Carithers 2019/11/18.
//  Updates: 2021/11/07 by wrc
//  Copyright 2021 Rochester Institute of Technology.  All rights reserved.
//
//  Students should not change this file.
//

#ifndef LIGHTING_H_
#define LIGHTING_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Models.h"

///
/// This function sets up the lighting, material, and shading parameters
/// for the Phong shader.
///
/// You will need to write this function, and maintain all of the values
/// needed to be sent to the vertex shader.
///
/// @param program  The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
/// @param object   which object is currently being drawn
///
void setLighting( GLuint program, Shape object );

#endif
