///
//  Transforms
//
//  Simple module for setting up the model and camera transformations
//  for the Transformation Assignment.
//
//  Created by Warren R. Carithers 2016/11/11.
//  Updates: 2019/04/04, 2019/10/12, 2021/10/23 by wrc.
//  Based on code created by Joe Geigel on 1/23/13.
//  Copyright 2021 Rochester Institute of Technology.  All rights reserved.
//
//  Contributor:  Owen Sullivan
///

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <iomanip>

#include "Transforms.h"

using namespace std;

// camera position 1
static GLfloat cam1_eye[] =    { 0.0f, 0.0f,  0.0f };
static GLfloat cam1_lookat[] = { 0.0f, 0.0f, -1.0f };
static GLfloat cam1_up[] =     { 0.0f, 1.0f,  0.0f };

// camera position 2
static GLfloat cam2_eye[] =    { 0.0f,  1.3f, -0.5f };
static GLfloat cam2_lookat[] = { 0.0f, -0.4f, -1.0f };
static GLfloat cam2_up[] =     { 0.0f,  1.0f,  0.0f };

// default transformations
static GLfloat std_scale[] =  { 1.0f, 1.0f, 1.0f };
static GLfloat std_rotate[] = { 0.0f, 0.0f, 0.0f };
static GLfloat std_xlate[] =  { 0.0f, 0.0f, 0.0f };

// alternate transformation values
static GLfloat alt_scale[] =  {  1.0f,   2.0f,   1.0f };
static GLfloat alt_rotate[] = {  0.0f, 330.0f, 305.0f };
static GLfloat alt_xlate[] =  { -0.2f,   0.2f,   0.0f };

// clipping boundaries: left, right, top, bottom, near, far
static GLfloat bounds[] = { -1.0f, 1.0f, 1.0f, -1.0f, 0.9f, 4.5f };

///
/// This function sets up the view and projection parameters for the
/// desired projection of the scene.
///
/// You will need to write this function, and maintain all of the values
/// needed to be sent to the vertex shader.
///
/// @param program - The ID of an OpenGL (GLSL) shader program to which
///    parameter values are to be sent
/// @param mode    - The desired projection mode (Frustum or Ortho)
///
void setProjection( GLuint program, ViewMode mode )
{
        GLint leftLoc = glGetUniformLocation(program, "left");
        glUniform1f(leftLoc, bounds[0]);
        GLint rightLoc = glGetUniformLocation(program, "right");
        glUniform1f(rightLoc, bounds[1]);
        GLint topLoc = glGetUniformLocation(program, "top");
        glUniform1f(topLoc, bounds[2]);
        GLint bottomLoc = glGetUniformLocation(program, "bottom");
        glUniform1f(bottomLoc, bounds[3]);
        GLint nearLoc = glGetUniformLocation(program, "near");
        glUniform1f(nearLoc, bounds[4]);
        GLint farLoc = glGetUniformLocation(program, "far");
        glUniform1f(farLoc, bounds[5]);

        GLint viewmodeLoc = glGetUniformLocation(program, "viewmode");
        glUniform1f(viewmodeLoc, mode == Frustum ? 1 : 0);
}

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
void setTransforms( GLuint program, XformMode mode )
{
    GLint scaleLoc = glGetUniformLocation( program, "scale" );
    GLint translateLoc = glGetUniformLocation( program, "translate" );
    GLint rotateLoc = glGetUniformLocation( program, "rotate" );

    if (mode == Off) {
        glUniform3f(scaleLoc, std_scale[0], std_scale[1], std_scale[2]);
        glUniform3f(translateLoc, std_xlate[0], std_xlate[1], std_xlate[2]);
        glUniform3f(rotateLoc, std_rotate[0], std_rotate[1], std_rotate[2]);
    } else {
        glUniform3f(scaleLoc, alt_scale[0], alt_scale[1], alt_scale[2]);
        glUniform3f(translateLoc, alt_xlate[0], alt_xlate[1], alt_xlate[2]);
        glUniform3f(rotateLoc, alt_rotate[0], alt_rotate[1], alt_rotate[2]);
    }

}

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
void setCamera( GLuint program, CamMode mode )
{
    GLint eyeLoc = glGetUniformLocation( program, "eye" );
    GLint lookatLoc = glGetUniformLocation( program, "lookat" );
    GLint upLoc = glGetUniformLocation( program, "up" );

    if (mode == Camera1) {
        glUniform3f(eyeLoc, cam1_eye[0], cam1_eye[1], cam1_eye[2]);
        glUniform3f(lookatLoc, cam1_lookat[0], cam1_lookat[1], cam1_lookat[2]);
        glUniform3f(upLoc, cam1_up[0], cam1_up[1], cam1_up[2]);
    } else {
        glUniform3f(eyeLoc, cam2_eye[0], cam2_eye[1], cam2_eye[2]);
        glUniform3f(lookatLoc, cam2_lookat[0], cam2_lookat[1], cam2_lookat[2]);
        glUniform3f(upLoc, cam2_up[0], cam2_up[1], cam2_up[2]);
    }
}
