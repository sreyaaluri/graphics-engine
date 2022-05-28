#ifndef __RAY_H__
#define __RAY_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <iostream>

// GL / GLM
#include "GLInclude.h"

////////////////////////////////////////////////////////////////////////////////

class Ray{
  private:
    glm::vec3 origin;
    glm::vec3 dir;

  public:
    Ray() = default;

    Ray(const glm::vec3& origin, const glm::vec3& direction)
    : origin(origin),
      dir(direction) 
    {};

    const glm::vec3& getOrigin() const { return origin; };
    const glm::vec3& getDir() const { return dir; };
};

#endif