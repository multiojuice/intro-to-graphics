//
//  Models.cpp
//
//  Assignment-specific code for objects.  This module will typically be
//  heavily modified to fit the needs of each assignment.
//
//  Created by Warren R. Carithers on 2021/09/02.
//  Based on earlier versions created by Joe Geigel and Warren R. Carithers
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#include <iostream>
#include <cmath>

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include "Models.h"

//
// PUBLIC GLOBALS
//

// object names (must match the sequence in Models.h)
const char *objects[ N_SHAPES ] = {
    "Cube", "Cone"
};

//
// PRIVATE FUNCTIONS
//

///
/// createCube - Create a unit cube, centered at the origin, with a given
/// number of subdivisions in each direction on each face.
///
/// @param C - the Canvas to use when drawing
/// @param radius - distance of each face from the origin
/// @param factor - number of equal subdivisons to be made in each 
///        direction along each face
///
/// Can only use calls to C.addTriangle()
///
static void createCube( Canvas &C, float radius, int factor )
{
    if( factor < 1 )
        factor = 1;

    // YOUR IMPLEMENTATION HERE
}

///
/// createCone - Create polygons for a cone with unit height, centered at
/// the origin, with separate number of radial subdivisions and height 
/// subdivisions.
///
/// @param C - the Canvas to use when drawing
/// @param radius - Radius of the base of the cone
/// @param disc - number of subdivisions on the radial base
/// @param height - number of subdivisions along the height
///
/// Can only use calls to C.addTriangle()
///
static void createCone( Canvas &C, float radius, int disc, int height )
{
    if( disc < 3 )
        disc = 3;

    if( height < 1 )
        height = 1;

    // YOUR IMPLEMENTATION HERE
}

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
void createObject( Canvas &C, Shape shape, float rad, int f1, int f2 )
{
    switch( shape ) {
    default:
        cerr << "Unknown current shape '" << shape
             << "', drawing cube" << endl;
        // FALL THROUGH
    case Cube:
        createCube( C, rad, f1 );
        break;
    case Cone:
        createCone( C, rad, f1, f2 );
        break;
    }
}
