#version 150

// Use this as the basis for each of your vertex shaders.
// Be sure to replace "SHADER" with the specific shader type
// and "YOUR_NAME_HERE" with your own name.

//
// SHADER vertex shader
//
// @author  RIT CS Department
// @author  YOUR_NAME_HERE
//

//
// Vertex attributes
//

// Vertex location (in model space)
in vec4 vPosition;

// Normal vector at vertex (in model space)
in vec3 vNormal;

//
// Uniform data
//

// Transformations
uniform mat4 viewMat;  // view (camera)
uniform mat4 projMat;  // projection

// Model transformation matrices
uniform mat4 modelMat; // composite

//////////////////////////////////////////////////////
// Add any additional incoming variables you need here
//////////////////////////////////////////////////////

//////////////////////////////////////////////////////
// Add any outgoing variables you need here
//////////////////////////////////////////////////////

void main()
{
    // create the modelview matrix
    mat4 modelViewMat = viewMat * modelMat;

    //////////////////////////////////////////////////////
    // Add all illumination and shading code you need here
    //////////////////////////////////////////////////////

// *************************************************************
// THE FOLLOWING CODE IS HERE AS A PLACEHOLDER
//
// Because this skeletal vertex shader doesn't actually use
// the vNormal attribute, the GLSL compiler will optimize it
// out, and the application will generate "unknown attribute"
// error messages whenever vNormal is enabled when drawing the
// objects.  This code makes it look like vNormal is being used,
// so the GLSL compiler won't eliminate it.
//
// REMOVE THESE STATEMENTS ONCE YOU ADD YOUR CODE THAT
// ACTUALLY USES vNormal
// *************************************************************
    vec3 nvec = normalize( vNormal );
    float nvm = length( nvec );

    // send the vertex position into clip space
// *************************************************************
// MODIFY THIS STATEMENT BY REMOVING THE 'nvm' TERM
// ONCE YOU HAVE ADDED YOUR CODE THAT USES vNormal
// *************************************************************
    gl_Position =  projMat * modelViewMat * nvm * vPosition;
}
