//
//  Canvas.h
//
//  Routines for manipulating a simple canvas holding point information
//  along with color, surface normal, and texture coordinate data.
//
//  Created by Warren R. Carithers 2016/09/30.
//  Based on a C++ version created by Joe Geigel.
//  Updates: 2018/11/29, 2019/09/09, 2020/01/03, 2021/09/11 by wrc.
//  Copyright 2020 Rochester Institute of Technology.  All rights reserved.
//
//  This file should not be modified by students.
//
//  This module provides two basic interfaces:  a pixel interface for
//  simple 2D drawings, and a vertex interface for 3D drawings.  The pixel
//  interface consists of these functions:
//
//      addPixel(x,y)   adds a pixel at (x,y) using the current drawing color
//      addPixel(v)     adds a pixel at Vertex v using the current color
//      addPixel(v,c)   adds a pixel at Vertex v using the Color c
//
//  These functions assume that every pixel should have a color, and
//  ensure that both position and color data are added to the canvas
//  when each is called.  As we're working in 2D, only the X and Y
//  components of the pixel location are used, and the alpha channel
//  of the color is forced to 1.0.
//
//  For 3D drawings, vertices, colors, surface normals, and texture
//  coordinates are added separately.  Vertices are counted; the module
//  assumes that the application will add the relevant additional data
//  as needed, and the vertex count is used to determine how much data
//  will be returned when each type of data is requested by the
//  application.  It is the application's responsibility to ensure that
//  all the relevant data has been added to the canvas in the proper
//  sequence.
//

#ifndef CANVAS_H_
#define CANVAS_H_

#if defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#endif

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Types.h"

using namespace std;

#include <vector>

///
/// Simple canvas class that allows for pixel-by-pixel rendering.
///

class Canvas {

    //
    // canvas size information
    //
    int width;
    int height;

    //
    // point-related data
    //

    // vertex locations
    vector<float> points;
    float *pointArray;

    // associated normal vectors
    vector<float> normals;
    float *normalArray;

    // associated (u,v) coordinates
    vector<float> uv;
    float *uvArray;

    // associated color data
    vector<float> colors;
    float *colorArray;

    // element count and connectivity data
    int numElements;
    GLuint *elemArray;

    //
    // other Canvas defaults
    //

    // current drawing color
    Color currentColor;

    // drawing depth
    float currentDepth;

public:
    ///
    /// Constructor
    ///
    /// @param w width of canvas
    /// @param h height of canvas
    ///
    Canvas( int w, int h );

    ///
    /// Destructor
    ///
    ~Canvas( void );

    /////////////////////////////////////
    // Basic Canvas manipulation
    /////////////////////////////////////

    ///
    /// Clear the canvas
    ///
    void clear( void );

    ///
    /// Set the pixel Z coordinate
    ///
    /// @param d   The depth to use
    /// @return    The old depth value
    ///
    float setDepth( float d );

    ///
    /// Set the current drawing color
    ///
    /// @param color   The desired color
    /// @return  The old color value
    ///
    Color setColor( Color color );

    /////////////////////////////////////
    //
    // Adding things to the Canvas
    //
    /////////////////////////////////////

    /////////////////////////////////////
    // 2D interface:  Setting pixels
    /////////////////////////////////////

    ///
    /// Add a pixel using the current drawing color
    ///
    /// @param x   The x coordinate of the pixel to be added
    /// @param y   The y coordinate of the pixel to be added
    ///
    void addPixel( int x, int y );

    ///
    /// Add a pixel using the current drawing color
    ///
    /// @param p   The pixel to be added
    ///
    void addPixel( Vertex p );

    ///
    /// Add a pixel using the specified drawing color
    ///
    /// @param p     The pixel to be added
    /// @param c     The desired color
    ///
    void addPixel( Vertex p, Color c );

    /////////////////////////////////////
    // Individual things (vertices, etc.)
    /////////////////////////////////////

    ///
    /// Add a color to the current shape
    ///
    /// @param c   The color to be added
    ///
    void addColor( Color c );

    ///
    /// Add a vertex to the current shape
    ///
    /// @param v   The vertex to be added
    ///
    void addVertex( Vertex v );

    ///
    /// Add a set of texture coordinates to the current shape
    ///
    /// @param t   The texture coordinate to be added
    ///
    void addTexCoord( TexCoord t );

    ///
    /// Add a normal vector to the current shape
    ///
    /// @param n   The normal to be added
    ///
    void addNormal( Normal n );

    /////////////////////////////////////
    // Larger things (triangles, etc.)
    /////////////////////////////////////

    ///
    /// Add a triangle to the current shape
    ///
    /// Calculates a triangle-wide surface normal and adds that as well
    ///
    /// @param p0   first triangle vertex
    /// @param p1   second triangle vertex
    /// @param p2   final triangle vertex
    ///
    void addTriangle( Vertex p0, Vertex p1, Vertex p2 );

    ///
    // Add a triangle to the current shape, along with normal data
    ///
    /// @param p0   first vertex
    /// @param n0   first vertex normal data
    /// @param p1   second vertex
    /// @param n1   second vertex normal data
    /// @param p2   final vertex
    /// @param n2   final vertex normal data
    ///
    void addTriangleWithNorms( Vertex p0, Normal n0,
        Vertex p1, Normal n1, Vertex p2, Normal n2 );

    ///
    /// Add texture coordinates to the current shape
    ///
    /// @param uv0 first vertex (u,v) data
    /// @param uv1 second vertex (u,v) data
    /// @param uv2 final vertex (u,v) data
    ///
    void addTextureCoords( TexCoord uv0, TexCoord uv1, TexCoord uv2 );

    /////////////////////////////////////
    //
    // Retrieving things from the Canvas
    //
    /////////////////////////////////////

    ///
    /// Retrieve the array of element data from this Canvas
    ///
    /// @return A pointer to a dynamic array of data, or NULL
    ///
    GLuint *getElements( void );

    ///
    /// Retrieve the array of vertex data from this Canvas
    ///
    /// @return A pointer to a dynamic array of data, or NULL
    ///
    float *getVertices( void );

    ///
    /// Retrieve the array of normal data from this Canvas
    ///
    /// @return A pointer to a dynamic array of data, or NULL
    ///
    float *getNormals( void );

    ///
    /// Retrieve the array of (u,v) data from this Canvas
    ///
    /// @return A pointer to a dynamic array of data, or NULL
    ///
    float *getUV( void );

    ///
    /// Retrieve the array of color data from this Canvas
    ///
    /// @return A pointer to a dynamic array of data, or NULL
    ///
    float *getColors( void );

    ///
    /// Retrieve the vertex count from this Canvas
    ///
    /// @return The number of vertices in the canvas
    ///
    int numVertices( void );

};

#endif
