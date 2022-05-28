#version 330

layout(location = 0) in vec3   pPos_modelSpace;  // Input point position from data
layout(location = 1) in vec4   pcolor;           // Input point color from data
layout(location = 2) in float  pmass;            // Input point mass from data
out vec4  color;   // Passalong color to fragment shader

uniform mat4 MVP;

void main() {
  gl_Position = MVP * vec4(pPos_modelSpace, 1);
  gl_PointSize =  pmass;
  color = pcolor;
}
