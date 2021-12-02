#version 150

// Use this as the basis for each of your fragment shaders.
// Be sure to replace "SHADER" with the specific shader type
// and "YOUR_NAME_HERE" with your own name.

//
// Texture fragment shader
//
// @author  RIT CS Department
// @author  Owen Sullivan
//

//
// INCOMING DATA
//

//
// Data coming from the vertex shader
//

// Light position
in vec3 lPos;

// Vertex position (in clip space)
in vec3 vPos;

// Vertex normal
in vec3 vNorm;

//
// ADD ANY INCOMING VARIABLES FROM THE APPLICATION HERE
//
in vec2 vTexCoord;

//
// Data coming from the application
//

// Light color
uniform vec4 lightColor;
uniform vec4 ambientLight;

// Material properties
uniform vec4 diffuseColor;
uniform vec4 ambientColor;
uniform vec4 specularColor;
uniform float specExp;
uniform vec3 kCoeff;

//
// ADD ANY INCOMING VARIABLES FROM THE APPLICATION HERE
//

uniform sampler2D texfront;
uniform sampler2D texback;

// OUTGOING DATA

// The final fragment color
out vec4 fragColor;

void main()
{
    // calculate lighting vectors
    vec3 L = normalize( lPos - vPos );
    vec3 N = normalize( vNorm );
    vec3 R = normalize( reflect(-L, N) );
    vec3 V = normalize( -(vPos) );

    vec4 ambient  = vec4(0.0);  // ambient color component
    vec4 diffuse  = vec4(0.0);  // diffuse color component
    vec4 specular = vec4(0.0);  // specular color component
    float specDot;  // specular dot(R,V) ^ specExp value

	//
	// Add your texture mapping code here.  Be sure to set
	// the vec4 variables ambient, diffuse, and specular to
	// those contributions to the final color.
	//

	// Modify these statements to do the shading calculations
    // using your texture variables.
	ambient  = ambientLight * ambientColor;
    diffuse  = lightColor * diffuseColor * max(dot(N,L),0.0);
    specDot  = pow( max(dot(R,V),0.0), specExp );
    specular = lightColor * specularColor * specDot;

    if (gl_FrontFacing) {
        ambient  = ambientLight * texture(texfront, vTexCoord) * max(dot(N,L),0.0);
        diffuse  = lightColor * texture(texfront, vTexCoord);
        specDot  = pow( max(dot(R,V),0.0), specExp );
        specular = lightColor * texture(texfront, vTexCoord) * specDot;

    } else {
        ambient  = ambientLight * texture(texback, vTexCoord) * max(dot(N,L),0.0);
        diffuse  = lightColor * texture(texback, vTexCoord);
        specDot  = pow( max(dot(R,V),0.0), specExp );
        specular = lightColor * texture(texback, vTexCoord) * specDot;
    }

    // calculate the final color
    vec4 color = (kCoeff.x * ambient) +
                 (kCoeff.y * diffuse) +
                 (kCoeff.z * specular);

    fragColor = color;
}
