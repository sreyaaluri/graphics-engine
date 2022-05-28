#ifndef __MATERIAL_H__
#define __MATERIAL_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

// GL / GLM
#include "GLInclude.h"

// SOIL
#include <SOIL2/SOIL2.h>

////////////////////////////////////////////////////////////////////////////////

class Material{
  private:
    const int TOTAL_NUM_MAPS = 2;
    GLuint hasMapKd = GL_FALSE;
    GLuint hasMapKs = GL_FALSE;
    std::string name;
    glm::vec3 Ka;
    glm::vec3 Kd;
    glm::vec3 Ks;
    GLboolean map_Kd;
    GLboolean map_Ks;
    float p;
    std::string texturePath = "";
    std::string specularPath = "";
    GLuint loadTexture(const char *texImagePath);

  public:
    Material(const std::string& infileName = "");
    Material(glm::vec3 Ka, glm::vec3 Kd, glm::vec3 Ks, float p, std::string name = "") : Ka(Ka), Kd(Kd), Ks(Ks), p(p), name(name) {};

    const std::string& getName() const { return name; };
    const glm::vec3& getKa() const { return Ka; };
    const glm::vec3& getKd() const { return Kd; };
    const glm::vec3& getKs() const { return Ks; };
    float getP() const { return p; };

    void sendUniforms(GLuint program) const;
    void initialize();
};

#endif