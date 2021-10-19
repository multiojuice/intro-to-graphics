//
//  Types.h
//
//  Supporting data types for various assignments.
//
//  Author:  Warren R. Carithers
//  Date:    2016/10/07 12:34:54
//

#ifndef TYPES_H_
#define TYPES_H_

//
// Information pertaining to a color
//

typedef struct st_color {
    float r;
    float g;
    float b;
    float a;
} Color;

//
// Information pertaining to a normal
//

typedef struct st_norm {
    float x;
    float y;
    float z;
} Normal;

//
// Texture coordinate data.
//

typedef struct st_texcoord {
    float u;
    float v;
} TexCoord;


//
// Information pertaining to a vertex
//
// For 2D assignments, we ignore the 'z' and 'w' components.
//

typedef struct st_vertex {
    float x;
    float y;
    float z;
    float w;
} Vertex;

#endif
