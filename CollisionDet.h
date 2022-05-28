#ifndef __COLLISIONDET_H__
#define __COLLISIONDET_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <iostream>

// GL / GLM
#include "GLInclude.h"

////////////////////////////////////////////////////////////////////////////////

class CollisionDet{
  private:
    glm::vec3 point;
    glm::vec3 normal;
    float t;
    bool collides;

  public:
    CollisionDet()
    : collides(false)
    {
      point = {0,0,0};
      normal = {0,0,0};
      t = 0;
    };

    CollisionDet(const glm::vec3& point, const glm::vec3& normal, float t)
    : collides(true),
      point(point),
      normal(normal),
      t(t)
    {};

    const glm::vec3& getPoint() const { return point; };
    const glm::vec3& getNormal() const { return normal; };
    float getT() const { return t; };
    bool exists() const { return collides; };
};

#endif