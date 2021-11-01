//
// Vertex shader for the transformation assignment, GLSL 1.50
//
// Created by Warren R. Carithers 2021/10/24.
//
// Contributor:  YOUR_NAME_HERE
//

#version 150

// attribute:  vertex position
in vec4 vPosition;

// add other global shader variables to hold the
// parameters your application code is providing

void main()
{
    // By default, no transformations are performed.
    gl_Position =  vPosition;
}
