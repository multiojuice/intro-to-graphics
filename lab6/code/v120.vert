#version 120

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
attribute vec4 vPosition;

// Normal vector at vertex (in model space)
attribute vec3 vNormal;

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

//
// Inversion function for 3x3 matrices by Mikola Lysenko.
// Origin: https://github.com/glslify/glsl-inverse/blob/master/index.glsl
//
// The MIT License (MIT)
//
// Copyright (c) 2014 Mikola Lysenko
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the "Software"),
// to deal in the Software without restriction, including without limitation
// the rights to use, copy, modify, merge, publish, distribute, sublicense,
// and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
// OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//

mat3 inverse(mat3 m) {
  float a00 = m[0][0], a01 = m[0][1], a02 = m[0][2];
  float a10 = m[1][0], a11 = m[1][1], a12 = m[1][2];
  float a20 = m[2][0], a21 = m[2][1], a22 = m[2][2];

  float b01 = a22 * a11 - a12 * a21;
  float b11 = -a22 * a10 + a12 * a20;
  float b21 = a21 * a10 - a11 * a20;

  float det = a00 * b01 + a01 * b11 + a02 * b21;

  return mat3(b01, (-a22 * a01 + a02 * a21), (a12 * a01 - a02 * a11),
              b11, (a22 * a00 - a02 * a20), (-a12 * a00 + a02 * a10),
              b21, (-a21 * a00 + a01 * a20), (a11 * a00 - a01 * a10)) / det;
}

void main()
{
    // create the modelview matries
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
