#ifndef __WIND_H__
#define __WIND_H__

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

class Wind : public Force{
  private:
    glm::vec3 dir;
    float mag;

  public:
    Wind(const nlohmann::json& pa) 
      : dir(glm::normalize(glm::vec3(pa["dir"][0], pa["dir"][1], pa["dir"][2]))), mag(pa["magnitude"])
    {};

    glm::vec3 applyForce(const glm::vec3& p, float m) const {
      return (mag * dir) / m;   //return acceleration
    };
};

#endif