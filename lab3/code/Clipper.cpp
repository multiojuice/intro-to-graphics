//
//  Clipper.cpp
//
//  Simple module that performs clipping
//
//  Created by Warren R. Carithers on 01/15/14.
//  Updates: 2019/02/16, 2019/09/17, 2021/09/23 by wrc.
//  Based on a C++ version created by Joe Geigel on 11/30/11.
//  Copyright 2021 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  Owen Sullivan
//

// Includs
#include "Types.h"
#include "Clipper.h"
#include <iostream>
#include <vector>

using namespace std;

/**
 * getIntersect takes 2 points and a straight line x=x | y=y 
 * and gets the intersect between the two lines.
 */
Vertex getIntersect(Vertex p1, Vertex p2, Vertex e1, Vertex e2) {
    Vertex v;

    // Get slope
    float slope;
    // dont divide by 0
    if (p1.x == p2.x) {
        slope = 0;
    } else {
        // Get slope
        slope = (p2.y - p1.y) / (p2.x - p1.x);
    }
    // Get b
    float b = p1.y - slope*p1.x;

    // vertical
    if (e1.x == e2.x) {
        // set x to equal the edge wall
        v.x = e1.x;
        // y = mx+b
        v.y = slope * e1.x + b;
    // horizontal
    } else if (e1.y == e2.y) {
        // set y to horizontal
        v.y = e1.y;
        if (p2.x == p1.x) {
            // Make sure slope isnt infinity
            v.x = p1.x;
        } else {
            v.x = (v.y - b) / slope;
        }
    }

    return v;
}

/**
 * insideEdge takes in a point and a straight edge
 * then it returns a boolean confirming if it is 
 * within the clipping rectangle.
 * The reason this works is because I rotate the edges in a clockwise from ur
 * and look backwards (so ur to ul).
 */
bool insideEdge(Vertex a, Vertex e1, Vertex e2) { 
    // vertical
    if (e1.x == e2.x) {
        //right edge
        if (e1.y < e2.y) {
            return a.x <= e1.x;
        // Left edge
        } else {
            return a.x >= e1.x;
        }
    // horizontal
    } else if (e1.y == e2.y) {
        // Bottom Edge
        if (e1.x < e2.x) {
            return a.y >= e1.y;
        // Top edge
        } else {
            return a.y <= e1.y;
        }
    }

    // impossible but used in debugging process
    return false;
}

/**
 * circ is used as a helper for a 
 * circular index so when 0 wants to look back
 * it gets n-1 rather than -1
 */
int circ(int j, int n) {
    return (j == 0 ? n - 1 : j - 1);
}

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
                 Vertex ll, Vertex ur ) {


    // set lower right
    Vertex lr;
    lr.x = ur.x;
    lr.y = ll.y;
    // set upper left
    Vertex ul;
    ul.x = ll.x;
    ul.y = ur.y;

    // clockwise set of edges
    Vertex edges[] = {ur, lr, ll, ul};

    // Input vector and output vector
    vector<Vertex> inVec;
    vector<Vertex> outVec;
    
    // set intial output vector
    for (int i = 0; i < num; i++) {
        outVec.push_back(inV[i]);
    }

    // Start algorithm per edge
    for (int j = 0; j < 4; j++) {
        // make input vector the previous output vector
        inVec = outVec;
        outVec.clear();

        // Check each remaining edge
        for (int i = 0; i < inVec.size(); i++) {
            // get the intersect
            Vertex intersect = getIntersect(inVec[i], inVec[circ(i, inVec.size())], edges[j], edges[circ(j, 4)]);

            // if i is within the edge
            if (insideEdge(inVec[i], edges[j], edges[circ(j, 4)])) {
                // if last edge wasnt in, add the intersect
                if (!insideEdge(inVec[circ(i, inVec.size())], edges[j], edges[circ(j, 4)])) {
                    outVec.push_back(intersect);
                }
                // add the current point
                outVec.push_back(inVec[i]);

            // if the previous point was inside, then intersect the other
            } else if (insideEdge(inVec[circ(i, inVec.size())], edges[j], edges[circ(j, 4)])) {
                outVec.push_back(intersect);
            }
        }
    }

    // convert output vector to array
    int count = 0;
    for (int i = 0; i < outVec.size(); i++) {
        outV[i] = outVec[i];
        count++;
    }

    return( count );  // remember to return the outgoing vertex count!
}
