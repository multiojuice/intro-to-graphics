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
// Camera information
uniform vec3 eye;
uniform vec3 lookat;
uniform vec3 up;

// Transformation information
uniform vec3 scale;
uniform vec3 translate;
uniform vec3 rotate;

// View and bounds information
uniform float left;
uniform float right;
uniform float top;
uniform float bottom;
uniform float near;
uniform float far;
uniform float viewmode;

// add other global shader variables to hold the
// parameters your application code is providing


/**
* createCameraMatrix creates a camera transformation matrix
* based on the global camera info
*/
mat4 createCameraMatrix() {
    vec3 n = normalize(eye - lookat);
    vec3 u = normalize(cross(normalize(up), n));
    vec3 v = normalize(cross(n, u));

    mat4 cameraMatrix = mat4(
        u[0], v[0], n[0], 0,
        u[1], v[1], n[1], 0,
        u[2], v[2], n[2], 0,
        -1*(dot(u, eye)), -1*(dot(v, eye)), -1*(dot(n, eye)), 1
    );

    return cameraMatrix;
}

/**
* createFrustumMatrix creates a frustum view transformation matrix
* based on the global bounds
*/
mat4 createFrustumMatrix() {
    return mat4(
        (2*near)/(right-left), 0, 0, 0,
        0, (2*near)/(top - bottom), 0, 0,
        (right+left)/(right-left), (top + bottom)/(top - bottom), -1*(far + near)/(far-near), -1,
        0, 0, (-2*near*far)/(far-near), 0
    );
}


/**
* createOrthoMatrix creates a ortho view transformation matrix
* based on the global bounds
*/
mat4 createOrthoMatrix() {
    return mat4(
        (2)/(right-left), 0, 0, 0,
        0, (2)/(top - bottom), 0, 0,
        0, 0, -2/(far-near), 0,
        -1*(right+left)/(right-left), -1*(top + bottom)/(top - bottom), -1*(far + near)/(far-near), 1
    );
}

/**
* createProjectionMatrix creates view transformation
* based on the viewmode if it is frustum or ortho
*/
mat4 createProjectionMatrix() {
    return viewmode == 1 ? createFrustumMatrix() : createOrthoMatrix();
}

/**
* createTransformationMatrix gets a combined transformation matrix
* that scales, rotates, and translates the model in order
*/
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

    mat4 rotate4z = mat4(
        cosOfZ, sinOfZ, 0, 0,
        -1*sinOfZ, cosOfZ, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    );

    mat4 rotate4y = mat4(
        cosOfY, 0, -1*sinOfY, 0,
        0, 1, 0, 0, 
        sinOfY, 0, cosOfY, 0, 
        0, 0, 0, 1
    );

    mat4 translate4 = mat4(
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        translate[0], translate[1], translate[2], 1
    );

    return translate4 * rotate4y * rotate4z * scale4;
}


/**
* main is ran by the program and creates each proj camera and trans
* matrix then applies them to the pixel
*/
void main()
{
    mat4 projectionMatrix = createProjectionMatrix();

    mat4 cameraMatrix = createCameraMatrix();

    mat4 transformationMatrix = createTransformationMatrix();

    // proj camera trans 
    gl_Position =  projectionMatrix * cameraMatrix * transformationMatrix * vPosition;
}
