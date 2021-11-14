//
//  Models.h
//
//  Assignment-specific code for objects.
//
//  Created by Warren R. Carithers on 2021/09/02.
//  Based on earlier code created by Joe Geigel and Warren R. Carithers
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#ifndef MODELS_H_
#define MODELS_H_

#include "Canvas.h"

//
// Object selection
//
typedef
    enum shape_e {
        Cube = 0, Cone
        // Sentinel gives us the number of shapes
        , N_SHAPES
    } Shape;

//
// PUBLIC GLOBALS
//

// object names
extern const char *objects[ N_SHAPES ];

//
// PUBLIC FUNCTIONS
//

///
/// Create an object
///
/// @param C      the Canvas we'll be using
/// @param shape  which shape to draw
/// @param rad    object radius
/// @param f1     primary tessellation factor
/// @param f2     secondary tessellation factor
///
void createObject( Canvas &C, Shape shape, float rad, int f1, int f2 );

#endif
