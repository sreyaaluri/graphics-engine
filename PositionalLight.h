#ifndef __POSITIONALLIGHT_H__
#define __POSITIONALLIGHT_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <iostream>
#include <algorithm>
#include <string>

// JSON
#include <nlohmann/json.hpp> 

// GL / GLM
#include "GLInclude.h"

using namespace std;

////////////////////////////////////////////////////////////////////////////////

class PositionalLight {
  private:
    GLuint spot = GL_FALSE;
    glm::vec3 pos;          // position
    glm::vec3 dir;          // direction
    float ac;               // linear attenuation constants
    float al;
    float aq;
    float aa;               // angular attenuation constant
    float theta;            // cut-off angle (in degrees)
    glm::vec3 Ia;           // ambient intensity
    glm::vec3 Id;           // diffuse intensity
    glm::vec3 Is;           // specular intensity



  public:
    PositionalLight(const nlohmann::json &l)
    : ac(l["ac"]), al(l["al"]), aq(l["aq"]),
      Ia(glm::vec3(l["Ia"][0],l["Ia"][1],l["Ia"][2])), 
      Id(glm::vec3(l["Id"][0],l["Id"][1],l["Id"][2])), 
      Is(glm::vec3(l["Is"][0],l["Is"][1],l["Is"][2])),
      pos(glm::vec3(l["position"][0],l["position"][1],l["position"][2]))
    {
      if(l["type"] == "point") { 
        dir = glm::vec3(0.f,0.f,0.f);
        aa = 0.0;
        theta = -180;
      }
      else {
        spot = GL_TRUE;
        dir = glm::normalize(glm::vec3(l["direction"][0],l["direction"][1],l["direction"][2]));
        aa = l["aa"];
        theta = l["theta"];
      }
    };

    const glm::vec3& getPos() const { return pos; };
    const glm::vec3& getIa()  const { return Ia; };
    const glm::vec3& getId()  const { return Id; };
    const glm::vec3& getIs()  const { return Is; };

    const glm::vec3 getDirToLight(const glm::vec3& point) const {
      return glm::normalize(pos - point);
    };

    float getDistToLight(const glm::vec3& point) const {
      return glm::length(pos - point);
    };

    float computeAttenuation(const glm::vec3& l, float d) const {
      float linear = 1 / (ac + al * d + aq * d * d);
      float angular = pow(glm::dot(l, dir), aa);
      return linear * angular;
    }

    const glm::vec3 computeAmbient(const glm::vec3& modelKa) const {
      return modelKa * Ia;
    }

    const glm::vec3 computeDiffuse(const glm::vec3& modelKd, const glm::vec3& n, const glm::vec3& l, float d) const {
      float alpha = computeAttenuation(-l,d);
      if( spot && (glm::dot(-l,dir) < glm::cos(glm::radians(theta))) ) return glm::vec3(0,0,0);
      return alpha * modelKd*Id*max(0.f, glm::dot(n,l));
    }

    const glm::vec3 computeSpecular(const glm::vec3& modelKs, float p, const glm::vec3& n, const glm::vec3& l, const glm::vec3& r, float d) const {
      float alpha = computeAttenuation(-l,d);
      if( spot && (glm::dot(-l,dir) < glm::cos(glm::radians(theta))) ) return glm::vec3(0,0,0);
      return alpha * modelKs*Is*max(0.f, pow(glm::dot(n,r), p));
    }

    void sendUniforms(GLuint program, int i) const {
      std::string spotString = "posLights[" + std::to_string(i) + "].spot";
      GLint spot_location = glGetUniformLocation(program, spotString.c_str());

      std::string posString = "posLights[" + std::to_string(i) + "].pos";
      GLint pos_location  = glGetUniformLocation(program, posString.c_str());

      std::string dirString = "posLights[" + std::to_string(i) + "].dir";
      GLint dir_location  = glGetUniformLocation(program, dirString.c_str());

      std::string IaString = "posLights[" + std::to_string(i) + "].Ia";
      GLint Ia_location = glGetUniformLocation(program, IaString.c_str());

      std::string IdString = "posLights[" + std::to_string(i) + "].Id";
      GLint Id_location = glGetUniformLocation(program, IdString.c_str());

      std::string IsString = "posLights[" + std::to_string(i) + "].Is";
      GLint Is_location = glGetUniformLocation(program, IsString.c_str());

      std::string aaString = "posLights[" + std::to_string(i) + "].aa";
      GLint aa_location = glGetUniformLocation(program, aaString.c_str());

      std::string acString = "posLights[" + std::to_string(i) + "].ac";
      GLint ac_location = glGetUniformLocation(program, acString.c_str());

      std::string alString = "posLights[" + std::to_string(i) + "].al";
      GLint al_location = glGetUniformLocation(program, alString.c_str());

      std::string aqString = "posLights[" + std::to_string(i) + "].aq";
      GLint aq_location = glGetUniformLocation(program, aqString.c_str());

      std::string cosThetaString = "posLights[" + std::to_string(i) + "].cosTheta";
      GLint cosTheta_location = glGetUniformLocation(program, cosThetaString.c_str());

      glUniform3fv(pos_location, 1, glm::value_ptr(pos));
      glUniform3fv(dir_location, 1, glm::value_ptr(dir));
      glUniform3fv(Ia_location, 1, glm::value_ptr(Ia));
      glUniform3fv(Id_location, 1, glm::value_ptr(Id));
      glUniform3fv(Is_location, 1, glm::value_ptr(Is));
      glUniform1f(cosTheta_location, glm::cos(glm::radians(theta)));
      glUniform1i(spot_location, spot);
      glUniform1f(aa_location, aa);
      glUniform1f(ac_location, ac);
      glUniform1f(al_location, al);
      glUniform1f(aq_location, aq);
    }
};

#endif