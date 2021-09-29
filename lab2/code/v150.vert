//
// Vertex shader for 2D assignments.
//
// Performans a normalization transformation on the vertex.
//

#version 150

// incoming vertex attributes
in vec4 vPosition;
in vec4 vColor;

// scale factors for normalization
uniform vec2 sf;

// outgoing color sent to the fragment shader
out vec4 rescolor;

void main()
{
    // normalize the location in (x,y)
    float x = vPosition.x * sf.x - 1.0;
    float y = vPosition.y * sf.y - 1.0;

    vec4 newvert = vec4( x, y, vPosition.z, vPosition.w );

    gl_Position = newvert;
    rescolor = vColor;
}
