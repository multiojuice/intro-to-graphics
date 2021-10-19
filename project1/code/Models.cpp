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

#include "Models.h"
#include "Application.h"
#include "Buffers.h"
#include "Pipeline.h"
#include "ShaderSetup.h"
#include "Types.h"
#include "Utils.h"

using namespace std;

//
// PRIVATE GLOBALS
//

//
// Object information
//

// a triangle in the upper-left corner
static const Vertex triangle_v[] = {
    { 25.0f, 125.0f }, { 75.0f, 125.0f }, { 50.0f, 175.0f }
};
static const int triangle_nv = sizeof(triangle_v) / sizeof(Vertex);
static const Color triangle_c = { 0.25f, 0.50f, 0.74f, 1.00f };
static int triangle_id;
static Vertex triangle_bb[2] = { { 25.0f, 125.0f }, { 75.0f, 175.0f } };

// an octagon in the upper-right corner
static const Vertex octagon_v[] = {
    { 125.0f, 133.0f },
    { 125.0f, 157.0f },
    { 143.0f, 175.0f },
    { 167.0f, 175.0f },
    { 185.0f, 157.0f },
    { 185.0f, 133.0f },
    { 167.0f, 115.0f },
    { 143.0f, 115.0f }
};
static const int octagon_nv = sizeof(octagon_v) / sizeof(Vertex);
static const Color octagon_c = { 0.90f, 0.00f, 0.00f, 1.00f };
static int octagon_id;
static Vertex octagon_bb[2] = { { 125.0f, 115.0f }, { 175.0f, 175.0f } };

// a teapot in the lower-left corner
static Vertex teapot_v[] = {
    {  17.50f, 40.00f }, {  20.62f, 35.00f }, {  25.00f, 28.75f },
    {  33.12f, 23.75f }, {  40.00f, 16.25f }, {  45.00f, 15.00f },
    {  72.50f, 15.00f }, {  77.50f, 15.00f }, {  83.75f, 21.25f },
    {  85.00f, 23.75f }, {  92.50f, 31.25f }, {  95.00f, 35.00f },
    {  97.50f, 40.00f }, { 102.50f, 47.50f }, { 103.25f, 48.75f },
    {  95.00f, 48.75f }, {  94.00f, 47.50f }, {  90.00f, 40.00f },
    {  88.25f, 37.50f }, {  83.00f, 35.00f }, {  78.75f, 50.00f },
    {  62.50f, 52.50f }, {  66.25f, 57.50f }, {  58.75f, 60.00f },
    {  51.25f, 57.50f }, {  55.00f, 52.50f }, {  40.00f, 50.00f },
    {  37.50f, 46.25f }, {  27.50f, 46.25f }, {  20.62f, 45.00f }
};
static const int teapot_nv = sizeof(teapot_v) / sizeof(Vertex);
static const Color teapot_c = { 0.20f, 0.90f, 0.20f, 1.00f };
static int teapot_id;
static Vertex teapot_bb[2] = { {  17.50f,  15.00f }, { 103.25f,  60.00f } };

// a star in the lower-right corner
static Vertex star_v[] = {
    { 123.60f, 10.40f }, { 146.10f, 26.40f }, { 168.60f, 10.40f },
    { 160.10f, 35.90f }, { 182.10f, 52.40f }, { 155.10f, 52.40f },
    { 148.10f, 79.40f }, { 137.10f, 52.40f }, { 110.10f, 52.40f },
    { 133.10f, 35.90f },
};
static const int star_nv = sizeof(star_v) / sizeof(Vertex);
static const Color star_c = { 0.68f, 0.00f, 0.75f, 1.00f };
static int star_id;
static Vertex star_bb[2] = { { 110.10f, 10.40f }, { 182.10f, 79.40f } };

// bounding box for the scene
static Vertex bb[2]={ { 0.0f, 0.0f }, { (float) w_width, (float) w_height } };

// standard colors
static const Color background = { 0.00f, 0.00f, 0.00f, 0.00f };
static const Color col1_c = { 0.98f, 0.31f, 0.08f, 1.00f };
static const Color col2_c = { 0.00f, 0.91f, 0.08f, 1.00f };
static const Color col3_c = { 0.98f, 0.00f, 0.48f, 1.00f };

//
// PUBLIC GLOBALS
//

// image to be displayed
int which;

//
// PRIVATE FUNCTIONS
//

//
// Macro to draw a polygon with its standard color
//
#define DRAW(x) { P.setColor( x ## _c ); P.drawPoly( x ## _id ); }

///
/// Draw plain old polygons.
///
static void drawPolysNorm( Pipeline &P )
{
    DRAW( triangle );
    DRAW( teapot );
    DRAW( octagon );
    DRAW( star );
}

///
/// Draw the world transformed, version 1
///
static void drawPolysXform1( Pipeline &P ) {

    //
    // Draw a triangle rotated
    //
    P.clearTransform();
    P.rotate( -25.0f );
    DRAW( triangle );

    //
    // Draw a teapot translated
    //
    P.clearTransform();
    P.translate( 80.0f, 375.0f );
    DRAW( teapot );

    //
    // Draw an octagon scaled
    //
    P.clearTransform();
    P.scale( 0.55f, 0.5f );
    DRAW( octagon );

    //
    // Draw a star translated, scaled, rotated, then scaled back
    //
    P.clearTransform();
    P.translate( 50.0f, 50.0f );
    P.scale( 2.0f, 2.0f );
    P.rotate( -10.0f );
    P.translate( -50.0f, 50.0f );
    DRAW( star );
}

///
/// Draw the world transformed, version 2
///
static void drawPolysXform2( Pipeline &P ) {

    //
    // Draw several rotated and translated triangles
    //
    P.clearTransform();
    P.translate( 0.0f, 300.0f );
    DRAW( triangle );

    P.clearTransform();
    P.translate( -25.0f, -125.0f );
    P.rotate( 70.0f );
    P.translate( 200.0f, 425.0f );
    DRAW( triangle );

    P.clearTransform();
    P.translate( -25.0f, -125.0f );
    P.rotate( 140.0f );
    P.translate( 325.0f, 425.0f );
    DRAW( triangle );

    P.clearTransform();
    P.translate( -25.0f, -125.0f );
    P.rotate( 210.0f );
    P.translate( 425.0f, 470.0f );
    DRAW( triangle );

    //
    // Translate and scale several teapots
    //
    P.clearTransform();
    P.scale( 2.0f, 2.0f );
    P.translate( 25.0f, 300.0f );
    DRAW( teapot );

    P.clearTransform();
    P.scale( 1.5f, 1.5f );
    P.translate( 125.0f, 200.0f );
    DRAW( teapot );

    P.clearTransform();
    P.scale( 0.75f, 0.75f );
    P.translate( 50.0f, 100.0f );
    DRAW( teapot );

    //
    // Scale the octagon a few times
    //
    P.clearTransform();
    P.translate( -125.0f, -115.0f );
    P.scale( 0.75f, 0.5f );
    P.translate( 125.0f, 15.0f );
    DRAW( octagon );

    P.clearTransform();
    P.translate( -125.0f, -115.0f );
    P.scale( 1.0f, 1.5f );
    P.translate( 15.0f, 200.0f );
    DRAW( octagon );

    P.clearTransform();
    P.translate( -125.0f, -115.0f );
    P.scale( 3.0f, 2.0f );
    P.translate( 300.0f, 15.0f );
    DRAW( octagon );

    //
    // Draw the star translated, scaled, rotated, then translated back
    //
    P.clearTransform();
    P.translate( 50.0f, 50.0f );
    P.scale( 2.5f, 2.5f );
    P.rotate( 40.0f );
    P.translate( 150.0f, -200.0f );
    DRAW( star );
}

//
// PUBLIC FUNCTIONS
//

///
/// Draw the objects
///
/// @param P   the Pipeline to use
///
void drawObjects( Pipeline &P )
{
    //
    // Set clear color to the background
    //
    P.setColor( background );
    P.clear();
    P.clearTransform();

    switch( which % N_IMAGES ) {

    case 0: // plain old polygon test

        glfwSetWindowTitle( w_window,
                "2D Pipeline: (0) Basic Polygons" );

        // default clip window
        P.setClipWindow( 0.0f, (float)(w_height-1),
                         0.0f, (float)(w_width-1) );

        // only 1/4 of the window
        // P.setClipWindow( 0.0f, (float)(w_height-1) / 2.0,
        //                  0.0f, (float)(w_width-1) / 2.0 );

        // default viewport
        P.setViewport( 0, 0, w_width, w_height );

        // draw the polys
        drawPolysNorm( P );
        break;

    case 1: // clipping test

        glfwSetWindowTitle( w_window,
                "2D Pipeline: (1) Alternate Clipping Window" );

        // modified clip window
        P.setClipWindow( 20.0f, 145.0f, 35.0f, 165.0f );

        // default viewport
        P.setViewport( 0, 0, w_width, w_height );

        // draw the polys
        drawPolysNorm( P );
        break;

    case 2: // varying transformations

        glfwSetWindowTitle( w_window,
                "2D Pipeline: (2) Simple Transformations" );

        // default clip window
        P.setClipWindow( 0.0f, (float)(w_height-1),
                         0.0f, (float)(w_width-1) );

        // default viewport
        P.setViewport( 0, 0, w_width, w_height );

        // draw the transformed polys
        drawPolysXform1( P );
        break;

    case 3: // varying transformations

        glfwSetWindowTitle( w_window,
                "2D Pipeline: (3) Multiple Transformations" );

        // default clip window
        P.setClipWindow( 0.0f, (float)(w_height-1),
                         0.0f, (float)(w_width-1) );

        // default viewport
        P.setViewport( 0, 0, w_width, w_height );

        // draw the transformed polys
        drawPolysXform2( P );
        break;

    case 4: // multiple varying viewports

        glfwSetWindowTitle( w_window,
                "2D Pipeline: (4) Viewport Mania" );

        // clip to the teapot bounding box +/- 5.0
#if 0
        P.setClipWindow( teapot_bb[0].y - 5.0f, teapot_bb[1].y + 5.0f,
                         teapot_bb[0].x - 5.0f, teapot_bb[1].x + 5.0f );
#else
        P.setClipWindow( bb[0].y - 5.0f, bb[1].y + 5.0f,
                         bb[0].x - 5.0f, bb[1].x + 5.0f );
#endif

        // have some fun with the view port
        int wdiff = w_width / 3;
        int hdiff = w_height / 3;

        // will use xaspect and yaspect to
        // draw each row with a different ratio
        int xaspect = w_width / 3;
        int yaspect = w_height / 3;
        int x = 0;
        int y = 0;
        int i, j;

        for( i = 0; i < 3; i++ ) {
            // adjust yaspect
            yaspect = hdiff/(i+1);

            for( j = 0; j < 3; j++ ) {
                // let's play around with colors
                if( i == j ) // 1::1 ratios
                    P.setColor( col1_c );
                else if( i < j ) // yaspect is smaller
                    P.setColor( col2_c );
                else // i > j, xaspect larger
                    P.setColor( col3_c );

                // adjust xaspect and shift to next column
                xaspect = wdiff/(j+1);
                P.setViewport( x, y, xaspect, yaspect );
                P.drawPoly( triangle_id );
                P.drawPoly( octagon_id );
                P.drawPoly( teapot_id );
                P.drawPoly( star_id );
                x += wdiff + 35;
            }
            // shift to next row, also add a little extra space
            // due to aspect ratio stuff making lots of blank canvas
            y += hdiff + 35;

            // change y aspect ratio
            xaspect = wdiff;

            // reset to left side of canvas
            x = 0;
        }
        break;
    }
}

///
/// Create our objects
///
/// @param P    The Pipeline we'll be using
///
void createObject( Pipeline &P ) {

    // add each polygon to the pipeline
    triangle_id = P.addPoly( triangle_nv, triangle_v );
    teapot_id   = P.addPoly( teapot_nv, teapot_v );
    octagon_id  = P.addPoly( octagon_nv, octagon_v );
    star_id     = P.addPoly( star_nv, star_v );

    // calculate the real bounding box
    // (RELIES ON THE ORIGINAL OBJECT LAYOUT)
    bb[0].x = teapot_bb[0].x;     // triangle @ 25, teapot @ 17.5
    bb[0].y = star_bb[0].y;       // star @ 10.4, teapot @ 15
    bb[1].x = star_bb[1].x;       // star @ 182.1, octagon @ 175
    bb[1].y = triangle_bb[1].y;   // triangle, octagon @ 175
}
