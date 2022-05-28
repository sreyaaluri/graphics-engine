#ifndef __PLANE_CPP__
#define __PLANE_CPP__
#include "Plane.h"

using namespace std;

Plane::Plane(const nlohmann::json &p)
  : Model(p["mtl"]),
    a(glm::vec3(p["point"][0],p["point"][1],p["point"][2])), 
    n(glm::vec3(p["normal"][0],p["normal"][1],p["normal"][2]))
{}

CollisionDet Plane::getCollisionDet(const Ray &r) const {
  float denominator = glm::dot(r.getDir(), n);
  if( denominator != 0){
    float t = ((glm::dot( (a-r.getOrigin()), n )) / denominator) - std::numeric_limits<float>::epsilon()*100;     //t = (a-p)•n / d•n
    if(t>0) return CollisionDet( r.getOrigin() + t*r.getDir(), n, t );
  }
  return CollisionDet();
}

Mesh* Plane::getMesh(glm::mat4 T, glm::mat4 R, glm::mat4 S) {
  glm::vec3 x_axis = glm::vec3(1,0,0);
  glm::vec3 y_axis = glm::vec3(0,1,0);
  float theta = glm::asin(glm::dot(n, x_axis));       // gives angle about y_axis
  float phi = glm::asin(glm::dot(n, y_axis));         // gives angle about x_axis
  cout << "theta: "<< theta << "\t phi: " << phi << endl;
  glm::mat4 rot1 = glm::rotate(glm::mat4(1.0f), theta, y_axis);
  glm::mat4 rot2 = glm::rotate(glm::mat4(1.0f), phi, x_axis);
  glm::mat4 trans = glm::translate(glm::mat4(1.0f), a);
  glm::mat4 transform = trans * rot2 * rot1;

  glm::vec4 p1 = glm::vec4(-100, -100, 0, 1);
  glm::vec4 p2 = glm::vec4(+100, -100, 0, 1);
  glm::vec4 p3 = glm::vec4(+100, +100, 0, 1);
  glm::vec4 p4 = glm::vec4(-100, +100, 0, 1);
  p1 = transform * p1;
  p2 = transform * p2;
  p3 = transform * p3;
  p4 = transform * p4;

  glm::vec2 t1(0,0);
  glm::vec2 t2(1,0);
  glm::vec2 t3(1,1);
  glm::vec2 t4(0,1);

  std::vector<vertex> vertices;
  vertices.emplace_back(p1, glm::vec4(n,0), t1); // 1
  vertices.emplace_back(p2, glm::vec4(n,0), t2); // 2
  vertices.emplace_back(p3, glm::vec4(n,0), t3); // 3
  vertices.emplace_back(p1, glm::vec4(n,0), t1); // 1
  vertices.emplace_back(p4, glm::vec4(n,0), t4); // 4
  vertices.emplace_back(p3, glm::vec4(n,0), t3); // 3

  m = new Mesh(vertices, T*R*S, mtl);
  return m;
}

#endif