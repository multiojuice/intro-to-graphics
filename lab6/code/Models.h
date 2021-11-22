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

#include "Canvas.h"

//
// Object selection
//
typedef
    enum shape_e {
        Torus = 0, Teapot
        // Sentinel gives us the number of shapes
        , N_SHAPES
    } Shape;

typedef
    enum shading_e {
        Gouraud = 0, Phong
        // sentinel
        , N_SHADERS
    } Shader;

//
// PUBLIC GLOBALS
//

// object names
extern const char *objects[ N_SHAPES ];

// shader names
extern const char *shaders[ N_SHADERS ];

//
// PUBLIC FUNCTIONS
//

///
/// Create an object
///
/// @param C        the Canvas we'll be using
/// @param shape    which shape to draw
///
void createObject( Canvas &C, Shape shape );

#endif
