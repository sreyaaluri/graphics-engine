////////////////////////////////////////////////////////////////////////////////
// Simple pass through vertex shader, i.e., do not alter anything about the
// position, simply set the required gl data.
////////////////////////////////////////////////////////////////////////////////

#version 330

in vec3 norm_viewSpace;      // Input vertex normal in view space
in vec3 vPos_viewSpace;      // Input vertex position in view space
in vec3 lDir;                // Input vector pointing to light from vertex
out vec4 fcolor;             // Output fragment color

struct PositionalLight {
  vec3 Ia;
  vec3 Id;
  vec3 Is;
  vec3 pos;
};

struct Material {
  vec3 Ka;
  vec3 Kd;
  vec3 Ks;
  float p;
};

uniform PositionalLight light;
uniform Material mtl;
uniform mat4 MV;
uniform mat4 MV_inv_T;
uniform mat4 Proj;
uniform mat4 View;

void main() {
  //lighting computations are not done for wireframes
  vec3 ambient  = mtl.Kd * light.Id;
  fcolor = vec4(ambient, 1.0);
}
