//
// Alternate fragment shader for 2D assignments.
//
// This will be used if the OpenGL implementation does not
// support OpenGL 3.2 and GLSL 1.50.
//

#version 120

// incoming color from the vertex shader
varying vec4 rescolor;

void main()
{
    gl_FragColor = rescolor;
}
