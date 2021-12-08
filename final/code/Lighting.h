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
//  Students should not modify this file.
//

#ifndef LIGHTING_H_
#define LIGHTING_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Models.h"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

// lighting data that needs to be available when texture mapping
extern glm::vec4 lpDefault;
extern glm::vec4 lightpos;

///
/// This function sets up the lighting parameters for the shaders.
///
/// @param program  The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
///
void setLighting( GLuint program );

#endif
