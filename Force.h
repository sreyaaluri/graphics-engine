#ifndef __FORCE_H__
#define __FORCE_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <iostream>
#include <string>

// GL / GLM
#include "GLInclude.h"

////////////////////////////////////////////////////////////////////////////////

class Force{
  public:
    virtual glm::vec3 applyForce(const glm::vec3& p, float mass) const = 0; //redefine
};

#endif