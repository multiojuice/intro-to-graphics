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

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/vec3.hpp>

#include "Models.h"

// data for the three objects
#include "CylinderData.h"
#include "BasketData.h"
#include "QuadData.h"
#include "TeapotData.h"
#include "Cube10.h"
#include "Sphere20.h"

using namespace std;

//
// PRIVATE GLOBALS
//
float PI  = 3.141592654;

//
// PUBLIC GLOBALS
//

// object names (must match the sequence in Models.h)
const char *objects[ N_OBJECTS ] = {
    "Quad", "Cylinder", "Discs", "Teapot", "Cube"
};

//
// PRIVATE FUNCTIONS
//

/**
 * Converts a vertex to coordinates for a cylinder texture
 * based on slides
 */
TexCoord convertVertexToTexCoordCyl(Vertex vertex) {
    float phi = atan(vertex.x / vertex.z);

    TexCoord tc = {(phi/(2 * PI)), (vertex.y + 1) / 2};
    return tc;
}

/**
 * Converts a vertex to coordinates for a disc texture
 * scale to .5, -.5 range
 */
TexCoord convertVertexToTexCoordDisc(Vertex vertex) {
    TexCoord tc = {vertex.x + .5, 1.5 - vertex.z};
    return tc;
}


///
/// makeQuad() - create a quad object
///
static void makeQuad( Canvas &C )
{
    for( int i = 0; i < quadElementsLength - 2; i += 3 ) {

        // Calculate the base indices of the three vertices
        int point1 = quadElements[i];
        int point2 = quadElements[i + 1];
        int point3 = quadElements[i + 2];

        Vertex p1 = quadVertices[point1];
        Vertex p2 = quadVertices[point2];
        Vertex p3 = quadVertices[point3];

        // Add this triangle to the collection
        C.addTriangleWithNorms( p1, quadNorm, p2, quadNorm, p3, quadNorm );

        // Add the texture coordinates
        C.addTextureCoords( quadUV[point1], quadUV[point2],
                            quadUV[point3] );
    }
}


Normal calculateNormal(Vertex p1, Vertex p2, Vertex p3) {
    glm::vec3 u(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
    glm::vec3 v(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);

    Normal norm;
    norm.x = (u.y * v.z) - (u.z * v.y);
    norm.y = (u.z * v.x) - (u.x * v.z);
    norm.z = (u.x * v.y) - (u.y * v.x);

    return norm;
}


///
/// makeQuad() - create a quad object
///
static void makeCube( Canvas &C )
{
    for( int i = 0; i < cubeElementsLength - 2; i += 3 ) {

        // Calculate the base indices of the three vertices
        int point1 = cubeElements[i];
        int point2 = cubeElements[i + 1];
        int point3 = cubeElements[i + 2];

        Vertex p1 = cubeVertices[point1];
        Vertex p2 = cubeVertices[point2];
        Vertex p3 = cubeVertices[point3];

        // Add this triangle to the collection
        Normal norm = calculateNormal(p1, p2, p3);

        C.addTriangleWithNorms( p1, norm, p2, norm, p3, norm );

        // // Add the texture coordinates
        // C.addTextureCoords( quadUV[point1], quadUV[point2],
        //                     quadUV[point3] );
    }
}

///
/// makeCylinder() - create the cylinder body
///
void makeCylinder( Canvas &C )
{
    // Only use the vertices for the body itself
    for( int i = body.first; i <= body.last - 2; i += 3 ) {

        // Calculate the base indices of the three vertices
        int point1 = cylinderElements[i];
        int point2 = cylinderElements[i + 1];
        int point3 = cylinderElements[i + 2];

        Vertex p1 = cylinderVertices[point1];
        Vertex p2 = cylinderVertices[point2];
        Vertex p3 = cylinderVertices[point3];

        // Calculate the normal vectors for each vertex
        Normal n1, n2, n3;

        // Normals on the body run from the axis to the vertex, and
        // are in the XZ plane; thus, for a vertex at (Px,Py,Pz), the
        // corresponding point on the axis is (0,Py,0), and the normal is
        // P - Axis, or just (Px,0,Pz).

        n1 = (Normal) { p1.x, 0.0f, p1.z };
        n2 = (Normal) { p2.x, 0.0f, p2.z };
        n3 = (Normal) { p3.x, 0.0f, p3.z };

        // Add this triangle to the collection
        C.addTriangleWithNorms( p1, n1, p2, n2, p3, n3 );
        C.addTextureCoords(convertVertexToTexCoordCyl(p1), convertVertexToTexCoordCyl(p2), convertVertexToTexCoordCyl(p3));
    }
}

///
/// makeDiscs() - create the cylinder discs
///
static void makeDiscs( Canvas &C )
{
    // Only use the vertices for the top and bottom discs

    for( int disc = 0; disc < 2; ++disc ) {

        // Select the starting and ending indices, and create the surface
        // normal for this disc.  For the top and bottom, the normals are
        // parallel to the Y axis.  Points on the disk all have Y == 0.5,
        // and those on the bottom have Y == -0.5.
        int first, last;
        Normal nn;

        if( disc == 0 ) { // bottom disc
            first = bdisc.first;
            last  = bdisc.last;
            nn = (Normal) { 0.0f, -1.0f, 0.0f };
        } else {
            first = tdisc.first;
            last  = tdisc.last;
            nn = (Normal) { 0.0f, 1.0f, 0.0f };
        }

        // Create the triangles
        for( int i = first; i <= last - 2; i += 3 ) {

            // Calculate the base indices of the three vertices
            int point1 = cylinderElements[i];
            int point2 = cylinderElements[i + 1];
            int point3 = cylinderElements[i + 2];

            Vertex p1 = cylinderVertices[point1];
            Vertex p2 = cylinderVertices[point2];
            Vertex p3 = cylinderVertices[point3];

            // Add this triangle to the collection
            C.addTriangleWithNorms( p1, nn, p2, nn, p3, nn );
            C.addTextureCoords(convertVertexToTexCoordDisc(p1), convertVertexToTexCoordDisc(p2), convertVertexToTexCoordDisc(p3));
        }
    }
}


static void createTeapot( Canvas &C )
{
    for( int i = 0; i < teapotElementsLen - 2; i += 3 ) {
        C.addTriangleWithNorms(
          teapotVerts[ teapotElements[i] ],   teapotNorms[ teapotNormIx[i] ],
          teapotVerts[ teapotElements[i+1] ], teapotNorms[ teapotNormIx[i+1] ],
          teapotVerts[ teapotElements[i+2] ], teapotNorms[ teapotNormIx[i+2] ]
        );

    }
}

static void makeBasket( Canvas &C )
{
    for( int i = 0; i < basketElementsLen - 2; i += 3 ) {
        Vertex p1 = basketVerts[ basketElements[i] ];
        Vertex p2 = basketVerts[ basketElements[i+1] ];
        Vertex p3 = basketVerts[ basketElements[i+2] ];

        Normal norm = calculateNormal(p1, p2, p3);
        C.addTriangleWithNorms(
          p1,   norm,
          p2, norm,
          p3, norm 
        );

    }
}

static void makeSphere( Canvas &C )
{
    for( int i = 0; i < sphereElementsLength - 2; i += 3 ) {
        Vertex p1 = sphereVertices[ sphereElements[i] ];
        Vertex p2 = sphereVertices[ sphereElements[i+1] ];
        Vertex p3 = sphereVertices[ sphereElements[i+2] ];
        Normal n1 = {p1.x, p1.y, p1.z};
        Normal n2 = {p2.x, p2.y, p2.z};
        Normal n3 = {p3.x, p3.y, p3.z};
        Normal norm = calculateNormal(p1, p2, p3);
        C.addTriangleWithNorms(
          p1, n1  ,
          p2, n2,
          p3, n3 
        );
    }
}


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
void createObject( Canvas &C, Object obj, BufferSet &buf )
{
    // start with a fresh Canvas
    C.clear();

    // create the specified object
    switch( obj ) {
    case Quad:      makeQuad( C );      break;
    case Cylinder:  makeCylinder( C );  break;
    case Discs:     makeDiscs( C );     break;
    case Teapot:    createTeapot( C );     break;
    case Cube:      makeCube( C ); break;
    case Basket:      makeBasket( C ); break;
    case Sphere:      makeSphere( C ); break;
    }

    // create the buffers for the object
    buf.createBuffers( C );
}