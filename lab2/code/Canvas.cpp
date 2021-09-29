//
//  Canvas.cpp
//
//  Routines for manipulating a simple canvas holding point information
//  along with color, surface normal, and texture coordinate data.
//
//  Created by Warren R. Carithers 2016/09/30.
//  Based on a C++ version created by Joe Geigel.
//  Updates: 2018/11/29, 2019/09/09, 2020/01/03 by wrc.
//  Copyright 2020 Rochester Institute of Technology. All rights reserved.
//
//  This file should not be modified by students.
//
//  This module provides two basic interfaces:  a pixel interface for
//  simple 2D drawings, and a vertex interface for 3D drawings. The pixel
//  interface consists of two functions:
//
//      addPixel()          adds a pixel using the current drawing color
//      addPixelColor()     adds a pixel using the specified color
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

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <glm/vec3.hpp>
#include <glm/geometric.hpp>

// Canvas.h includes all the OpenGL/GLFW/etc. header files for us
#include "Canvas.h"
#include "Utils.h"

///
/// Constructor
///
/// @param w width of canvas
/// @param h height of canvas
///
Canvas::Canvas( int w, int h ) : width(w), height(h) {
    // G++ allows us to use (Color) { ... }, but Visual Studio
    // doesn't, so we do this the long way to keep everyone happy
    Color black = { 0.0f, 0.0f, 0.0f, 1.0f };
    currentColor = black;
    currentDepth = -1.0f;
    pointArray = 0;
    colorArray = 0;
    normalArray = 0;
    uvArray = 0;
    elemArray = 0;
    numElements = 0;
}

///
/// Destructor
///
Canvas::~Canvas( void ) {
    clear();
}

    /////////////////////////////////////
    // Basic Canvas manipulation
    /////////////////////////////////////

///
/// Clear the canvas
///
void Canvas::clear( void )
{
    if( pointArray ) {
        delete [] pointArray;
        pointArray = 0;
    }
    if( normalArray ) {
        delete [] normalArray;
        normalArray = 0;
    }
    if( uvArray ) {
        delete [] uvArray;
        uvArray = 0;
    }
    if( elemArray ) {
        delete [] elemArray;
        elemArray = 0;
    }
    if( colorArray ) {
        delete [] colorArray;
        colorArray = 0;
    }
    points.clear();
    normals.clear();
    uv.clear();
    colors.clear();
    numElements = 0;
    Color black = { 0.0f, 0.0f, 0.0f, 1.0f };
    currentColor = black;
    currentDepth = -1.0f;
}

///
/// Set the pixel Z coordinate
///
/// @param d   The depth to use
/// @return    The old depth value
///
float Canvas::setDepth( float d )
{
    float r = currentDepth;

    currentDepth = d;
    return( r );
}

///
/// Set the current drawing color
///
/// @param color   The desired color
/// @return The old color value
///
Color Canvas::setColor( Color color )
{
    Color old = currentColor;

    currentColor = color;
    return( old );
}

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
void Canvas::addPixel( int x, int y )
{
    // we assume that we're working in 2D, and ignore the Z
    // coordinate that came in with the pixel location
    Vertex pix = { (float) x, (float) y, currentDepth };

    // ignore the alpha channel value for the current color
    Color col = { currentColor.r, currentColor.g, currentColor.b, 1.0f };

    addVertex( pix );
    addColor( col );
}

///
/// Add a pixel using the current drawing color
///
/// @param p   The pixel to be added
///
void Canvas::addPixel( Vertex p )
{
    // we assume that we're working in 2D, and ignore the Z
    // coordinate that came in with the pixel location
    Vertex pix = { p.x, p.y, currentDepth };

    // ignore the alpha channel value for the current color
    Color col = { currentColor.r, currentColor.g, currentColor.b, 1.0f };

    addVertex( pix );
    addColor( col );
}

///
/// Add a pixel using the specified drawing color
///
/// @param p     The pixel to be set
/// @param c     The desired color
///
void Canvas::addPixel( Vertex p, Color c )
{
    Vertex pix = { p.x, p.y, currentDepth };
    Color col = { c.r, c.g, c.b, 1.0f };

    addVertex( pix );
    addColor( col );
}

    /////////////////////////////////////
    // Individual things (vertices, etc.)
    /////////////////////////////////////

///
/// Add a color to the current shape
///
/// @param c   The color to be added
///
void Canvas::addColor( Color c )
{
    colors.push_back( c.r );
    colors.push_back( c.g );
    colors.push_back( c.b );
    colors.push_back( c.a );
}

///
/// Add a vertex to the current shape
///
/// @param v   The vertex to be added
///
void Canvas::addVertex( Vertex v )
{
    points.push_back( v.x );
    points.push_back( v.y );
    points.push_back( v.z );
    points.push_back( 1.0f );  // ignore the homogeneous coordinate

    // here is where we actually count the number of
    // things that have been put into the canvas
    numElements += 1;
}

///
/// Add a normal vector to the current shape
///
/// @param n   The normal to be added
///
void Canvas::addNormal( Normal n )
{
    normals.push_back( n.x );
    normals.push_back( n.y );
    normals.push_back( n.z );
}

///
/// Add a set of texture coordinates to the current shape
///
/// @param t   The texture coordinate to be added
///
void Canvas::addTexCoord( TexCoord t )
{
    uv.push_back( t.u );
    uv.push_back( t.v );
}

    /////////////////////////////////////
    // Larger things (triangles, etc.)
    /////////////////////////////////////

///
/// Add a triangle to the current shape
///
/// Calculates a triangle-wide surface normal and adds that as well
///
/// @param p0 first triangle vertex
/// @param p1 second triangle vertex
/// @param p2 final triangle vertex
///
void Canvas::addTriangle( Vertex p0, Vertex p1, Vertex p2 )
{
    // calculate the normal for the triangle
    glm::vec3 u( p1.x - p0.x, p1.y - p0.y, p1.z - p0.z );
    glm::vec3 v( p2.x - p0.x, p2.y - p0.y, p2.z - p0.z );
    glm::vec3 n( glm::cross(u,v) );

    Normal nn = { n[0], n[1], n[2] };

    addTriangleWithNorms( p0, nn, p1, nn, p2, nn );
}

///
/// Add a triangle to the current shape, along with normal data
///
/// @param p0 first triangle vertex
/// @param n0 first triangle normal data
/// @param p1 second triangle vertex
/// @param n1 second triangle normal data
/// @param p2 final triangle vertex
/// @param n2 final triangle normal data
///
void Canvas::addTriangleWithNorms( Vertex p0, Normal n0,
             Vertex p1, Normal n1, Vertex p2, Normal n2 )
{
    addVertex( p0 );   addNormal( n0 );
    addVertex( p1 );   addNormal( n1 );
    addVertex( p2 );   addNormal( n2 );
}

///
/// Add texture coordinates to the current shape
///
/// @param uv0 first vertex (u,v) data
/// @param uv1 second vertex (u,v) data
/// @param uv2 final vertex (u,v) data
///
void Canvas::addTextureCoords( TexCoord uv0, TexCoord uv1, TexCoord uv2 )
{
    addTexCoord( uv0 );
    addTexCoord( uv1 );
    addTexCoord( uv2 );
}

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
GLuint *Canvas::getElements( void )
{
    // delete the old element array if we have one
    if( elemArray ) {
        delete [] elemArray;
        elemArray = 0;
    }

    int n = numElements;

    if( n > 0 ) {
        // create and fill a new element array
        elemArray = new GLuint[ n ];
        if( elemArray == 0 ) {
            cerr << "element allocation failure" << endl;
            exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            elemArray[i] = i;
        }
    }

    return elemArray;
}

///
/// Retrieve the array of vertex data from this Canvas
///
/// @return A pointer to a dynamic array of data, or NULL
///
float *Canvas::getVertices( void )
{
    // delete the old point array if we have one
    if( pointArray ) {
        delete [] pointArray;
        pointArray = 0;
    }

    int n = points.size();

    if( n > 0 ) {
        // create and fill a new point array
        pointArray = new float[ n ];
        if( pointArray == 0 ) {
            cerr << "point allocation failure" << endl;
            exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            pointArray[i] = points[i];
        }
    }

    return pointArray;
}

///
/// Retrieve the array of normal data from this Canvas
///
/// @return A pointer to a dynamic array of data, or NULL
///
float *Canvas::getNormals( void )
{
    // delete the old normal array if we have one
    if( normalArray ) {
        delete [] normalArray;
        normalArray = 0;
    }

    int n = normals.size();

    if( n > 0 ) {
        // create and fill a new normal array
        normalArray = new float[ n ];
        if( normalArray == 0 ) {
            cerr << "normal allocation failure" << endl;
            exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            normalArray[i] = normals[i];
        }
    }

    return normalArray;
}

///
/// Retrieve the array of texture coordinate data from this Canvas
///
/// @return A pointer to a dynamic array of data, or NULL
///
float *Canvas::getUV( void )
{
    // delete the old texture coordinate array if we have one
    if( uvArray ) {
        delete [] uvArray;
        uvArray = 0;
    }

    int n = uv.size();

    if( n > 0 ) {
        // create and fill a new texture coordinate array
        uvArray = new float[ n ];
        if( uvArray == 0 ) {
            cerr << "uv allocation failure" << endl;
            exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            uvArray[i] = uv[i];
        }
    }

    return uvArray;
}

///
/// Retrieve the array of color data from this Canvas
///
/// @return A pointer to a dynamic array of data, or NULL
///
float *Canvas::getColors( void )
{
    // delete the old color array if we have one
    if( colorArray ) {
        delete [] colorArray;
        colorArray = 0;
    }

    int n = colors.size();

    if( n > 0 ) {
        // create and fill a new color array
        colorArray = new float[ n ];
        if( colorArray == 0 ) {
            cerr << "color allocation failure" << endl;
            exit( 1 );
        }
        for( int i = 0; i < n; i++ ) {
            colorArray[i] = colors[i];
        }
    }

    return colorArray;
}

///
/// Retrieve the vertex count from this Canvas
///
/// @return The number of vertices in the canvas
///
int Canvas::numVertices( void )
{
    return numElements;
}
