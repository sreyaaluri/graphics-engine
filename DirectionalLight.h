#ifndef __DIRECTIONALLIGHT_H__
#define __DIRECTIONALLIGHT_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <iostream>
#include <string>

// JSON
#include <nlohmann/json.hpp> 

// GL / GLM
#include "GLInclude.h"

////////////////////////////////////////////////////////////////////////////////

class DirectionalLight {
  private:
    glm::vec3 dir;
    glm::vec3 Ia;
    glm::vec3 Id;
    glm::vec3 Is;

  public:
    DirectionalLight(const nlohmann::json &l)
    : Ia(glm::vec3(l["Ia"][0],l["Ia"][1],l["Ia"][2])), 
      Id(glm::vec3(l["Id"][0],l["Id"][1],l["Id"][2])), 
      Is(glm::vec3(l["Is"][0],l["Is"][1],l["Is"][2])),
      dir(glm::normalize(glm::vec3(l["direction"][0],l["direction"][1],l["direction"][2])))
    {};

    const glm::vec3& getDir() const { return dir; };
    const glm::vec3& getIa()  const { return Ia; };
    const glm::vec3& getId()  const { return Id; };
    const glm::vec3& getIs()  const { return Is; };

    const glm::vec3 getDirToLight() const {
      return -dir;
    };

    const glm::vec3 computeAmbient(const glm::vec3& modelKa) const {
      return modelKa * Ia;
    }

    const glm::vec3 computeDiffuse(const glm::vec3& modelKd, const glm::vec3& n) const {
      return modelKd*Id*max(0.f, glm::dot(n,-dir));
    }

    const glm::vec3 computeSpecular(const glm::vec3& modelKs, float p, const glm::vec3& n, const glm::vec3& v) const {
      glm::vec3 r = glm::normalize(v-dir);
      return modelKs*Is*max(0.f, pow(glm::dot(n,r), p));
    }
    
    void sendUniforms(GLuint program, int i) const {
      std::string dirString = "dirLights[" + std::to_string(i) + "].dir";
      GLint dir_location  = glGetUniformLocation(program, dirString.c_str());

      std::string IaString = "dirLights[" + std::to_string(i) + "].Ia";
      GLint Ia_location = glGetUniformLocation(program, IaString.c_str());

      std::string IdString = "dirLights[" + std::to_string(i) + "].Id";
      GLint Id_location = glGetUniformLocation(program, IdString.c_str());

      std::string IsString = "dirLights[" + std::to_string(i) + "].Is";
      GLint Is_location = glGetUniformLocation(program, IsString.c_str());

      glUniform3fv(dir_location, 1, glm::value_ptr(dir));
      glUniform3fv(Ia_location, 1, glm::value_ptr(Ia));
      glUniform3fv(Id_location, 1, glm::value_ptr(Id));
      glUniform3fv(Is_location, 1, glm::value_ptr(Is));
    }
};

#endif


    // {
    //   "type": "point",
    //   "position": [-10,-10,20], 
    //   "Ia": [0.06,0.06,0.06], 
    //   "Id": [0.7,0.7,0.7], 
    //   "Is": [1,1,1],
    //   "ac": 1,
    //   "al": 0.1,
    //   "aq": 0.01
    // }