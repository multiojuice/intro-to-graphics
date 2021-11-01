//
//  Transforms.h
//
//  Simple module for setting up the model and camera transformations
//  for the Transformation Assignment.
//
//  Created by Warren R. Carithers 2016/11/11.
//  Updates: 2019/04/04, 2019/10/12, 2021/10/23 by wrc.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2021 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#ifndef TRANSFORMS_H_
#define TRANSFORMS_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//
// Control parameters
//

// projection type (viewing mode)
typedef
    enum vmode_e {
        Frustum, Ortho
    } ViewMode;

// transformations
typedef
    enum xmode_e {
        Off, On
    } XformMode;

// camera position
typedef
    enum cmode_e {
        Camera1, Camera2
    } CamMode;

///
/// This function sets up the view and projection parameters for the
/// desired projection of the scene. See the assignment description for
/// the values for the projection parameters.
///
/// You will need to write this function, and maintain all of the values
/// needed to be sent to the vertex shader.
///
/// @param program - The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
/// @param mode    - The desired projection mode (Frustum or Ortho)
///
void setProjection( GLuint program, ViewMode mode );

///
/// This function sets up the transformation parameters for the vertices
/// of the teapot.  The order of application is specified in the driver
/// program.
///
/// You will need to write this function, and maintain all of the values
/// which must be sent to the vertex shader.
///
/// @param program - The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
/// @param mode    - Which set of transformations to apply
///
void setTransforms( GLuint program, XformMode mode );

///
/// This function sets up the camera parameters controlling the viewing
/// transformation.
///
/// You will need to write this function, and maintain all of the values
/// which must be sent to the vertex shader.
///
/// @param program - The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
/// @param mode    - The desired camera position
///
void setCamera( GLuint program, CamMode mode );

#endif
