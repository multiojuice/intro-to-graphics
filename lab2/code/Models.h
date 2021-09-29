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
#include "Rasterizer.h"

//
// Enumerated type with polygon names
//
// Caution:  enum members must have sequential values
// beginning at 0 for N_POLYS to be correct!
//

enum Polygons {
    // Teapot
    Base = 0, Body1, Body2, Spout1, Spout2, Handle, Lid,
    // Basic shapes
    Triangle, Quad, Star1, Star2,
    // Corner squares
    Square_ll, Square_ul, Square_ur, Square_lr,
    // Quads for bottom and left sides
    Side1, Side2,
    // Triangles for top and right sides
    Tri_u1, Tri_u2, Tri_r1, Tri_r2,
    // Sentinel gives us polygon count
    N_POLYS
};

///
/// Create an object
///
/// @param num  Which object we are creating
/// @param R    The Rasterizer we'll be using
/// @param B    The BufferSet for the created object
///
void createObject( int num, Rasterizer &R, BufferSet &B );

#endif
