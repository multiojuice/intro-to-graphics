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
//  This file should not be modified by students.
//

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
#include "Utils.h"
#include "ShaderSetup.h"
#include "Buffers.h"
#include "Models.h"
#include "Canvas.h"
#include "Clipper.h"

using namespace std;

//
// PRIVATE GLOBALS
//

//
// Object information
//

// our clipping regions (LL and UR corners)

static Vertex regions[][2] = {
    { {  20, 120 }, {  70, 170 } },
    { {  30,  30 }, {  70,  80 } },
    { { 100,  34 }, { 130,  60 } },
    { {  90,  90 }, { 130, 120 } },
    { { 198, 198 }, { 276, 258 } },
    { { 221,  80 }, { 251, 101 } },
    { {  30, 200 }, {  90, 260 } }
};

//
// our polygons (list of vertices)
//

// quads, clipping region 1
// shades of red

static Vertex quad1[] = {  // entirely inside the region
    {  35, 135 }, {  35, 155 }, {  55, 155 }, {  55, 135 }
};
// static int quad1_nv = sizeof(quad1) / sizeof(Vertex);
static Color quad1c = { 1.00f, 0.00f, 0.00f, 1.00f };  // 0xFF0000

static Vertex quad2[] = {  // entirely outside the region
    { 130, 140 }, { 130, 170 }, {  90, 170 }, {  90, 140 }
};
// static int quad2_nv = sizeof(quad2) / sizeof(Vertex);
static Color quad2c = { 1.00f, 0.40f, 0.40f, 1.00f };  // 0xFF6666

// quads, clipping region 2
// shades of blue

static Vertex quad3[] = {  // two vertices outside w/r/t the right edge
    {  64,  47 }, {  80,  47 }, {  80,  71 }, {  64,  71 }
};
// static int quad3_nv = sizeof(quad3) / sizeof(Vertex);
static Color quad3c = { 0.00f, 0.45f, 0.60f, 1.00f };  // 0x0073FF

static Vertex quad4[] = {  // two vertices outside w/r/t the left edge
    {  20,  60 }, {  50,  60 }, {  50,  50 }, {  20,  50 }
};
// static int quad4_nv = sizeof(quad4) / sizeof(Vertex);
static Color quad4c = { 0.00f, 0.60f, 0.80f, 1.00f };  // 0x0099CC

static Vertex quad5[] = {  // two vertices outside w/r/t the top edge
    {  50,  70 }, {  60,  70 }, {  60, 100 }, {  50, 100 }
};
// static int quad5_nv = sizeof(quad5) / sizeof(Vertex);
static Color quad5c = { 0.00f, 0.75f, 1.00f, 1.00f };  // 0x00BFFF

static Vertex quad6[] = {  // two vertices outside w/r/t the bottom edge
    {  40,  40 }, {  60,  40 }, {  60,  20 }, {  40,  20 }
};
// static int quad6_nv = sizeof(quad6) / sizeof(Vertex);
static Color quad6c = { 0.40f, 0.85f, 1.00f, 1.00f };  // 0x66D9FF

static Vertex pent1[] = {
    {  90,  20 }, { 100,  10 }, { 120,  20 }, { 110,  50 }, {  90,  40 }
};
// static int pent1_nv = sizeof(pent1) / sizeof(Vertex);
static Color pent1c = { 0.72f, 0.44f, 0.86f, 1.00f };  // 0xB86FDC

static Vertex pent2[] = {
    { 120,  40 }, { 160,  60 }, { 180,  50 }, { 170,  80 }, { 125,  65 }
};
// static int pent2_nv = sizeof(pent2) / sizeof(Vertex);
static Color pent2c = { 0.84f, 0.68f, 0.92f, 1.00f };  // 0xD6ADEB

static Vertex hept[] = {
    { 120,  80 }, { 140,  80 }, { 160,  90 }, { 160, 110 },
    { 140, 120 }, { 120, 110 }, { 110, 100 }
};
// static int hept_nv = sizeof(hept) / sizeof(Vertex);
static Color heptc = { 0.70f, 0.70f, 0.70f, 1.00f };  // 0xB3B3B3

static Vertex fish[] = {
    { 190,  56 }, { 230,  68 }, { 247,  56 }, { 269,  71 }, { 284, 104 },
    { 251, 122 }, { 233, 110 }, { 212, 119 }, { 203,  95 }
};
// static int fish_nv = sizeof(fish) / sizeof(Vertex);
static Color fishc = { 0.87f, 0.72f, 0.53f, 1.00f };  // 0xDEB887

static Vertex star[] = {
    { 177, 156 }, { 222, 188 }, { 267, 156 }, { 250, 207 }, { 294, 240 },
    { 240, 240 }, { 222, 294 }, { 204, 240 }, { 150, 240 }, { 194, 207 }
};
// static int star_nv = sizeof(star) / sizeof(Vertex);
static Color starc = { 1.00f, 0.65f, 0.00f, 1.00f };  // 0xFFA500

// triangles, clipping region 7
// shades of green

static Vertex tri1[] = {
    {  60, 180 }, {  70, 210 }, {  50, 210 }
};
// static int tri1_nv = sizeof(tri1) / sizeof(Vertex);
static Color tri1c = { 0.00f, 0.60f, 0.00f, 1.00f };  // 0x009900

static Vertex tri2[] = {
    {  80, 220 }, { 110, 230 }, {  80, 240 }
};
// static int tri2_nv = sizeof(tri2) / sizeof(Vertex);
static Color tri2c = { 0.00f, 0.80f, 0.00f, 1.00f };  // 0x00CC00

static Vertex tri3[] = {
    {  50, 250 }, {  70, 250 }, {  60, 280 }
};
// static int tri3_nv = sizeof(tri3) / sizeof(Vertex);
static Color tri3c = { 0.00f, 1.00f, 0.00f, 1.00f };  // 0x00FF00

static Vertex tri4[] = {
    {  10, 230 }, {  40, 220 }, {  40, 240 }
};
// static int tri4_nv = sizeof(tri4) / sizeof(Vertex);
static Color tri4c = { 0.40f, 1.00f, 0.40f, 1.00f };  // 0x66FF66

//
// PUBLIC GLOBALS
//

// bitmap for tests to be performed
unsigned int which;

// count of vertices in each clipped polygon
int nv[N_POLYS];

// number of clipping regions
const int n_regions = sizeof(regions) / (2 * sizeof(Vertex));

// original vertex count for each polygon
const int n_verts[N_POLYS] = {
    sizeof(quad1) / sizeof(Vertex),
    sizeof(quad2) / sizeof(Vertex),
    sizeof(quad3) / sizeof(Vertex),
    sizeof(quad4) / sizeof(Vertex),
    sizeof(quad5) / sizeof(Vertex),
    sizeof(quad6) / sizeof(Vertex),
    sizeof(pent1) / sizeof(Vertex),
    sizeof(pent2) / sizeof(Vertex),
    sizeof(hept)  / sizeof(Vertex),
    sizeof(fish)  / sizeof(Vertex),
    sizeof(star)  / sizeof(Vertex),
    sizeof(tri1)  / sizeof(Vertex),
    sizeof(tri2)  / sizeof(Vertex),
    sizeof(tri3)  / sizeof(Vertex),
    sizeof(tri4)  / sizeof(Vertex)
};

// aliases for the vertex counts
#define quad1_nv n_verts[Quad1]
#define quad2_nv n_verts[Quad2]
#define quad3_nv n_verts[Quad3]
#define quad4_nv n_verts[Quad4]
#define quad5_nv n_verts[Quad5]
#define quad6_nv n_verts[Quad6]
#define pent1_nv n_verts[Pent1]
#define pent2_nv n_verts[Pent2]
#define hept_nv  n_verts[Hept]
#define fish_nv  n_verts[Fish]
#define star_nv  n_verts[Star]
#define tri1_nv  n_verts[Tri1]
#define tri2_nv  n_verts[Tri2]
#define tri3_nv  n_verts[Tri3]
#define tri4_nv  n_verts[Tri4]

//
// PRIVATE FUNCTIONS
//

///
/// Support function that draws clip regions as line loops
///
static void drawClipRegion( Vertex ll, Vertex ur, Canvas &C ) {
    C.addPixel( (Vertex) { ll.x, ll.y } );  // LL
    C.addPixel( (Vertex) { ur.x, ll.y } );  // LR
    C.addPixel( (Vertex) { ur.x, ur.y } );  // UR
    C.addPixel( (Vertex) { ll.x, ur.y } );  // UL
}

///
/// Draw all the clipping rectangles
///
void makeClipOutlines( Canvas &C ) {

    // we draw the clipping regions as rectangles.
    // all vertices are put into one vertex buffer, and
    // we use glDrawArrays() instead of glDrawElements()
    // to draw them as line loops

    for( int i = 0; i < n_regions; ++i ) {
        drawClipRegion( regions[i][0], regions[i][1], C );
    }
}

///
/// Draw a polygon
///
static void drawPolygon( Vertex v[], int nv, Canvas &C ) {

    // just put the vertices into the vertex buffer, in order

    for( int i = 0; i < nv; ++i ) {
        C.addPixel( v[i] );
    }
}

///
/// Create the polygon outlines
///
void makePolygonOutlines( Canvas &C ) {

    // here, we draw the original polygons; these
    // will be rendered using line loops

    // region 1
    DOIF( Quad1,
        C.setColor( quad1c );
        drawPolygon( quad1, quad1_nv, C );
    )
    DOIF( Quad2,
        C.setColor( quad2c );
        drawPolygon( quad2, quad2_nv, C );
    )

    // region 2
    DOIF( Quad3,
        C.setColor( quad3c );
        drawPolygon( quad3, quad3_nv, C );
    )
    DOIF( Quad4,
        C.setColor( quad4c );
        drawPolygon( quad4, quad4_nv, C );
    )
    DOIF( Quad5,
        C.setColor( quad5c );
        drawPolygon( quad5, quad5_nv, C );
    )
    DOIF( Quad6,
        C.setColor( quad6c );
        drawPolygon( quad6, quad6_nv, C );
    )

    // region 3
    DOIF( Pent1,
        C.setColor( pent1c );
        drawPolygon( pent1, pent1_nv, C );
    )
    DOIF( Pent2,
        C.setColor( pent2c );
        drawPolygon( pent2, pent2_nv, C );
    )

    // region 4
    DOIF( Hept,
        C.setColor( heptc );
        drawPolygon( hept, hept_nv, C );
    )

    // region 5
    DOIF( Fish,
        C.setColor( fishc );
        drawPolygon( fish, fish_nv, C );
    )

    // region 6
    DOIF( Star,
        C.setColor( starc );
        drawPolygon( star, star_nv, C );
    )

    // region 7
    DOIF( Tri1,
        C.setColor( tri1c );
        drawPolygon( tri1, tri1_nv, C );
    )
    DOIF( Tri2,
        C.setColor( tri2c );
        drawPolygon( tri2, tri2_nv, C );
    )
    DOIF( Tri3,
        C.setColor( tri3c );
        drawPolygon( tri3, tri3_nv, C );
    )
    DOIF( Tri4,
        C.setColor( tri4c );
        drawPolygon( tri4, tri4_nv, C );
    )
}

///
/// Create a polygon
///
/// @param num  Which object we are creating
/// @param C    The Canvas we'll be using
///
void createObject( int num, Canvas &C ) {
    // temporary vertex array
    Vertex tmp[50];

    ///
    // region one:  quads, shades of red
    ///

    switch( num ) {
    case Quad1:
        // entirely inside
        C.setColor( quad1c );
        nv[Quad1] = clipPolygon( quad1_nv, quad1, tmp,
                             regions[0][0], regions[0][1] );
        if( nv[Quad1] > 0 ) {
            drawPolygon( tmp, nv[Quad1], C );
        }
        break;

    case Quad2:
        // entirely outside
        C.setColor( quad2c );
        nv[Quad2] = clipPolygon( quad2_nv, quad2, tmp,
                             regions[0][0], regions[0][1] );
        // shouldn't draw anything!
        if( nv[Quad2] > 0 ) {
            drawPolygon( tmp, nv[Quad2], C );
        }
        break;

    ///
    // region two:  more quads, shades of blue
    ///

    case Quad3:
        // two vertices outside w/r/t the right edge
        C.setColor( quad3c );
        nv[Quad3] = clipPolygon( quad3_nv, quad3, tmp,
                             regions[1][0], regions[1][1] );
        if( nv[Quad3] > 0 ) {
            drawPolygon( tmp, nv[Quad3], C );
        }
        break;

    case Quad4:
        // two vertices outside w/r/t the left edge
        C.setColor( quad4c );
        nv[Quad4] = clipPolygon( quad4_nv, quad4, tmp,
                             regions[1][0], regions[1][1] );
        if( nv[Quad4] > 0 ) {
            drawPolygon( tmp, nv[Quad4], C );
        }
        break;

    case Quad5:
        // two vertices outside w/r/t the top edge
        C.setColor( quad5c );
        nv[Quad5] = clipPolygon( quad5_nv, quad5, tmp,
                             regions[1][0], regions[1][1] );
        if( nv[Quad5] > 0 ) {
            drawPolygon( tmp, nv[Quad5], C );
        }
        break;

    case Quad6:
        // two vertices outside w/r/t the bottom edge
        C.setColor( quad6c );
        nv[Quad6] = clipPolygon( quad6_nv, quad6, tmp,
                             regions[1][0], regions[1][1] );
        if( nv[Quad6] > 0 ) {
            drawPolygon( tmp, nv[Quad6], C );
        }
        break;

    ///
    // region three: pentagons, outside w/r/t two edges
    ///

    case Pent1:
        // outside w/r/t the left and bottom edges
        C.setColor( pent1c );
        nv[Pent1] = clipPolygon( pent1_nv, pent1, tmp,
                             regions[2][0], regions[2][1] );
        if( nv[Pent1] > 0 ) {
            drawPolygon( tmp, nv[Pent1], C );
        }
        break;

    case Pent2:
        // outside w/r/t the top and right edges
        C.setColor( pent2c );
        nv[Pent2] = clipPolygon( pent2_nv, pent2, tmp,
                             regions[2][0], regions[2][1] );
        if( nv[Pent2] > 0 ) {
            drawPolygon( tmp, nv[Pent2], C );
        }
        break;

    ///
    // region four:  outside on top, right, and bottom
    ///

    case Hept:
        C.setColor( heptc );
        nv[Hept] = clipPolygon( hept_nv, hept, tmp,
                             regions[3][0], regions[3][1] );
        if( nv[Hept] > 0 ) {
            drawPolygon( tmp, nv[Hept], C );
        }
        break;

    ///
    // region five:  surrounds the clip region
    ///

    case Fish:
        C.setColor( fishc );
        nv[Fish] = clipPolygon( fish_nv, fish, tmp,
                             regions[5][0], regions[5][1] );
        if( nv[Fish] > 0 ) {
            drawPolygon( tmp, nv[Fish], C );
        }
        break;

    ///
    // region six:  outside on all four edges
    ///

    case Star:
        C.setColor( starc );
        nv[Star] = clipPolygon( star_nv, star, tmp,
                             regions[4][0], regions[4][1] );
        if( nv[Star] > 0 ) {
            drawPolygon( tmp, nv[Star], C );
        }
        break;

    ///
    // region seven: outside w/r/t one edge
    ///

    case Tri1:
        C.setColor( tri1c );
        nv[Tri1] = clipPolygon( tri1_nv, tri1, tmp,
                             regions[6][0], regions[6][1] );
        if( nv[Tri1] > 0 ) {
            drawPolygon( tmp, nv[Tri1], C );
        }
        break;

    case Tri2:
        C.setColor( tri2c );
        nv[Tri2] = clipPolygon( tri2_nv, tri2, tmp,
                             regions[6][0], regions[6][1] );
        if( nv[Tri2] > 0 ) {
            drawPolygon( tmp, nv[Tri2], C );
        }
        break;

    case Tri3:
        C.setColor( tri3c );
        nv[Tri3] = clipPolygon( tri3_nv, tri3, tmp,
                             regions[6][0], regions[6][1] );
        if( nv[Tri3] > 0 ) {
            drawPolygon( tmp, nv[Tri3], C );
        }
        break;

    case Tri4:
        C.setColor( tri4c );
        nv[Tri4] = clipPolygon( tri4_nv, tri4, tmp,
                             regions[6][0], regions[6][1] );
        if( nv[Tri4] > 0 ) {
            drawPolygon( tmp, nv[Tri4], C );
        }
        break;

    default:
        cerr << "bad polygon number '" << num << "', skipping" << endl;
    }
}
