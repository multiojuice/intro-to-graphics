//
// Fragment shader for the transformation assignment.
//
// Author:  Warren R. Carithers
//
// This file should not be modified by students.
//

#version 150

out vec4 fragColor;

void main()
{
    // Phosphor screen (Amber monochrome) effect
    fragColor = vec4( 1.0, 0.749, 0.0, 1.0 );
}
