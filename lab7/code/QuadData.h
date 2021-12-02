//
//  QuadData.h
//
//  A quad with diameter 1 centered at the origin and coincident with
//  the XY plane.  It was created with a tessellation factor of four.
//
//  Created by Warren R. Carithers on 2020/11/10.
//  Updated:  2021/11/11 by wrc
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//
//  NOTE: OpenGL (etc.) headers must be included before this file, as
//  this file does not include any GL-related header files.
//

#ifndef QUADDATA_H_
#define QUADDATA_H_

#include "Types.h"

//
// Vertices defining the quad.  Each is represented once in this array.
//
static Vertex quadVertices[] = {
 // top row
 {-0.50f,  0.50f}, {-0.25f,  0.50f}, { 0.00f,  0.50f},
 { 0.25f,  0.50f}, { 0.50f,  0.50f},
 // second row
 {-0.50f,  0.25f}, {-0.25f,  0.25f}, { 0.00f,  0.25f},
 { 0.25f,  0.25f}, { 0.50f,  0.25f},
 // third (middle) row
 {-0.50f,  0.00f}, {-0.25f,  0.00f}, { 0.00f,  0.00f},
 { 0.25f,  0.00f}, { 0.50f,  0.00f},
 // fourth row
 {-0.50f, -0.25f}, {-0.25f, -0.25f}, { 0.00f, -0.25f},
 { 0.25f, -0.25f}, { 0.50f, -0.25f},
 // fifth (last) row
 {-0.50f, -0.50f}, {-0.25f, -0.50f}, { 0.00f, -0.50f},
 { 0.25f, -0.50f}, { 0.50f, -0.50f}
};

static int quadVerticesLength = sizeof(quadVertices) / sizeof(Vertex);

//
// Texture coordinates for each vertex of the quad.
//
static TexCoord quadUV[] = {
// top row
{0.00f, 1.00f}, {0.25f, 1.00f}, {0.50f, 1.00f}, {0.75f, 1.00f}, {1.00f, 1.00f},
// second row
{0.00f, 0.75f}, {0.25f, 0.75f}, {0.50f, 0.75f}, {0.75f, 0.75f}, {1.00f, 0.75f},
// third (middle) row
{0.00f, 0.50f}, {0.25f, 0.50f}, {0.50f, 0.50f}, {0.75f, 0.50f}, {1.00f, 0.50f},
// fourth row
{0.00f, 0.25f}, {0.25f, 0.25f}, {0.50f, 0.25f}, {0.75f, 0.25f}, {1.00f, 0.25f},
// fifth (last) row
{0.00f, 0.00f}, {0.25f, 0.00f}, {0.50f, 0.00f}, {0.75f, 0.00f}, {1.00f, 0.00f}
};

static int quadUVLength = sizeof(quadUV) / sizeof(TexCoord);

//
// Because the quad faces +Z, all the normals are (0,0,1)
//
static Normal quadNorm = { 0.0f, 0.0f, 1.0f };

//
// Unlike the other shapes, each vertex defining the quad is present exactly
// once in the vertex array.  This means that the element array isn't just
// a sequential list of vertex indices; instead, it must explicitly list
// the three vertex indices which define each triangle.
//
static GLuint quadElements[] = {
  // top row
   0,  5,  1,  5,  6,  1,  1,  6,  2,  6,  7,  2,
   2,  7,  3,  7,  8,  3,  3,  8,  4,  8,  9,  4,
  // second row
   5, 10,  6, 10, 11,  6,  6, 11,  7, 11, 12,  7,
   7, 12,  8, 12, 13,  8,  8, 13,  9, 13, 14,  9,
  // third row
  10, 15, 11, 15, 16, 11, 11, 16, 12, 16, 17, 12,
  12, 17, 13, 17, 18, 13, 13, 18, 14, 18, 19, 14,
  // fourth row
  15, 20, 16, 20, 21, 16, 16, 21, 17, 21, 22, 17,
  17, 22, 18, 22, 23, 18, 18, 23, 19, 23, 24, 19
};

static int quadElementsLength = sizeof(quadElements) / sizeof(GLuint);

#endif
