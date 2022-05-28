#ifndef __SPHERE_CPP__
#define __SPHERE_CPP__
#include "Sphere.h"
#include <algorithm>

using namespace std;

Sphere::Sphere(const nlohmann::json &s)
  : Model(s["mtl"]),
    c(glm::vec3(s["center"][0],s["center"][1],s["center"][2])), 
    r(s["radius"])
{}

CollisionDet Sphere::getCollisionDet(const Ray &ray) const {
  float A = glm::length2(ray.getDir());                     // A = d^2
  float B = glm::dot( 2*ray.getDir(), ray.getOrigin()-c );  // B = 2d • (p-c)
  float C = glm::length2(ray.getOrigin()-c) - (r*r);        // C = (p-c)^2 - r^2
  float discriminant = B*B - 4*A*C;
  if( discriminant > 0 ) {
    float tminus = ((-B - sqrt(discriminant)) / (2*A)) - std::numeric_limits<float>::epsilon()*1000;
    float tplus = ((-B + sqrt(discriminant)) / (2*A)) - std::numeric_limits<float>::epsilon()*1000;
    float t = 0;

    if( tminus > 0 && tplus > 0 ){ //if both are positive, take the smaller one
      t = min(tminus, tplus);
    }
    else if( (tminus < 0 && tplus > 0) || (tplus < 0 && tminus > 0) ){ //if one positive and the other is negative, take the positive one
      t = max(tminus, tplus);
    }

    if(t>0){
      glm::vec3 x = ray.getOrigin() + (t * ray.getDir());
      return CollisionDet(x,(x-c)/r,t);
    }
  }
  return CollisionDet();
}

void Sphere::getMesh() {

}

#endif