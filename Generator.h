#ifndef __GENERATOR_H__
#define __GENERATOR_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <string>
#include <random>

// GL / GLM
#include "GLInclude.h"

// JSON
#include <nlohmann/json.hpp> 

////////////////////////////////////////////////////////////////////////////////

class Generator{
  enum type {POINT, DIRECTED, DISC, UNDEFINED} type; //use enum classes instead

  private:
    bool uniform_dist;
    glm::vec4 col;
    glm::vec4 p;
    float v;
    std::uniform_real_distribution<float> uDist;
    std::normal_distribution<float> nDist;
    std::uniform_real_distribution<float> massDist;

  public:
    Generator(const nlohmann::json &g)
      : p(glm::vec4(g["pos"][0],g["pos"][1],g["pos"][2], 1.f)), 
        col(glm::vec4(g["col"][0],g["col"][1],g["col"][2], 1.f)),
        v(g["v"])
    {
      if(g["type"] == "point") type = POINT;
      else type = UNDEFINED;

      g["dist_type"] == "uniform" ? uniform_dist = true : uniform_dist = false;

      if(uniform_dist) {
        uDist = std::uniform_real_distribution<float>((float) g["low"], (float) g["high"]);
      } else {
        nDist = std::normal_distribution<float>((float) g["mean"], (float) g["variance"]);
      }

      massDist = std::uniform_real_distribution<float>((float) g["mass_range"][0], (float) g["mass_range"][1]);
    };

    glm::vec4 getCol() { return col; };

    glm::vec4 getPos() {
      if(type != POINT) return glm::vec4(0,0,0,1);
      return p;
    };

    glm::vec3 getVel() {
      if(type != POINT) return glm::vec3(0,0,0);
      glm::vec3 r(1.f, 1.f, 1.f); 
      static std::default_random_engine gen;
      if(uniform_dist){
        r = glm::vec3(uDist(gen), uDist(gen), uDist(gen));
      }
      else {
        r = glm::vec3(nDist(gen), nDist(gen), nDist(gen));
      }
      return v * r;
    };
    
    float getMass() {
      static std::default_random_engine gen;
      return massDist(gen);
    }
};

#endif