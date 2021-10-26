//
//  Pipeline.h
//
//  Created by Warren R. Carithers on 2016/10/19.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Updates: 2019/02/25, 2019/10/01, 2021/10/03 by wrc.
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Owen Sullivan
//

#ifndef PIPELINE_H_
#define PIPELINE_H_

#include "Canvas.h"
#include "Types.h"

#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/geometric.hpp>
#include <glm/gtx/string_cast.hpp>

// a handy definition of PI
#define MY_PI   3.141592654

// shorthand way to convert degrees to radians
#define D2R(d)  ((d) * MY_PI / 180.0)

using namespace std;

// A struct that represents a polygon aka the vertices 
struct Polygon {
    vector<Vertex> vertices;
};

///
/// Simple wrapper class for midterm assignment
///
/// Only methods in the implementation file whose bodies
/// contain the comment
///
///
/// are to be modified by students.
///

class Pipeline : public Canvas {

private:
    int npolys;   /// number of polygon models added to the pipeline

    vector<Polygon> polys; // Polygon repository

    glm::mat3 tMatrix; // Transformation Matrix

    // Vertices on outer edge of clip rectangle
    Vertex lowerLeftClip;
    Vertex upperRightClip;
    // Vertices on outer edge of view rectangle
    Vertex lowerLeftView;
    Vertex upperRightView;

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

    ///
    /// Draw a filled polygon.
    ///
    /// Implementation should use the scan-line polygon fill algorithm
    /// discussed in class.
    ///
    /// The polygon has n distinct vertices.  The coordinates of the vertices
    /// making up the polygon are supplied in the 'v' array parameter, such
    /// that the ith vertex is in v[i].
    ///
    /// You are to add the implementation here using only calls to the
    /// addPixel() function.
    ///
    /// @param n - number of vertices
    /// @param v - array of vertices
    ///
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


    /**
     * applyViewPort takes in number of verts and the vertices
     * This creates and applies a viewport tranformation matrix to 
     * each vertice and manipulates it in place
     * @param n - num of verts
     * @param v - vertices to apply to
     */
    void applyViewport(int n, Vertex v[]);

};

/**
 * applyMatrix takes in number of verts and the vertices and a matrix to apply
 * This applies a given matrix m to 
 * each vertice and manipulates it in place
 */
void applyMatrix(int n, Vertex v[], glm::mat3 matrix);

/**
 * Rounds a vertice up or down depending on
 * what int is closest.
 * Without this, my draw polygon doesn't work
 * Because I use int inputs and sloping
 */
Vertex round(Vertex v);

///
/// clipPolygon
///
/// Clip the polygon with vertex count in and vertices inV against the
/// rectangular clipping region specified by lower-left corner ll and
/// upper-right corner ur. The resulting vertices are placed in outV.
///
/// The routine should return the with the vertex count of polygon
/// resulting from the clipping.
///
/// @param num   the number of vertices in the polygon to be clipped
/// @param inV   the incoming vertex list
/// @param outV  the outgoing vertex list
/// @param ll    the lower-left corner of the clipping rectangle
/// @param ur    the upper-right corner of the clipping rectangle
///
/// @return number of vertices in the polygon resulting after clipping
///
int clipPolygon( int num, const Vertex inV[], Vertex outV[],
                 Vertex ll, Vertex ur );

#endif

