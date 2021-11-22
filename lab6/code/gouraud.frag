#version 150
//
// gouraud fragment shader
//
// @author  RIT CS Department
// @author  Owen Sullivan
//

// Incoming variables
in vec4 gouraudColor;

// Outgoing fragment color
out vec4 fragColor;

void main()
{
    // simply set the calculated color
    fragColor = gouraudColor;
}
