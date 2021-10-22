//
//  Pipeline.h
//
//  Created by Warren R. Carithers on 2016/10/19.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Updates: 2019/02/25, 2019/10/01, 2021/10/03 by wrc.
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "Canvas.h"
#include "Types.h"

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/geometric.hpp>

// a handy definition of PI
#define MY_PI   3.141592654

// shorthand way to convert degrees to radians
#define D2R(d)  ((d) * MY_PI / 180.0)

using namespace std;

///
/// Simple wrapper class for midterm assignment
///
/// Only methods in the implementation file whose bodies
/// contain the comment
///
///    // YOUR IMPLEMENTATION HERE
///
/// are to be modified by students.
///

struct Polygon {
    vector<Vertex> vertices;
};

class Pipeline : public Canvas {

private:
    int npolys;   /// number of polygon models added to the pipeline

    vector<Polygon> polys; // Polygon repository

    glm::mat3 tMatrix; // Transformation Matrix

    Vertex lowerLeftClip;
    Vertex upperRightClip;

    Vertex lowerLeftView;
    Vertex upperRightView;


    // Add any private data members and member functions here

public:

    ///
    /// Constructor
    ///
    /// @param w width of canvas
    /// @param h height of canvas
    ///
    Pipeline(int w, int h);

    ///
    /// addPoly - Add a polygon to the canvas.  This method does not draw
    ///           the polygon, but merely stores it for later drawing.
    ///           Drawing is initiated by the drawPoly() method.
    ///
    /// @param n - Number of vertices in polygon
    /// @param p - Array of Vertex entries defining the polygon to be added
    ///
    /// @return a unique integer identifier for the polygon
    ///
    int addPoly( int n, const Vertex p[] );

    ///
    /// drawPoly - Draw the polygon with the given id.  The polygon should
    ///            be drawn after applying the current transformation to
    ///            the vertices of the polygon.
    ///
    /// @param polyID - the ID of the polygon to be drawn.
    ///
    void drawPoly( int polyID );

    void drawPolygon( int n, const Vertex p[] );

    ///
    /// clearTransform - Set the current transformation to the identity matrix.
    ///
    void clearTransform( void );

    ///
    /// translate - Add a translation to the current transformation by
    ///             premultiplying the appropriate translation matrix to
    ///             the current transformation matrix.
    ///
    /// @param tx - Amount of translation in x.
    /// @param ty - Amount of translation in y.
    ///
    void translate( float tx, float ty );

    ///
    /// rotate - Add a rotation to the current transformation by premultiplying
    ///          the appropriate rotation matrix to the current transformation
    ///          matrix.
    ///
    /// @param degrees - Amount of rotation in degrees.
    ///
    void rotate( float degrees );

    ///
    /// scale - Add a scale to the current transformation by premultiplying
    ///         the appropriate scaling matrix to the current transformation
    ///         matrix.
    ///
    /// @param sx - Amount of scaling in x.
    /// @param sy - Amount of scaling in y.
    ///
    void scale( float sx, float sy );

    ///
    /// setClipWindow - Define the clip window.
    ///
    /// @param bottom - y coord of bottom edge of clip window (in world coords)
    /// @param top - y coord of top edge of clip window (in world coords)
    /// @param left - x coord of left edge of clip window (in world coords)
    /// @param right - x coord of right edge of clip window (in world coords)
    ///
    void setClipWindow( float bottom, float top, float left, float right );

    ///
    /// setViewport - Define the viewport.
    ///
    /// @param x - x coord of lower left of view window (in screen coords)
    /// @param y - y coord of lower left of view window (in screen coords)
    /// @param w - width of view window (in world coords)
    /// @param h - width of view window (in world coords)
    ///
    void setViewport( int x, int y, int w, int h );

};

int clipPolygon( int num, const Vertex inV[], Vertex outV[],
                 Vertex ll, Vertex ur );

#endif

