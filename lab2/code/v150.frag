//
// Standard fragment shader for 2D assignments.
//

#version 150

// incoming color from the vertex shader
in vec4 rescolor;

// outgoing color to the rest of the pipeline
out vec4 fragmentColor;

void main()
{
    fragmentColor = rescolor;
}
