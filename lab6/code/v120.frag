#version 120

// Use this as the basis for each of your fragment shaders.
// Be sure to replace "SHADER" with the specific shader type
// and "YOUR_NAME_HERE" with your own name.

//
// SHADER fragment shader
//
// @author  RIT CS Department
// @author  YOUR_NAME_HERE
//

//////////////////////////////////////////////////////
// Add any incoming variables (from the application
// or from the vertex shader) here
//////////////////////////////////////////////////////

void main()
{
    //////////////////////////////////////////////////////
    // Add all illumination and shading code you need here
    //////////////////////////////////////////////////////

    // by default, just use 80% gray for everything
    gl_FragColor = vec4( 0.8, 0.8, 0.8, 1.0 );
}
