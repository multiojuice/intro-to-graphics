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

// Shape data
uniform vec4 shape_ambient;
uniform float shape_amb_ref_coef;
uniform vec4 shape_diffuse_color;
uniform float shape_diff_ref_coef;
uniform vec4 shape_specular_color;
uniform float shape_spec_expo;
uniform float shape_spec_ref_coef;

// Light info
uniform vec4 light_color;
uniform vec4 light_position;

// Ambient light info
uniform vec4 amb_color;


// Transformations
uniform mat4 viewMat;  // view (camera)
uniform mat4 projMat;  // projection

// Model transformation matrices
uniform mat4 modelMat; // composite

// Outbound color
out vec4 gouraudColor;

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
    vec3 N = normalize(vertexNorm); 
    vec3 L = normalize(newLight - vertexPos);
    vec3 R = normalize (reflect(L, N)); 
    vec3 V = normalize (vertexPos); 

    // ambient light totaled from A = (I_a)(k_a)
    vec4 A = shape_ambient * shape_amb_ref_coef * amb_color;

    // diffuse light totaled from D = (I_d)(k_d) * (L . N)
    vec4 D = shape_diffuse_color * shape_diff_ref_coef * (max(dot(L, N), 0));

    // specular light total from S = (I_s)(k_s) (cos(a)^n) -> (I_s)(k_s) ((V . R)^n)
    // since normalized we dont need to div by the norms of V and R
    vec4 S = shape_specular_color * shape_spec_ref_coef * pow(max(dot(V,R), 0), shape_spec_expo);

    // Outbound color A + fD + S
    gouraudColor = A + (light_color * D) + (light_color * S);

    // send the vertex position into clip space
    gl_Position =  projMat * modelViewMat * vPosition;
}
