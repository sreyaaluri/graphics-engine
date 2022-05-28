#ifndef __MATERIAL_CPP__
#define __MATERIAL_CPP__
#include "Material.h"

using namespace std;

Material::Material(const std::string& infileName) {

  std::cout << "Parsing: " << infileName << std::endl;

  std::ifstream ifs(infileName);

  std::string line;
  while(getline(ifs, line)) {
    std::istringstream iss(line);
    std::string tag;
    iss >> tag;
    if(tag == "newmtl") {
      iss >> name;
    }
    if(tag == "Ka") {
      iss >> Ka.x >> Ka.y >> Ka.z;
    }
    if(tag == "Kd") {
      iss >> Kd.x >> Kd.y >> Kd.z;
    }
    else if(tag == "Ks") {
      iss >> Ks.x >> Ks.y >> Ks.z;
    }
    else if(tag == "p") {
      iss >> p;
    }
    else if(tag == "map_Kd") {
      string path;
      iss >> texturePath;
    }
    else if(tag == "map_Ks") {
      string path;
      iss >> specularPath;
    }
  }
}

void Material::sendUniforms(GLuint program) const {
  GLint hasMapKd_loc = glGetUniformLocation(program, "mtl.hasMapKd");
  GLint hasMapKs_loc = glGetUniformLocation(program, "mtl.hasMapKs");
  GLint ka_location = glGetUniformLocation(program, "mtl.Ka");
  GLint kd_location = glGetUniformLocation(program, "mtl.Kd");
  GLint ks_location = glGetUniformLocation(program, "mtl.Ks");
  GLint p_location  = glGetUniformLocation(program, "mtl.p");
  glUniform1i(hasMapKd_loc, hasMapKd);
  glUniform1i(hasMapKs_loc, hasMapKs);
  glUniform3fv(ka_location, 1, glm::value_ptr(Ka));
  glUniform3fv(kd_location, 1, glm::value_ptr(Kd));
  glUniform3fv(ks_location, 1, glm::value_ptr(Ks));
  glUniform1f(p_location, p);
  
  if(hasMapKd) {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, map_Kd);
    GLuint texSamplerLoc = glGetUniformLocation(program, "mtl.texSampler");
    glUniform1i(texSamplerLoc, 0);
  }
  if(hasMapKs) {
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, map_Ks);
    GLuint specSamplerLoc = glGetUniformLocation(program, "mtl.specSampler");
    glUniform1i(specSamplerLoc, 1);
  }
};

GLuint Material::loadTexture(const char *texImagePath) {
  GLuint textureID;
  textureID = SOIL_load_OGL_texture(texImagePath, SOIL_LOAD_AUTO, SOIL_CREATE_NEW_ID, SOIL_FLAG_INVERT_Y);
  if (textureID == 0) cout << "could not find texture file" << texImagePath << endl; 
  return textureID;
};

void Material::initialize(){
  //loading texture map
  if(texturePath != "") {
    map_Kd = loadTexture(texturePath.c_str());
    if(map_Kd != 0) hasMapKd = GL_TRUE;
  }
  //loading specular map
  if(specularPath != "") {
    map_Ks = loadTexture(specularPath.c_str());
    if(map_Ks != 0) hasMapKs = GL_TRUE;
  }
};

#endif