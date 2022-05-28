#version 330
layout(location = 0) in vec4 vPos_modelSpace;   // Input vertex position from data
layout(location = 1) in vec4 vNorm_modelSpace;  // Input vertex normal from data
layout(location = 2) in vec2 vTextCoord;        // Input vertex texture coordinate from data
out vec3 norm_projSpace;      // Output vertex normal in proj space
out vec3 pos_projSpace;       // Output vertex position in proj space
out vec2 textCoord;           // Pass along vertex texture coordinate

uniform mat4 MV;
uniform mat4 MV_inv_T;
uniform mat4 Proj;

void main() {
  pos_projSpace = ( MV * vPos_modelSpace).xyz;
  norm_projSpace = normalize(MV_inv_T * vNorm_modelSpace).xyz;
  gl_Position = Proj * MV * vPos_modelSpace;
  textCoord = vTextCoord;
}