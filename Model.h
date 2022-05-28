#ifndef __MODEL_H__
#define __MODEL_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <iostream>
#include <string>

// Objects
#include "Ray.h"
#include "CollisionDet.h"
#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

class Model{
  protected:
    Material* mtl;

  public:
    Model(const std::string& fn) { mtl = new Material(fn); };
    virtual ~Model() = default;
    const Material& getMaterial() const { return *mtl; }; 
    virtual CollisionDet getCollisionDet(const Ray &r) const = 0;
};

#endif