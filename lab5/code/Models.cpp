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
//  Contributor:  Owen Sullivan
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

/**
 * This function calculates the average of each point
 * in a vertex and returns the result
 */
Vertex averageVertex(Vertex p1, Vertex p2) {
    return {(p1.x + p2.x) / 2, (p1.y + p2.y) / 2, (p1.z + p2.z) / 2};
}

/**
 * This function recursivly divides and renders squares
 * it calculates the next points by averaging vertices
 */
static void createSquare(Canvas &C, Vertex ul, Vertex ur, Vertex lr, Vertex ll, int factor) {
    if (factor == 1) {
        C.addTriangle(ur, ll, lr);
        C.addTriangle(ul, ll, ur);
    } else {
        Vertex upper = averageVertex(ul, ur);
        Vertex lower = averageVertex(lr, ll);
        Vertex right = averageVertex(ur, lr);
        Vertex left = averageVertex(ul, ll);
        Vertex center = averageVertex(lr, ul);

        createSquare(C, ul, upper, center, left, factor / 2);
        createSquare(C, upper, ur, right, center, factor / 2);
        createSquare(C, left, center, lower, ll, factor / 2);
        createSquare(C, center, right, lr, lower, factor / 2);
    }
}


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

    Vertex ppp = { radius, radius, radius };
    Vertex ppn = { radius, radius, -radius };
    Vertex pnp = { radius, -radius, radius };
    Vertex pnn = { radius, -radius, -radius };
    Vertex npp = { -radius, radius, radius };
    Vertex npn = { -radius, radius, -radius };
    Vertex nnp = { -radius, -radius, radius };
    Vertex nnn = { -radius, -radius, -radius };

    createSquare(C, ppp, npp, npn, ppn, factor);
    createSquare(C, pnp, ppp, ppn, pnn, factor);
    createSquare(C, nnp, pnp, pnn, nnn, factor);
    createSquare(C, npp, nnp, nnn, npn, factor);
    createSquare(C, pnp, nnp, npp, ppp, factor);
    createSquare(C, ppn, npn, nnn, pnn, factor);
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

    // Get the degree differnce between each
    float deg = 360 / disc;
    Vertex v[height][disc];

    // removes portions of the radius to scale it correctly
    float current_radius = radius;
    float detractor = radius / height;

    // For each layer in the height
    for (int j = 0; j < height; j ++) {
        // Get the desired current height of the vertices
        float percentage_height = ((float) j)/height;
        float current_height = ((2 * radius) * percentage_height) - radius;

        // Calculate each point in the current layer
        for (int i = 0; i < disc; i ++ ) {
            Vertex curr = { current_radius * sin(deg * i * 0.0174533), current_radius * cos(deg * i * 0.0174533), current_height};
            // Save for later use
            v[j][i] = curr;
        }
        current_radius -= detractor;
    }

    // Draw base
    Vertex lower_origin = {0, 0,  -radius};
    for (int i = 0; i < disc; i++) {
        C.addTriangle(v[0][i], v[0][i - 1 == -1 ? disc - 1 : i - 1], lower_origin);
    }

    // Draw triangles between layers (other than last layer)
    for (int j = 0; j < height - 1; j++) {
        for (int i = 0; i < disc; i ++ ) {
            C.addTriangle(v[j][i - 1 == -1 ? disc - 1 : i - 1], v[j][i], v[j + 1][i]);
            C.addTriangle(v[j + 1][i], v[j + 1][i - 1 == -1 ? disc - 1 : i - 1], v[j][i - 1 == -1 ? disc - 1 : i - 1]);
        }
    }
    // Draw last layer to the tip of triangle
    Vertex upper_origin = {0, 0, radius};
    for (int i = 0; i < disc; i++) {
        C.addTriangle(upper_origin, v[height - 1][i - 1 == -1 ? disc - 1 : i - 1], v[height - 1][i]);
    }
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
