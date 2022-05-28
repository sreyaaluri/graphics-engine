#ifndef __PARTICLESYSTEM_H__
#define __PARTICLESYSTEM_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <iostream>
#include <vector>

// GL / GLM
#include "GLInclude.h"

// JSON
#include <nlohmann/json.hpp> 

// Objects
#include "Generator.h"
#include "Force.h"
#include "PointAttractor.h"
#include "PointRepulsor.h"
#include "LineRepulsor.h"
#include "Wind.h"

////////////////////////////////////////////////////////////////////////////////

//rethink whether to use struct or direct array in system
struct particle {
  glm::vec3 pos;
  glm::vec4 col;
  float mass;
  glm::vec3 vel;
  glm::vec3 acc;

  particle(const glm::vec3& pos, const glm::vec3& vel, const glm::vec4& col, float mass)
    : pos(pos), col(col), mass(mass), vel(vel), acc(glm::vec3(0)) {};
};

class ParticleSystem{
  private:
    size_t MAX_PARTICLES;
    std::vector<particle> particles;
    std::vector<Force*> forces;
    Generator* generator;
    glm::mat4 M;
    GLuint vao;
    GLuint vbo;
    void sendVertexData();

  public: //add def
    ParticleSystem(const nlohmann::json &sys);
    const glm::mat4& getModelMatrix() const { return M; };
    void initialize();
    void update(float appDT);
    void draw();
};

#endif