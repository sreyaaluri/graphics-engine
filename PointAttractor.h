#ifndef __POINTATTRACTOR_H__
#define __POINTATTRACTOR_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <iostream>
#include <string>

// GL / GLM
#include "GLInclude.h"

// JSON
#include <nlohmann/json.hpp> 

//Objects
#include "Force.h"

////////////////////////////////////////////////////////////////////////////////

class PointAttractor : public Force{
  private:
    float G;
    float M;
    glm::vec3 pos;

  public:
    PointAttractor(const nlohmann::json& pa) 
      : G(pa["G"]), M(pa["mass"]), pos(glm::vec3(pa["pos"][0], pa["pos"][1], pa["pos"][2]))
    {};

    glm::vec3 applyForce(const glm::vec3& p, float m) const {
      glm::vec3 u = p-pos;        //u is away from the mass
      float r = glm::length(u);
      u = glm::normalize(u);
      return - G * M * u / (r*r);   //return acceleration
    };
};

#endif