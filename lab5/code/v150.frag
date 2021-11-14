//
// Tessellation assignment fragment shader
//

#version 150

// outgoing color to the rest of the pipeline
out vec4 fragColor;

void main()
{
    // P1 Phosphor screen (Green monochrome) effect
    // fragColor = vec4( 0.0, 1.0, 0.0, 1.0 );

    // P2 Phosphor screen (Blue - green monochrome) effect
    // fragColor = vec4( 0.0, 1.0, 0.8, 1.0 );

    // P3 Phosphor screen (Amber monochrome) effect
    fragColor = vec4( 1.0, 0.749, 0.0, 1.0 );

    // P4 Phosphor screen (White monochrome) effect
    // fragColor = vec4( 1.0, 1.0, 1.0, 1.0 );

    // P5 Phosphor screen (Black monochrome) effect
    // fragColor = vec4( 0.0, 0.0, 0.0, 1.0 );
}
