//
//  Models.cpp
//
//  Assignment-specific code for objects.
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
#include "Rasterizer.h"

#include "Models.h"

using namespace std;

//
// PRIVATE GLOBALS
//

//
// Object information
//

// ########### TEAPOT ###########
// Base
static Vertex g_base[] = {
    { 760.0f,  40.0f }, { 600.0f,  40.0f },
    { 620.0f,  60.0f }, { 740.0f,  60.0f }
};
static int n_base = sizeof(g_base) / sizeof(Vertex);
static Color c_base = { 1.0f, 0.0f, 0.0f };

// Body: right bottom triangle
static Vertex g_body1[] = {
    { 800.0f, 120.0f }, { 740.0f,  60.0f }, { 620.0f,  60.0f }
};
static int n_body1 = sizeof(g_body1) / sizeof(Vertex);
static Color c_body1 = { 0.9f, 0.0f, 0.0f };

// Body: midsection
static Vertex g_body2[] = {
    { 620.0f,  60.0f }, { 580.0f, 160.0f }, { 620.0f, 240.0f },
    { 740.0f, 240.0f }, { 800.0f, 120.0f }
};
static int n_body2 = sizeof(g_body2) / sizeof(Vertex);
static Color c_body2 = { 0.6f, 0.0f, 0.0f };

// Spout: lower triangle
static Vertex g_spout1[] = {
    { 620.0f,  60.0f }, { 560.0f, 100.0f }, { 500.0f, 180.0f }
};
static int n_spout1 = sizeof(g_spout1) / sizeof(Vertex);
static Color c_spout1 = { 0.8f, 0.0f, 0.0f };

// Spout: remainder
static Vertex g_spout2[] = {
    { 620.0f,  60.0f }, { 500.0f, 180.0f }, { 460.0f, 200.0f },
    { 520.0f, 200.0f }, { 580.0f, 160.0f }
};
static int n_spout2 = sizeof(g_spout2) / sizeof(Vertex);
static Color c_spout2 = { 0.7f, 0.0f, 0.0f };

// Handle
static Vertex g_handle[] = {
    { 800.0f, 120.0f }, { 840.0f, 160.0f }, { 855.0f, 200.0f },
    { 720.0f, 220.0f }, { 720.0f, 200.0f }, { 830.0f, 190.0f },
    { 825.0f, 165.0f }, { 780.0f, 120.0f }
};
static int n_handle = sizeof(g_handle) / sizeof(Vertex);
static Color c_handle = { 0.8f, 0.0f, 0.2f };

// Lid
static Vertex g_lid[] = {
    { 690.0f, 240.0f }, { 710.0f, 260.0f },
    { 650.0f, 260.0f }, { 670.0f, 240.0f }
};
static int n_lid = sizeof(g_lid) / sizeof(Vertex);
static Color c_lid = { 0.8f, 0.2f, 0.2f };

// ######## SHAPES #######
// Triangle
static Vertex g_tri[] = {
    { 460.0f, 220.0f }, { 490.0f, 280.0f }, { 420.0f, 280.0f }
};
static int n_tri = sizeof(g_tri) / sizeof(Vertex);
static Color c_tri = { 0.8f, 0.2f, 1.0f };

// Quad
static Vertex g_quad[] = {
    { 380.0f, 280.0f }, { 320.0f, 320.0f },
    { 360.0f, 380.0f }, { 420.0f, 340.0f }
};
static int n_quad = sizeof(g_quad) / sizeof(Vertex);
static Color c_quad = { 0.0f, 0.8f, 0.8f };

// Star: right half
static Vertex g_star1[] = {
    { 230.0f, 389.0f }, { 260.0f, 369.0f }, { 254.0f, 402.0f },
    { 278.0f, 425.0f }, { 245.0f, 430.0f }, { 230.0f, 460.0f },
    { 230.0f, 410.0f }
};
static int n_star1 = sizeof(g_star1) / sizeof(Vertex);
static Color c_star1 = { 1.0f, 0.60f, 0.0f };

// Star: left half
static Vertex g_star2[] = {
    { 230.0f, 460.0f }, { 216.0f, 430.0f }, { 183.0f, 425.0f },
    { 207.0f, 402.0f }, { 201.0f, 369.0f }, { 230.0f, 389.0f },
    { 230.0f, 410.0f }
};
static int n_star2 = sizeof(g_star2) / sizeof(Vertex);
static Color c_star2 = { 1.0f, 0.8f, 0.0f };

// ########## BORDERS ###############
// Bottom left corner: square
static Vertex g_sqbl[] = {
    {   0.0f,   0.0f }, {   0.0f,  20.0f },
    {  20.0f,  20.0f }, {  20.0f,   0.0f }
};
static int n_sqbl = sizeof(g_sqbl) / sizeof(Vertex);
static Color c_sqbl = { 0.0f, 0.4f, 0.4f };

// Top left corner: square
static Vertex g_sqtl[] = {
    {   0.0f, 580.0f }, {   0.0f, 599.0f },
    {  20.0f, 599.0f }, {  20.0f, 580.0f }
};
static int n_sqtl = sizeof(g_sqtl) / sizeof(Vertex);
static Color c_sqtl = { 0.0f, 0.2f, 0.8f };

// Top right corner: square
static Vertex g_sqtr[] = {
    { 899.0f, 599.0f }, { 899.0f, 580.0f },
    { 880.0f, 580.0f }, { 880.0f, 599.0f }
};
static int n_sqtr = sizeof(g_sqtr) / sizeof(Vertex);
static Color c_sqtr = { 0.0f, 0.6f, 0.2f };

// Bottom right corner: square
static Vertex g_sqbr[] = {
    { 899.0f,   0.0f }, { 899.0f,  20.0f },
    { 880.0f,  20.0f }, { 880.0f,   0.0f }
};
static int n_sqbr = sizeof(g_sqbr) / sizeof(Vertex);
static Color c_sqbr = { 0.0f, 0.6f, 0.0f };

// Bottom edge: quad
static Vertex g_qb[] = {
    {  20.0f,   0.0f }, {  20.0f,  20.0f },
    { 880.0f,  20.0f }, { 880.0f,   0.0f }
};
static int n_qb = sizeof(g_qb) / sizeof(Vertex);
static Color c_qb = { 0.2f, 0.8f, 0.0f };

// Left edge: quad
static Vertex g_ql[] = {
    {   0.0f,  20.0f }, {  20.0f,  20.0f },
    {  20.0f, 580.0f }, {   0.0f, 580.0f }
};
static int n_ql = sizeof(g_ql) / sizeof(Vertex);
static Color c_ql = { 0.0f, 0.2f, 1.0f };

// Top edge: upper triangle
static Vertex g_trt1[] = {
    {  20.0f, 580.0f }, {  20.0f, 599.0f }, { 880.0f, 599.0f }
};
static int n_trt1 = sizeof(g_trt1) / sizeof(Vertex);
static Color c_trt1 = { 0.0f, 0.4f, 1.0f };

// Top edge: lower triangle
static Vertex g_trt2[] = {
    {  20.0f, 580.0f }, { 880.0f, 580.0f }, { 880.0f, 599.0f }
};
static int n_trt2 = sizeof(g_trt2) / sizeof(Vertex);
static Color c_trt2 = { 0.0f, 0.6f, 1.0f };

// Right edge: lefthand triangle
static Vertex g_trr1[] = {
    { 880.0f, 580.0f }, { 899.0f, 580.0f }, { 880.0f,  20.0f }
};
static int n_trr1 = sizeof(g_trr1) / sizeof(Vertex);
static Color c_trr1 = { 0.0f, 1.0f, 0.4f };

// Right edge: righthand triangle
static Vertex g_trr2[] = {
    { 899.0f, 580.0f }, { 899.0f,  20.0f }, { 880.0f,  20.0f }
};
static int n_trr2 = sizeof(g_trr2) / sizeof(Vertex);
static Color c_trr2 = { 0.0f, 0.8f, 0.2f };

///
/// Create an object
///
/// @param num  Which object we are creating
/// @param R    The Rasterizer we'll be using
/// @param B    The BufferSet for the created object
///
void createObject( int num, Rasterizer &R, BufferSet &B )
{
    // start with a clean canvas
    R.C.clear();

    switch( num ) {

        // ######## TEAPOT #######
    case Base:
        // Base
        R.C.setColor( c_base );
        R.drawPolygon( n_base, g_base );
        break;

    case Body1:
        // Body: right bottom triangle
        R.C.setColor( c_body1 );
        R.drawPolygon( n_body1, g_body1 );
        break;

    case Body2:
        // Body: midsection
        R.C.setColor( c_body2 );
        R.drawPolygon( n_body2, g_body2 );
        break;

    case Spout1:
        // Spout: lower triangle
        R.C.setColor( c_spout1 );
        R.drawPolygon( n_spout1, g_spout1 );
        break;

    case Spout2:
        // Spout: remainder
        R.C.setColor( c_spout2 );
        R.drawPolygon( n_spout2, g_spout2 );
        break;

    case Handle:
        // Handle
        R.C.setColor( c_handle );
        R.drawPolygon( n_handle, g_handle );
        break;

    case Lid:
        // Lid
        R.C.setColor( c_lid );
        R.drawPolygon( n_lid, g_lid );
        break;

        // ######## SHAPES #######
    case Triangle:
        // Triangle
        R.C.setColor( c_tri );
        R.drawPolygon( n_tri, g_tri );
        break;

    case Quad:
        // Quad
        R.C.setColor( c_quad );
        R.drawPolygon( n_quad, g_quad );
        break;

    case Star1:
        // Star: right half
        R.C.setColor( c_star1 );
        R.drawPolygon( n_star1, g_star1 );
        break;

    case Star2:
        // Star: left half
        R.C.setColor( c_star2 );
        R.drawPolygon( n_star2, g_star2 );
        break;

        // ########## BORDERS ###############
    case Square_ll:
        // Bottom left corner: square
        R.C.setColor( c_sqbl );
        R.drawPolygon( n_sqbl, g_sqbl );
        break;

    case Square_ul:
        // Top left corner: square
        R.C.setColor( c_sqtl );
        R.drawPolygon( n_sqtl, g_sqtl );
        break;

    case Square_ur:
        // Top right corner: square
        R.C.setColor( c_sqtr );
        R.drawPolygon( n_sqtr, g_sqtr );
        break;

    case Square_lr:
        // Bottom right corner: square
        R.C.setColor( c_sqbr );
        R.drawPolygon( n_sqbr, g_sqbr );
        break;

    case Side1:
        // Bottom edge: quad
        R.C.setColor( c_qb );
        R.drawPolygon( n_qb, g_qb );
        break;

    case Side2:
        // Left edge: quad
        R.C.setColor( c_ql );
        R.drawPolygon( n_ql, g_ql );
        break;

    case Tri_u1:
        // Top edge: upper triangle
        R.C.setColor( c_trt1 );
        R.drawPolygon( n_trt1, g_trt1 );
        break;

    case Tri_u2:
        // Top edge: lower triangle
        R.C.setColor( c_trt2 );
        R.drawPolygon( n_trt2, g_trt2 );
        break;

    case Tri_r1:
        // Right edge: lefthand triangle
        R.C.setColor( c_trr1 );
        R.drawPolygon( n_trr1, g_trr1 );
        break;

    case Tri_r2:
        // Right edge: righthand triangle
        R.C.setColor( c_trr2 );
        R.drawPolygon( n_trr2, g_trr2 );
        break;

    default:
        cerr << "bad polygon # " << num << " ignored" << endl;
    }

    // finally, create the buffers
    B.createBuffers( R.C );
}
