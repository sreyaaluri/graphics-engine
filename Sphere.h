#ifndef __SPHERE_H__
#define __SPHERE_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <iostream>
#include <math.h>

// GL / GLM
#include "GLInclude.h"

// JSON
#include <nlohmann/json.hpp> 

// Objects
#include "Model.h"

////////////////////////////////////////////////////////////////////////////////

class Sphere : public Model
{
  private:
    glm::vec3 c;
    float r;

  public:
    Sphere(const nlohmann::json &s);
    CollisionDet getCollisionDet(const Ray &r) const override;
    void getMesh(); //change return type
};

#endif