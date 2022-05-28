#ifndef __LINEREPULSOR_H__
#define __LINEREPULSOR_H__

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

class LineRepulsor : public Force{
  private:
    float G;
    float M;
    glm::vec3 pos;
    glm::vec3 dir;

  public:
    LineRepulsor(const nlohmann::json& pa) 
      : G(pa["G"]), M(pa["mass"]), pos(glm::vec3(pa["pos"][0], pa["pos"][1], pa["pos"][2])), dir(glm::normalize(glm::vec3(pa["dir"][0], pa["dir"][1], pa["dir"][2])))
    {};

    glm::vec3 applyForce(const glm::vec3& p, float m) const {
      float t =  glm::dot(p-pos,dir);         // projection of particle pos on direction  // this understand the math
      glm::vec3 closestPos = pos + t*dir;     // closest point on line to particle
      glm::vec3 u = p-closestPos;             // u is away from the mass
      float r = glm::length(u);
      u = glm::normalize(u);
      return G * M * u / (r*r);               // return acceleration
    };
};

#endif