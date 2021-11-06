// //
// // Vertex shader for the transformation assignment, GLSL 1.50
// //
// // Created by Warren R. Carithers 2021/10/24.
// //
// // Contributor:  Owen Sullivan
// //

// #version 150

// // attribute:  vertex position
// in vec4 vPosition;
// uniform vec3 eye;
// uniform vec3 lookat;
// uniform vec3 up;

// uniform float left;
// uniform float right;
// uniform float top;
// uniform float bottom;
// uniform float near;
// uniform float far;
// uniform float viewmode;

// // add other global shader variables to hold the
// // parameters your application code is providing


// mat4 createCameraMatrix() {
//     vec3 n = normalize(eye - lookat);
//     vec3 u = normalize(cross(normalize(up), n));
//     vec3 v = normalize(cross(n, u));

//     mat4 cameraMatrix = mat4(
//         u[0], u[1], u[2], -1*(dot(u, eye)),
//         v[0], v[1], v[2], -1*(dot(v, eye)),
//         n[0], n[1], n[2], -1*(dot(n, eye)),
//         0, 0, 0, 1
//     );

//     return cameraMatrix;
// }

// mat4 createFrustumMatrix() {
//         return mat4(
//         (2*near)/(right-left), 0, (right+left)/(right-left), 0,
//         0, (2*near)/(top - bottom), (top + bottom)/(top - bottom), 0,
//         0, 0, -1*(far + near)/(far-near), (-2*near*far)/(far-near),
//         0,0,-1,0
//     );
// }

// mat4 createOrthoMatrix() {
//     return mat4(
//         (2)/(right-left), 0, 0, -1*(right+left)/(right-left),
//         0, (2)/(top - bottom), 0, -1*(top + bottom)/(top - bottom),
//         0, 0, -2/(far-near), -1*(far + near)/(far-near),
//         0,0,0,1
//     );
// }



// mat4 createProjectionMatrix() {
//     return viewmode == 1 ? createFrustumMatrix() : createOrthoMatrix();
// }



// void main()
// {
//     mat4 projectionMatrix = createProjectionMatrix();

//     mat4 cameraMatrix = createCameraMatrix();

//     // proj camera trans 
//     gl_Position =   cameraMatrix * vPosition;
// }

//
// Vertex shader for the transformation assignment, GLSL 1.50
//
// Created by Warren R. Carithers 2021/10/24.
//
// Contributor:  Owen Sullivan
//

#version 150

// attribute:  vertex position
in vec4 vPosition;
uniform vec3 eye;
uniform vec3 lookat;
uniform vec3 up;

uniform vec3 scale;
uniform vec3 translate;
uniform vec3 rotate;

uniform float left;
uniform float right;
uniform float top;
uniform float bottom;
uniform float near;
uniform float far;
uniform float viewmode;

// add other global shader variables to hold the
// parameters your application code is providing


mat4 createCameraMatrix() {
    vec3 n = normalize(eye - lookat);
    vec3 u = normalize(cross(normalize(up), n));
    vec3 v = normalize(cross(n, u));

    mat4 cameraMatrix = transpose(mat4(
        vec4(u[0], u[1], u[2], -1*(dot(u, eye))),
        vec4(v[0], v[1], v[2], -1*(dot(v, eye))),
        vec4(n[0], n[1], n[2], -1*(dot(n, eye))),
        vec4(0, 0, 0, 1))
    );

    return cameraMatrix;
}

mat4 createFrustumMatrix() {
        return transpose(mat4(
        vec4((2*near)/(right-left), 0, (right+left)/(right-left), 0),
        vec4(0, (2*near)/(top - bottom), (top + bottom)/(top - bottom), 0),
        vec4(0, 0, -1*(far + near)/(far-near), (-2*near*far)/(far-near)),
        vec4(0,0,-1,0) 
    ));
}

mat4 createOrthoMatrix() {
    return transpose(mat4(
        (2)/(right-left), 0, 0, -1*(right+left)/(right-left),
        0, (2)/(top - bottom), 0, -1*(top + bottom)/(top - bottom),
        0, 0, -2/(far-near), -1*(far + near)/(far-near),
        0,0,0,1
    ));
}

mat4 createProjectionMatrix() {
    return viewmode == 1 ? createFrustumMatrix() : createOrthoMatrix();
}

mat4 createTransformationMatrix() {
    float pi = 3.1415;
    float zRad = rotate[2] * 0.01745;
    float sinOfZ = sin(zRad);
    float cosOfZ = cos(zRad);

    float yRad = rotate[1] * 0.01745;
    float sinOfY = sin(yRad);
    float cosOfY = cos(yRad);

    mat4 scale4 = transpose(mat4(
        scale[0], 0, 0, 0,
        0, scale[1], 0, 0,
        0, 0, scale[2], 0,
        0, 0, 0, 1)
    );

    mat4 rotate4z = transpose(mat4(
        cosOfZ, -1*sinOfZ, 0, 0,
        sinOfZ, cosOfZ, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1)
    );

    mat4 rotate4y = transpose(mat4(
        cosOfY, 0, sinOfY, 0,
        0, 1, 0, 0,
        -1*sinOfY, 0, cosOfY, 0,
        0, 0, 0, 1)
    );


    mat4 translate4 = transpose(mat4(
        1, 0, 0, translate[0],
        0, 1, 0, translate[1],
        0, 0, 1, translate[2],
        0, 0, 0, 1)
    );


    return translate4 * rotate4y * rotate4z * scale4;
}


void main()
{
    mat4 projectionMatrix = createProjectionMatrix();

    mat4 cameraMatrix = createCameraMatrix();

    mat4 transformationMatrix = createTransformationMatrix();


    // proj camera trans 
    gl_Position =  projectionMatrix * cameraMatrix * transformationMatrix * vPosition;
}
