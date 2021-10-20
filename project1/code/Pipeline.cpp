//
//  Pipeline.cpp
//
//  Created by Warren R. Carithers on 2016/10/19.
//  Based on a version created by Joe Geigel on 11/30/11.
//  Updates: 2019/02/25, 2019/10/01, 2021/10/03 by wrc.
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Owen Sullivan
//

#include <iostream>
#include "Pipeline.h"

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

///
/// Constructor
///
/// @param w width of canvas
/// @param h height of canvas
///
Pipeline::Pipeline( int w, int h ) : Canvas(w,h)
    // YOUR IMPLEMENTATION HERE if you need to add initializers
{
    npolys = 0;   // initially, the repository is empty

    // YOUR IMPLEMENTATION HERE if you need to add other constructor code
}

///
/// addPoly - Add a polygon to the canvas.  This method does not draw
///           the polygon, but merely stores it for later drawing.
///           Drawing is initiated by the drawPoly() method.
///
///           Returns a unique integer id for the polygon.
///
/// @param n - Number of vertices in polygon
/// @param p - Array containing the vertices of the polygon to be added.
///
/// @return a unique integer identifier for the polygon
///
int Pipeline::addPoly( int n, const Vertex p[] )
{
    cout << "Out of " << n << " :";
    Polygon newPoly;
    for (int i = 0; i < n; i++) {
        cout << i << " ";
        Vertex round;
        round.x = floor(p[i].x + 0.5);
        round.y = floor(p[i].y + 0.5);
        newPoly.vertices.push_back(round);
    }

    cout << "\n";

    this->polys.push_back(newPoly);
    npolys++;

    return this->polys.size() - 1;
}

///
/// drawPoly - Draw the polygon with the given id.  The polygon should
///            be drawn after applying the current transformation to
///            the vertices of the polygon.
///
/// @param polyID - the ID of the polygon to be drawn.
///
void Pipeline::drawPoly( int polyID )
{
    // if this isn't a valid ID, print a message and return
    if( polyID < 0 || polyID >= npolys ) {
        cerr << "error: drawPoly(" << polyID << "), invalid ID" << endl;
        return;
    }

    cerr << "drawPoly(" << polyID << "), size " << this->polys.size() << endl;
    vector<Vertex> currentV = this->polys[polyID].vertices;


    Vertex v[currentV.size()]; 
    for (int i = 0; i < currentV.size(); i++) {
        cout << currentV[i].x << " " << currentV[i].y << "\n";
        v[i] = currentV[i];
    }

    // drawOutline(currentV.size(), v);
    drawPolygon(currentV.size(), v);
}

///
/// clearTransform - Set the current transformation to the identity matrix.
///
void Pipeline::clearTransform( void )
{
    glm::mat3 identityMatrix = glm::mat3(1.0f);
    this->tMatrix = identityMatrix;
}

///
/// translate - Add a translation to the current transformation by
///             premultiplying the appropriate translation matrix to
///             the current transformation matrix.
///
/// @param tx - Amount of translation in x.
/// @param ty - Amount of translation in y.
///
void Pipeline::translate( float tx, float ty )
{
    // YOUR IMPLEMENTATION HERE
}

///
/// rotate - Add a rotation to the current transformation by premultiplying
///          the appropriate rotation matrix to the current transformation
///          matrix.
///
/// @param degrees - Amount of rotation in degrees.
///
void Pipeline::rotate( float degrees )
{
    // YOUR IMPLEMENTATION HERE
}

///
/// scale - Add a scale to the current transformation by premultiplying
///         the appropriate scaling matrix to the current transformation
///         matrix.
///
/// @param sx - Amount of scaling in x.
/// @param sy - Amount of scaling in y.
///
void Pipeline::scale( float sx, float sy )
{
    // YOUR IMPLEMENTATION HERE
}

///
/// setClipWindow - Define the clip window.
///
/// @param bottom - y coord of bottom edge of clip window (in world coords)
/// @param top - y coord of top edge of clip window (in world coords)
/// @param left - x coord of left edge of clip window (in world coords)
/// @param right - x coord of right edge of clip window (in world coords)
///
void Pipeline::setClipWindow( float bottom, float top, float left, float right )
{
    // YOUR IMPLEMENTATION HERE
}

///
/// setViewport - Define the viewport.
///
/// @param x - x coord of lower left of view window (in screen coords)
/// @param y - y coord of lower left of view window (in screen coords)
/// @param w - width of view window (in pixels)
/// @param h - width of view window (in pixels)
///
void Pipeline::setViewport( int x, int y, int w, int h )
{
    // YOUR IMPLEMENTATION HERE
}




/*********************************************
 * DrawPolygon from Rasterizer.cpp Lab2
 * 
 ********************************************/


// Struct that holds information about the edges
// And their current state
struct EdgeBucket {
    int yMax;
    int yMin;
    int x;
    int sum;
    int dX;
    int dY;
    int sign;
};

/**
 * Generates an edgebucket between vertex a and b
 * This contains information about slopes and current
 * states
 */
EdgeBucket generateEdgeBucket(Vertex a, Vertex b) {
    
    EdgeBucket current;
    current.dX = abs(b.x - a.x);
    current.dY = abs(b.y - a.y);
    current.yMax = max(b.y, a.y);
    current.yMin = min(b.y, a.y);

    // Set the sign of the slope
    if (current.yMin == b.y) {
        current.x = b.x;
        if (b.x < a.x) {
            current.sign = 1;
        } else {
            current.sign = -1;
        }
    } else {
        current.x = a.x;
        if (b.x < a.x) {
            current.sign = -1;
        } else {
            current.sign = 1;
        }
    }

    current.sum = 0;
    return current;
}

/**
 * Init the entire edgetable based on the vertexes in v
 */
vector<EdgeBucket> initEdgeTable(int n, const Vertex v[]) {
    // The edge table to populate
    vector<EdgeBucket> edgeTable;
    Vertex a = v[n - 1];
    Vertex b = v[0];

    // If the first and last points are not horizontal
    if (a.y != b.y) {
        edgeTable.push_back(generateEdgeBucket(a, b));
    }

    // Generate the edgebuckets for each pair of a and b
    int id = 1;
    for (int i = 1; i < n; i++) {
        a = v[i - 1];
        b = v[i];
        if (a.y != b.y) {
            edgeTable.push_back(generateEdgeBucket(a, b));
        }
        id++;
    }

    return edgeTable;
}

/*
 * A sort function I use for vectors that sort by edge min in descending order
 */
bool sortByMinY(EdgeBucket a, EdgeBucket b) {
    return a.yMin > b.yMin;
}

/*
 * A sort function used for vector sorts by x value small to high
 */
bool sortByX(EdgeBucket a, EdgeBucket b) {
    return a.x < b.x;
}

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
void Pipeline::drawPolygon( int n, const Vertex v[] )
{
    // Generate the edge table
    vector<EdgeBucket> edgeTable = initEdgeTable(n, v);
    sort(edgeTable.begin(), edgeTable.end(), sortByMinY);

    // Keep plotting each pixel. Good for debugging and completion
    for( int i = 0; i < n; ++i ) {
        addPixel( v[i] );
    }

    int currentY = 0; 
    vector<EdgeBucket> activeList;
    // Main loop, check if we still have edges
    while ((!edgeTable.empty() || !activeList.empty()) && currentY < 900) {
        vector<EdgeBucket> tempList;
        
        // Remove edges that are now out of scope of the active list
        for (int o = 0; o < activeList.size(); o++) {
            if (currentY < activeList[o].yMax) {
                tempList.push_back(activeList[o]);
            }
        }

        activeList = tempList;

        // Add edges we are now in contact with from the edge table
        // and pop them out of the edge table
        while (!edgeTable.empty()) {
            EdgeBucket back = edgeTable.back();
            if (currentY >= back.yMin) {
                edgeTable.pop_back();
                activeList.push_back(back);
            } else {
                break;
            }
        }

        // Sort the activelist by X 
        sort(activeList.begin(), activeList.end(), sortByX);

        // sentinol to tell if we are currently entering the inside of the polygon 
        bool isInside = false;
        // Value of the last X on an edge
        int lastX = -1;
        // Go through the active list
        for (int o = 0; o < activeList.size(); o++) {
            // When we hit an edge swap whether we are inside or not
            isInside = !isInside;
            // If we are now entering the outside, draw a line from the 
            // last edge to our current point
            if (!isInside) {
                for (int tempX = lastX; tempX < activeList[o].x; tempX++) {
                    addPixel(tempX, currentY);
                }
            }
            lastX = activeList[o].x;

            // Update the X on the current point
            activeList[o].sum += activeList[o].dX;
            while (activeList[o].sum >= activeList[o].dY) {
                activeList[o].x += activeList[o].sign;
                activeList[o].sum -= activeList[o].dY;
            }
        }

        currentY++;
    }
}


