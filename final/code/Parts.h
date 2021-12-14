//
// Parts.h
//
// Data structure containing information about individual parts of
// an object's mesh.
//
// If we want to draw the parts of an object independently, we need to
// know which vertices belong to the various components of the object
// (e.g., for a cylinder, the parts might be the top and bottom discs and
// the body of the cylinder).
//

#ifndef PARTS_H_
#define PARTS_H_

typedef
    struct parts_s {
        int nverts;   // number of vertices in this part
        int first;    // starting vertex/element index
        int last;     // ending vertex/element index
    } Parts;

#endif