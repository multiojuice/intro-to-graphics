#version 150

// Use this as the basis for each of your vertex shaders.
// Be sure to replace "SHADER" with the specific shader type
// and "YOUR_NAME_HERE" with your own name.

//
// Texture vertex shader
//
// @author  RIT CS Department
// @author  Owen Sullivan
//

//
// Vertex attributes
//

// Vertex position (in model space)
in vec4 vPosition;

// Normal vector at vertex (in model space)
in vec3 vNormal;

// Texture coordinate for this vertex
in vec2 vTexCoord;

//
// Uniform data
//

// Camera and projection matrices
uniform mat4 viewMat;   // view (camera)
uniform mat4 projMat;   // projection

uniform mat4 modelMat;  // composite

// Light position is given in world space
uniform vec4 lightPosition;

// OUTGOING DATA

// Vectors to "attach" to vertex and get sent to fragment shader
// Vectors and points will be passed in "eye" space
out vec3 lPos;
out vec3 vPos;
out vec3 vNorm;

// ADD ANY OUTGOING VARIABLES YOU NEED HERE

void main()
{
    // create the modelview matrix
    mat4 modelViewMat = viewMat * modelMat;

    // All vectors need to be converted to "eye" space
    // All vectors should also be normalized
    vec4 vertexInEye = modelViewMat * vPosition;
    vec4 lightInEye = viewMat * lightPosition;

    // "Correct" way to transform normals.  The normal matrix is the
    // inverse transpose of the upper left 3x3 submatrix of the modelView
    // matrix (i.e., does not include translations).  THIS IS EXPENSIVE
    // TO COMPUTE because of the inverse(), and should really be done in
    // the application, not here.
    mat3 normMat = inverse( transpose( mat3(modelViewMat) ) );
    vec4 normalInEye = vec4( normMat * vNormal, 0.0 );

    // pass our vertex data to the fragment shader
    lPos = lightInEye.xyz;
    vPos = vertexInEye.xyz;
    vNorm = normalInEye.xyz;

// *************************************************************
// THE FOLLOWING CODE IS HERE AS A PLACEHOLDER
//
// Because this skeletal vertex shader doesn't actually use
// the vTexCoord attribute, the GLSL compiler will optimize it
// out, and the application will generate "unknown attribute"
// error messages whenever vTexCoord is enabled when drawing the
// objects.  This code makes it look like vTexCoord is being used,
// so the GLSL compiler won't eliminate it.
//
// REMOVE THESE STATEMENTS ONCE YOU ADD YOUR CODE THAT
// ACTUALLY USES vTexCoord
// *************************************************************
    vec2 nvec = normalize( vTexCoord );
    float nvm = length( nvec );

    // send the vertex position into clip space
// *************************************************************
// MODIFY THIS STATEMENT BY REMOVING THE 'nvm' TERM
// ONCE YOU HAVE ADDED YOUR CODE THAT USES vTexCoord
// *************************************************************
    gl_Position =  projMat * modelViewMat * nvm * vPosition;

    //
    // Add any code you need to implement texture mapping here.
    //
}
