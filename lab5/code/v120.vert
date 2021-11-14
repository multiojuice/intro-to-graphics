//
// This is an alternate version of the standard fragment shader for the
// tessellation assignment in this course.
//
// Performs a rotation transformation on the vertex.
//

#version 120

// incoming vertex attribute
attribute vec4 vPosition;

// rotation angles in all three axes
uniform vec3 theta;

void main()
{
    // Compute the sines and cosines of each rotation about each axis
    vec3 angles = radians( theta );
    vec3 c = cos( angles );
    vec3 s = sin( angles );

    // rotation matrices
    mat4 rx = mat4 ( 1.0,  0.0,  0.0,  0.0,
                     0.0,  c.x,  s.x,  0.0,
                     0.0, -s.x,  c.x,  0.0,
                     0.0,  0.0,  0.0,  1.0 );

    mat4 ry = mat4 ( c.y,  0.0, -s.y,  0.0,
                     0.0,  1.0,  0.0,  0.0,
                     s.y,  0.0,  c.y,  0.0,
                     0.0,  0.0,  0.0,  1.0 );

    mat4 rz = mat4 ( c.z,  s.z,  0.0,  0.0,
                    -s.z,  c.z,  0.0,  0.0,
                     0.0,  0.0,  1.0,  0.0,
                     0.0,  0.0,  0.0,  1.0 );

    // rotate around x, then around y, then around z
    gl_Position = rz * ry * rx * vPosition;
}
