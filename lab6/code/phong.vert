#version 150
//
// gouraud vertex shader
//
// @author  RIT CS Department
// @author  Owen Sullivan
//

// Vertex location (in model space)
in vec4 vPosition;

// Normal vector at vertex (in model space)
in vec3 vNormal;

//
// Uniform data
//

// Light info
uniform vec4 light_position;

// Transformations
uniform mat4 viewMat;  // view (camera)
uniform mat4 projMat;  // projection

// Model transformation matrices
uniform mat4 modelMat; // composite

// Outbound color
out vec3 N;
out vec3 L;
out vec3 R;
out vec3 V;

void main()
{
    // create the modelview matrix
    mat4 modelViewMat = viewMat * modelMat;

    // Calculate transformed position vectors based on
    // the view, cam, and model for vert
    vec3 vertexPos = (modelViewMat * vPosition).xyz;
    vec3 vertexNorm = (normalize(modelViewMat * vec4(vNormal,0.0))).xyz;
    vec3 newLight = (modelMat * light_position).xyz;

    // Create needed vecs from slides for calculatoions
    // And send to frag shader for phong
    N = normalize(vertexNorm); 
    L = normalize(newLight - vertexPos);
    R = normalize (reflect(L, N)); 
    V = normalize (vertexPos);

    // send the vertex position into clip space
    gl_Position =  projMat * modelViewMat * vPosition;
}
