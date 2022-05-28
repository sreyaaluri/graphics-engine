#ifndef __PLANE_H__
#define __PLANE_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <iostream>

// GL / GLM
#include "GLInclude.h"

// JSON
#include <nlohmann/json.hpp> 

// Objects
#include "Model.h"
#include "Mesh.h"

////////////////////////////////////////////////////////////////////////////////

class Plane : public Model
{
  private:
    glm::vec3 a;
    glm::vec3 n;
    Mesh* m;

  public:
    Plane(const nlohmann::json &p);
    CollisionDet getCollisionDet(const Ray &r) const override; 
    Mesh* getMesh(glm::mat4 T, glm::mat4 R, glm::mat4 S); //change return type
};

#endif