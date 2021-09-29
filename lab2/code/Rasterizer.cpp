//
//  Rasterizer.cpp
//
//  Simple module that provides rasterization algorithms
//
//  Created by Joe Geigel on 11/30/11.
//  Updates: 2019/02/09, 2019/09/14 by wrc.
//  Copyright 2019 Rochester Institute of Technology. All rights reserved.
//
//  Contributor:  YOUR_NAME_HERE
//

#include <list>
#include <algorithm>
#include <iostream>

#include "Rasterizer.h"
#include "Canvas.h"
#include "Types.h"
#include "Utils.h"

using namespace std;

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

vector<EdgeBucket> initEdgeTable(int n, const Vertex v[]) {
    vector<EdgeBucket> edgeTable;

    std::cout << "Error ";

    Vertex a = v[n - 1];
    Vertex b = v[0];

    if (a.y != b.y) {
        EdgeBucket current;
        current.dX = abs(b.x - a.x);
        current.dY = abs(b.y - a.y);
        current.yMax = max(b.y, a.y);
        current.yMin = min(b.y, a.y);

        // TODO what happens if they are equal?? 
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
        edgeTable.push_back(current);
    }

    int id = 1;

    for (int i = 1; i < n; i++) {
        a = v[i - 1];
        b = v[i];

        if (a.y != b.y) {
            EdgeBucket current;
            current.dX = abs(b.x - a.x);
            current.dY = abs(b.y - a.y);
            current.yMax = max(b.y, a.y);
            current.yMin = min(b.y, a.y);

            // TODO what happens if they are equal?? 
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
            edgeTable.push_back(current);
        }
        id++;
    }

    return edgeTable;
}

bool sortByMinY(EdgeBucket a, EdgeBucket b) {
    return a.yMin > b.yMin;
}

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
    vector<EdgeBucket> edgeTable = initEdgeTable(n, v);
    sort(edgeTable.begin(), edgeTable.end(), sortByMinY);

    for( int i = 0; i < n; ++i ) {
        C.addPixel( v[i] );
    }

    int currentY = 0; 
    vector<EdgeBucket> activeList;
    while ((!edgeTable.empty() || !activeList.empty()) && currentY < 1000) {
        vector<EdgeBucket> tempList;
        
        for (EdgeBucket current : activeList) {
            if (currentY < current.yMax) {
                tempList.push_back(current);
            }
        }

        activeList = tempList;

        while (!edgeTable.empty()) {
            EdgeBucket back = edgeTable.back();
            if (currentY >= back.yMin) {
                edgeTable.pop_back();
                activeList.push_back(back);
            } else {
                break;
            }
        }

        sort(activeList.begin(), activeList.end(), sortByX);

        bool isInside = false;
        int lastX = -1;
        for (int o = 0; o < activeList.size(); o++) {
            isInside = !isInside;
            if (!isInside) {
                for (int tempX = lastX; tempX < activeList[o].x; tempX++) {
                    C.addPixel(tempX, currentY);
                }
            }
            lastX = activeList[o].x;

            activeList[o].sum += activeList[o].dX;
            while (activeList[o].sum >= activeList[o].dY) {
                activeList[o].x += activeList[o].sign;
                activeList[o].sum -= activeList[o].dY;
            }
        }

        currentY++;
    }
}


