//
// Alternate vertex shader for 2D assignments.
//
// This will be used if the OpenGL implementation does not
// support OpenGL 3.2 and GLSL 1.50.
//
// Performs a normalization transformation on the vertex.
//

#version 120

// incoming vertex attributes
attribute vec4 vPosition;
attribute vec4 vColor;

// scale factors for normalization
uniform vec2 sf;

// outgoing color sent to the fragment shader
varying vec4 rescolor;

void main()
{
    // normalize the location in (x,y)
    float x = vPosition.x * sf.x - 1.0;
    float y = vPosition.y * sf.y - 1.0;

    vec4 newvert = vec4( x, y, vPosition.z, vPosition.w );

    gl_Position = newvert;
    rescolor = vColor;
}
