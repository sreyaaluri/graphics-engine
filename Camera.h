#ifndef __CAMERA_H__
#define __CAMERA_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <iostream>

// GL / GLM
#include "GLInclude.h"

// JSON
#include <nlohmann/json.hpp>

// Objects
#include "Ray.h"

////////////////////////////////////////////////////////////////////////////////

// JD - n and d are the same. Reconcile the differences between perspective, ortho, and rasterize vs raytrace.
class Camera{
  private:
    bool perspView;
    glm::vec3 eye;            //camera eye == pos
    glm::vec3 at;             //camera at
    glm::vec3 top;            //camera top
    glm::vec3 right;          //camera right
    float d;                  //focal length
    float fov;                //field of view
    float t;                  //top
    float l;                  //left
    float b;                  //bottom
    float r;                  //right
    float n;                  //near
    float f;                  //far
    float VPWidth;            //view plane width
    float VPHeight;           //view plane height
    glm::mat4 V;              //view matrix
    glm::mat4 P;              //projection matrix
    float theta = -90.f;      //angle calculated clockwise about y-axis, so -90 degree implies forward-facing
    float phi = 0.f;          //angle calculated counter-clockwise about x-axis, so 0 degrees implies front-facing

    void moveEye(const glm::vec3 &offset);
    void setAt(const glm::vec3 &dir);
    
  public:
    Camera() = default;
    Camera(const nlohmann::json &c, bool perspective, bool rasterize, float VPWidth, float VPHeight);

    Ray getRay(float i, float j) const;
    const glm::vec3& getEye() const { return eye; };
    const glm::vec3& getAt() const {return at; };
    const glm::vec3& getTop() const { return top; };

    const glm::mat4& getViewMatrix() const { return V; };
    const glm::mat4& getProjectionMatrix() const { return P; };

    const glm::vec3 computeView(const glm::vec3& point) const { return glm::normalize(eye-point); };

    void processKeyInput(int key, int action, float deltaTime);
    void processMouseInput(float xoffset, float yoffset);
};

#endif