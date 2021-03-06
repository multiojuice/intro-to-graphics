//
//  Rasterizer.cpp
//
//  Simple module that provides rasterization algorithms
//
//  Created by Joe Geigel on 11/30/11.
//  Updates: 2019/02/09, 2019/09/14 by wrc.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Owen Sullivan
//

#include <list>
#include <algorithm>
#include <iostream>

#include "Rasterizer.h"
#include "Canvas.h"
#include "Types.h"
#include "Utils.h"

using namespace std;

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

///
/// Constructor
///
/// @param n number of scanlines
/// @param C The Canvas to use
///
Rasterizer::Rasterizer( int n, Canvas &canvas ) : n_scanlines(n), C(canvas)
{
}

/***********************
Added methods
***********************/
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
void Rasterizer::drawPolygon( int n, const Vertex v[] )
{
    // Generate the edge table
    vector<EdgeBucket> edgeTable = initEdgeTable(n, v);
    sort(edgeTable.begin(), edgeTable.end(), sortByMinY);

    // Keep plotting each pixel. Good for debugging and completion
    for( int i = 0; i < n; ++i ) {
        C.addPixel( v[i] );
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
                    C.addPixel(tempX, currentY);
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


