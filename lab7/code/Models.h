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

#include "Buffers.h"
#include "Canvas.h"

//
// Object selection
//
typedef
    enum objects_e {
        Quad = 0, Cylinder, Discs
        // Sentinel gives us the number of shapes
        , N_OBJECTS
    } Object;

//
// PUBLIC GLOBALS
//

// object names
extern const char *objects[ N_OBJECTS ];

//
// PUBLIC FUNCTIONS
//

///
/// Create an object
///
/// @param C      the Canvas we'll be using
/// @param obj    which object to draw
/// @param buf    BufferSet to use for the object
///
void createObject( Canvas &C, Object obj, BufferSet &buf );

#endif
