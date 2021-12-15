//
//  Materials.h
//
//  Simple class for setting up material property parameters.
//
//  Created by Warren R. Carithers 2021/11/11
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//  
//  This file should not be modified by students.
//

#ifndef MATERIALS_H_
#define MATERIALS_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>

#include "Models.h"

///
/// This function initializes all texture-related data structures for
/// the program.  This is where texture buffers should be created, where
/// texture images should be loaded, etc.
///
/// You will need to write this function, and maintain all of the values
/// needed to be sent to the various shaders.
///
void initTextures( void );

///
/// This function sets up the appearance parameters for the object.
///
/// @param program        The ID of an OpenGL (GLSL) shader program to which
///                       parameter values are to be sent
/// @param obj            The object type of the object being drawn
/// @param usingTextures  Are we texture-mapping this object?
///
void setMaterials( GLuint program, Object obj, bool usingTextures , int index);

#endif 
