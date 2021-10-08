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

//
// Polygon names
//

// Region 1
#define Quad1       0
#define Quad2       1

// Region 2
#define Quad3       2
#define Quad4       3
#define Quad5       4
#define Quad6       5

// Region 3
#define Pent1       6
#define Pent2       7

// Region 4
#define Hept        8

// Region 5
#define Fish        9

// Region 6
#define Star        10

// Region 7
#define Tri1        11
#define Tri2        12
#define Tri3        13
#define Tri4        14

#define N_POLYS         15

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

extern unsigned int which;      // bitmap for tests to be performed
extern int nv[N_POLYS];         // count of vertices in each clipped polygon
extern const int n_regions;     // number of clipping regions
extern const int n_verts[];     // original vertex count for each polygon

///
/// Draw all the clipping rectangles
///
/// @param C    The Canvas we'll be using
///
void makeClipOutlines( Canvas &C );

///
/// Create the polygon outlines
///
/// @param C    The Canvas we'll be using
///
void makePolygonOutlines( Canvas &C );

///
/// Create an object
///
/// @param num  Which object we are creating
/// @param C    The Canvas we'll be using
///
void createObject( int num, Canvas &C );

#endif
