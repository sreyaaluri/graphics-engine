#version 330

in vec3 norm_projSpace;      // Input fragment normal in view space
in vec3 pos_projSpace;       // Input fragment position in view space
in vec2 textCoord;           // Input fragment texture coordinate
out vec4 fcolor;             // Output fragment color

struct PositionalLight {
  bool spot;
  vec3 Ia;
  vec3 Id;
  vec3 Is;
  vec3 pos;
  vec3 dir;
  float aa;
  float ac;
  float al;
  float aq;
  float cosTheta;
};

struct DirectionalLight {
  vec3 Ia;
  vec3 Id;
  vec3 Is;
  vec3 dir;
};

struct Material {
  bool hasMapKd;
  bool hasMapKs;
  vec3 Ka;
  vec3 Kd;
  vec3 Ks;
  float p;
  sampler2D texSampler;
  sampler2D specSampler;
};

const int LightCount = 8;
uniform PositionalLight posLights[LightCount];
uniform DirectionalLight dirLights[LightCount];
uniform Material mtl;
uniform mat4 View;
vec3 Kd = vec3(0,0,0);
vec3 Ks = vec3(0,0,0);

vec3 getColorPerPosLight(int i) {
  //calculating required variables
  vec3 lPos_projSpace = (View * vec4(posLights[i].pos, 1.0)).xyz;
  float dist = length(lPos_projSpace - pos_projSpace);                 // distance between light and fragment
  vec3 L = normalize(lPos_projSpace - pos_projSpace);                  // direction towards light
  vec3 N = normalize(norm_projSpace);                                  // normal
  vec3 V = normalize(-pos_projSpace);                                  // view vector (would be equivalent to -vertexPos in view Space (because camera eye is origin))
  vec3 R = reflect(-L,N);                                              // reflection of -L about N
  float linear =  1.0 / (1 + posLights[i].al * dist + posLights[i].aq * dist * dist);
  float angular = pow(dot(-L, posLights[i].dir), posLights[i].aa);
  if(!posLights[i].spot) angular = 1;
  float alpha = linear * angular;                                      // attenuation

  //computing ADS lighting
  vec3 ambient  = mtl.Ka * posLights[i].Ia;

  vec3 diffuse = vec3(0,0,0);
  if( !(posLights[i].spot && dot(-L,posLights[i].dir) < posLights[i].cosTheta) ) {
    diffuse =  alpha * Kd * posLights[i].Id * max(0.0, dot(N,L));
  }

  vec3 specular = vec3(0,0,0);
  if( !(posLights[i].spot && dot(-L,posLights[i].dir) < posLights[i].cosTheta) ) {
    specular = alpha * Ks * posLights[i].Is * pow(max(0.0, dot(V,R)), mtl.p);
  }
  
  return ambient+diffuse+specular;
}

vec3 getColorPerDirLight(int i) {
  //calculating required variables
  vec3 L = normalize(-dirLights[i].dir);                              // direction towards light
  vec3 N = normalize(norm_projSpace);                                  // normal
  vec3 V = normalize(-pos_projSpace);                                  // view vector (would be equivalent to -vertexPos in view Space (because camera eye is origin))
  vec3 R = reflect(-L,N);                                              // reflection of -L about N

  //computing ADS lighting
  vec3 ambient = mtl.Ka * dirLights[i].Ia;
  vec3 diffuse = Kd * dirLights[i].Id * max(0.0, dot(N,L));
  vec3 specular = Ks * dirLights[i].Is * pow(max(0.0, dot(V,R)), mtl.p);

  return ambient+diffuse+specular;
}

vec4 getColor() {
  //getting values from texture maps, if any
  if(mtl.hasMapKd) Kd = texture(mtl.texSampler, textCoord).xyz;
  else Kd = mtl.Kd;

  if(mtl.hasMapKs) Ks = texture(mtl.specSampler, textCoord).xyz;
  else Ks = mtl.Ks;

  vec3 col = vec3(0,0,0);
  for(int i=0; i<LightCount; i++){                // lighting computations for positional lights
    col += getColorPerPosLight(i);
  }
  for(int i=0; i<LightCount; i++){
    col += getColorPerDirLight(i);                // lighting computations for directional lights
  }
  vec4 color = clamp( vec4(col,1.f), 0, 1);       // keeping the color between 0 and 1
  return color;
}

void main() {
  fcolor =  getColor();
}
