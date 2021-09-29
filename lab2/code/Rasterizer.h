//
//  Rasterizer.h
//
//  Simple module that implements rasterization algorithms
//
//  Created by Joe Geigel on 11/30/11.
//  Updates: 2019/02/09, 2019/09/14 by wrc.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#ifndef RASTERIZER_H_
#define RASTERIZER_H_

#include "Canvas.h"
#include "Types.h"

class Canvas;

class Rasterizer {

    //
    // number of scanlines
    //

    int n_scanlines;
    
public:

    //
    // Drawing canvas
    //

    Canvas &C;

    ///
    /// Constructor
    ///
    /// @param n number of scanlines
    /// @param C The Canvas to use
    ///
    Rasterizer( int n, Canvas &canvas );
    
    ///
    /// Draw a filled polygon
    ///
    /// Implementation should use the scan-line polygon fill algorithm
    /// discussed in class.
    ///
    /// The polygon has n distinct vertices.  The coordinates of the vertices
    /// making up the polygon are supplied in the 'v' array parameter, such
    /// that the ith vertex is in v[i].
    ///
    /// You are to add the implementation here using only calls to the
    /// addPixel() method of the canvas.
    ///
    /// @param n - number of vertices
    /// @param v - array of vertices
    ///
    void drawPolygon( int n, const Vertex v[] );
    
};

#endif
