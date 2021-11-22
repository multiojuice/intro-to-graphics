#version 150
//
// phong fragment shader
//
// @author  RIT CS Department
// @author  Owen Sullivan
//

// Incoming variables
// Shape data
uniform vec4 shape_ambient;
uniform float shape_amb_ref_coef;
uniform vec4 shape_diffuse_color;
uniform float shape_diff_ref_coef;
uniform vec4 shape_specular_color;
uniform float shape_spec_expo;
uniform float shape_spec_ref_coef;

// Light and amb colors
uniform vec4 light_color;
uniform vec4 amb_color;

// Incoming calculated vectors for light equations
in vec3 N;
in vec3 L;
in vec3 R;
in vec3 V;


// Outgoing fragment color
out vec4 fragColor;


void main()
{
    // ambient light totaled from A = (I_a)(k_a)
    vec4 A = shape_ambient * shape_amb_ref_coef * amb_color;

    // diffuse light totaled from D = (I_d)(k_d) * (L . N)
    vec4 D = shape_diffuse_color * shape_diff_ref_coef * (max(dot(L, N), 0));

    // specular light total from S = (I_s)(k_s) (cos(a)^n) -> (I_s)(k_s) ((V . R)^n)
    // since normalized we dont need to div by the norms of V and R
    vec4 S = shape_specular_color * shape_spec_ref_coef * pow(max(dot(V,R), 0), shape_spec_expo);

    // Outbound color A + fD + fS -- unsure if i should color specular but it looks better
    vec4 phongColor = A + (light_color * D) + (light_color * S);

    fragColor = phongColor;
}
