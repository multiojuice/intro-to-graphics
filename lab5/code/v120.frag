//
// This is an alternate version of the standard fragment shader for the
// tessellation assignment in this course.
//

#version 120

void main()
{
    // P1 Phosphor screen (Green monochrome) effect
    // gl_FragColor = vec4( 0.0, 1.0, 0.0, 1.0 );

    // P2 Phosphor screen (Blue - green monochrome) effect
    // gl_FragColor = vec4( 0.0, 1.0, 0.8, 1.0 );

    // P3 Phosphor screen (Amber monochrome) effect
    gl_FragColor = vec4( 1.0, 0.749, 0.0, 1.0 );

    // P4 Phosphor screen (White monochrome) effect
    // gl_FragColor = vec4( 1.0, 1.0, 1.0, 1.0 );

    // P5 Phosphor screen (Black monochrome) effect
    // gl_FragColor = vec4( 0.0, 0.0, 0.0, 1.0 );
}
