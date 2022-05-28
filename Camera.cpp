#ifndef __CAMERA_CPP__
#define __CAMERA_CPP__
#include "Camera.h"

using namespace std;

Camera::Camera(const nlohmann::json &c, bool perspective, bool rasterize, float VPWidth, float VPHeight)
  : eye(glm::vec3(c["eye"][0],c["eye"][1],c["eye"][2])),
    top(glm::vec3(c["top"][0],c["top"][1],c["top"][2])),
    at(glm::vec3(c["at"][0],c["at"][1],c["at"][2])),
    fov(glm::radians((float) c["fov"])),
    d(c["d"]),
    n(c["n"]),
    f(c["f"]),
    VPWidth(VPWidth),
    VPHeight(VPHeight),
    perspView(perspective)
{
  //normalize directions
  at = glm::normalize(at);
  top = glm::normalize(top);
  right = glm::normalize(glm::cross(at, top));

  theta = glm::degrees(glm::atan(at.z / at.x));
  phi = -glm::degrees(glm::atan(at.y / at.z));

  if(perspView) {
    t = glm::tan(fov/2) * d;
    b = -t;
    r = t * ((float) VPWidth/ (float) VPHeight);
    l = -r;
    P = glm::perspective(fov, VPWidth/VPHeight, n, f );
  }
  else {
    rasterize ? t = 0.08 : t = 10;
    b = -t;
    r = t * ((float) VPWidth/ (float) VPHeight);
    l = -r;
    P = glm::ortho(l, r, b, t, n, f );
  }

  V = glm::lookAt(eye, eye+at ,top);
}

void Camera::moveEye(const glm::vec3 &offset) {
  eye = eye+offset;
  V = glm::lookAt(eye, eye+at ,top);
}

void Camera::setAt(const glm::vec3 &dir) {
  at = dir;
  // top = ; //this recalculate top
  // right = glm::normalize(glm::cross(at, top));
  V = glm::lookAt(eye, eye+at ,top);
}

Ray Camera::getRay(float i, float j) const {
  glm::vec3 w = glm::normalize(-at);
  glm::vec3 u = glm::normalize(glm::cross(top,w));
  glm::vec3 v = glm::normalize(glm::cross(w,u));
  float tau = l + ( (r-l)*(i+0.5)/VPWidth );
  float sigma = b + ( (t-b)*(j+0.5)/VPHeight );

  Ray r;
  if(perspView) {
    glm::vec3 dir = -d * w + tau * u + sigma * v;
    r = Ray (eye, glm::normalize(dir));
  }
  else {
    glm::vec3 position = eye + tau * u + sigma * v;
    r = Ray (position, -w);
  }
  return r;
}

void Camera::processKeyInput(int key, int action, float deltaTime) {
  if (action == GLFW_REPEAT || action == GLFW_PRESS) {
    const float cameraSpeed =  0.3f * deltaTime;
    glm::vec3 cameraOffset = glm::vec3();
    switch(key) {
      case GLFW_KEY_LEFT:
        cameraOffset = -0.05*right;
        moveEye(cameraOffset);
        break;
      case GLFW_KEY_RIGHT:
        cameraOffset = 0.05*right;
        moveEye(cameraOffset);
        break;
      case GLFW_KEY_UP:
        cameraOffset = 0.05*top;
        moveEye(cameraOffset);
        break;
      case GLFW_KEY_DOWN:
        cameraOffset = -0.05*top;
        moveEye(cameraOffset);
        break;
      case GLFW_KEY_I:
        cameraOffset = 0.05*at;
        moveEye(cameraOffset);
        break;
      case GLFW_KEY_O:
        cameraOffset = -0.05*at;
        moveEye(cameraOffset);
        break;
      default:
        std::cout << "Unhandled key: " << (int)(key) << std::endl;
        break;
    }
  }
  //handle wireframe toggle somewhere main(?)
  // if(action == GLFW_PRESS && key == GLFW_KEY_W) {
  //   if(rasterization) rast->toggleWireFrame();
  // }
}

void Camera::processMouseInput(float xoffset, float yoffset) {
  theta += xoffset;
  phi += yoffset;
  if(phi > 89.9f)
    phi =  89.5f;
  if(phi < -89.5f)
    phi = -89.5f;
  glm::vec3 direction;
  direction.x = cos(glm::radians(theta)) * cos(glm::radians(phi));
  direction.y = sin(glm::radians(phi));
  direction.z = sin(glm::radians(theta)) * cos(glm::radians(phi));
  setAt(glm::normalize(direction));
}
#endif