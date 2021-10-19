//
//  Models.h
//
//  Assignment-specific code for objects.
//
//  Created by Warren R. Carithers on 2021/09/02.
//  Based on earlier code created by Joe Geigel and Warren R. Carithers
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
//

#ifndef MODELS_H_
#define MODELS_H_

#include <cstdlib>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

//
// GLEW and GLFW header files also pull in the OpenGL definitions
//
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Types.h"
#include "Buffers.h"
#include "Canvas.h"
#include "Pipeline.h"

//
// Number of different images
//
#define N_IMAGES    5

//
// Support for individual tests
//
#define N_TESTS     N_POLYS

#define TEST_ALL    0xffffffff

//
// Testing macros:
//      IFTEST(n) stmt
//      DOIF(n,stuff)
//
#define IFTEST(n)   if( which & (1 << (n)) )
#define DOIF(n,...) IFTEST(n) { __VA_ARGS__ }

//
// Global variables
//

// image to be displayed
extern int which;

///
/// Draw the objects
///
/// @param P   the Pipeline to use
///
void drawObjects( Pipeline &P );

///
/// Create our objects
///
/// @param C    The Pipeline we'll be using
///
void createObject( Pipeline &P );

#endif
