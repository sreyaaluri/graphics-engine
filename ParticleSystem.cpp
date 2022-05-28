#ifndef __PARTICLESYSTEM_CPP__
#define __PARTICLESYSTEM_CPP__
#include "ParticleSystem.h"

using namespace std;

ParticleSystem::ParticleSystem(const nlohmann::json &sys)
  : MAX_PARTICLES(sys["max_particles"])
{
  //constructing a generator
  generator = new Generator(sys["generator"]);

  // initializing model matrix
  glm::mat4 translationM = glm::translate(glm::mat4(1.0f), glm::vec3(sys["T"][0], sys["T"][1], sys["T"][2]));
  glm::mat4 rotationM = glm::rotate(glm::mat4(1.0f), glm::radians((float) sys["R"][0]), glm::vec3(sys["R"][1], sys["R"][2], sys["R"][3]));
  glm::mat4 scaleM = glm::scale(glm::mat4(1.0f), glm::vec3(sys["S"][0], sys["S"][1], sys["S"][2]));
  M = translationM * rotationM * scaleM;

  //initializing particles
  for (int i = 0; i < MAX_PARTICLES; i++)
    particles.emplace_back(generator->getPos(), generator->getVel(), generator->getCol(), generator->getMass());

  //initializing forces
  for (const nlohmann::json force: sys["forces"]){
    if(force["type"]=="point attractor"){
      PointAttractor* f = new PointAttractor(force);
      forces.emplace_back(f);
    }
    else if(force["type"]=="point repulsor"){
      PointRepulsor* f = new PointRepulsor(force);
      forces.emplace_back(f);
    }
    else if(force["type"]=="line repulsor"){
      LineRepulsor* f = new LineRepulsor(force);
      forces.emplace_back(f);
    }
    else if(force["type"]=="wind"){
      Wind* f = new Wind(force);
      forces.emplace_back(f);
    }
  }
}

void ParticleSystem::update(float appDT) {
  for(particle& p : particles){               // updating existing particles using semi-emplicit euler integration
    glm::vec3 netAcc(0.f,0.f,0.f);
    for(Force* fPtr : forces){                // for each force, apply force and net get acceleration
      netAcc += fPtr->applyForce(p.pos, p.mass);
    }
    p.acc = netAcc;                           // set acceleration to calculated next acc.
    p.vel = p.vel + p.acc * appDT;            // update velocity using acceleration
    p.pos = p.pos + p.vel *  appDT;           // update position using next new velocity
  }
}

void ParticleSystem::sendVertexData() {
  //requesting Vertex Array Object
  glGenVertexArrays(1, &vao);
  glBindVertexArray(vao);

  //generate vertex buffer object
  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, particles.size()*sizeof(particle), &particles[0], GL_STATIC_DRAW);

  //specifying layout of data using vertex attribute pointer
  glEnableVertexAttribArray(0);                                                       // 1st attribute in VBO (position)
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        sizeof(particle), (char *)NULL + 0);

  glEnableVertexAttribArray(1);                                                       // 2nd attribute in VBO (color)
  glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE,
                        sizeof(particle), (char *)NULL + sizeof(glm::vec3));

  glEnableVertexAttribArray(2);                                                       // 3rd attribute in VBO (mass)
  glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE,
                        sizeof(particle), (char *)NULL + sizeof(glm::vec4) + sizeof(glm::vec3)); 
}

void ParticleSystem::draw() {
  sendVertexData();
  glBindVertexArray(vao);
  glDrawArrays(GL_POINTS, 0, particles.size());
  glBindVertexArray(0);
}

#endif