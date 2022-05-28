#ifndef __RASTERIZER_CPP__
#define __RASTERIZER_CPP__
#include "Rasterizer.h"

using namespace std;

Rasterizer::Rasterizer(const InputParser& parser)
  : meshes(parser.getMeshes()), dirLights(parser.getDirLights()),  posLights(parser.getPosLights()), triangleProgram(0), pointProgram(0), wireframe(parser.wireframe)
{
  camera = parser.getCamera();                        //initializing camera
  json part_sys = parser.getPartSysJSON();            //initializing particle system
  if(part_sys.empty()) partSys = nullptr;
  else partSys = new ParticleSystem(part_sys);
};

void Rasterizer::initialize() {
  if(wireframe) glClearColor(0.0f, 0.0f, 0.0f, 0.f); //black
  else glClearColor(1.f, 1.f, 0.0f, 0.f); //yellow
  glEnable(GL_PROGRAM_POINT_SIZE);
  glEnable(GL_COLOR_MATERIAL);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  if(wireframe) {
    triangleProgram = compileProgram("Shaders/triangle.vert",
                              "Shaders/wireframe.frag");
  }
  else {
    triangleProgram = compileProgram("Shaders/triangle.vert",
                             "Shaders/triangle.frag");
  }
  pointProgram = compileProgram("Shaders/point.vert",
                             "Shaders/point.frag");

  // send mesh vertex data (static)
  glUseProgram(triangleProgram);
  sendMeshVertexData();
  sendLightData();
}

void Rasterizer::update(float appDT) {
  partSys->update(appDT);
}

void Rasterizer::draw() {
  //clear
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(wireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // draw triangles
  glUseProgram(triangleProgram);
  for(Mesh* m : meshes) {
    sendMeshUniforms(*m);
    m->draw();
  }

  // draw particle system
  glUseProgram(pointProgram);
  sendPartSysUniforms();
  partSys->draw();
}

void Rasterizer::sendMeshVertexData() {
  //creating Vertex Array Objects for each model
  for(Mesh* m : meshes) m->initialize();
}

void Rasterizer::sendLightData() {
  //sending light data
  int count=0;
  for(int i=0; i<dirLights.size(); i++){
    dirLights[i]->sendUniforms(triangleProgram, count);
    count++;
  }
  for(int i=0; i<posLights.size(); i++){
    posLights[i]->sendUniforms(triangleProgram, count);
    count++;
  }
}

void Rasterizer::sendMeshUniforms(const Mesh& m) {

  //sending material data
  m.getMaterial().sendUniforms(triangleProgram);

  //sending transformation matrices data
  const glm::mat4& M = m.getModelMatrix();
  const glm::mat4& V = camera->getViewMatrix();
  const glm::mat4& P = camera->getProjectionMatrix();

  glm::mat4 MV = V * M;
  glm::mat4 MV_inv_T = glm::transpose(glm::inverse(MV));
  GLint mv_location = glGetUniformLocation(triangleProgram, "MV");
  GLint mvit_location = glGetUniformLocation(triangleProgram, "MV_inv_T");
  GLint P_location = glGetUniformLocation(triangleProgram, "Proj");
  GLint V_location = glGetUniformLocation(triangleProgram, "View");
  glUniformMatrix4fv(mv_location, 1, GL_FALSE, glm::value_ptr(MV));
  glUniformMatrix4fv(mvit_location, 1, GL_FALSE, glm::value_ptr(MV_inv_T));
  glUniformMatrix4fv(P_location, 1, GL_FALSE, glm::value_ptr(P));
  glUniformMatrix4fv(V_location, 1, GL_FALSE, glm::value_ptr(V));
}

void Rasterizer::sendPartSysUniforms() {
  const glm::mat4& M = partSys->getModelMatrix();
  const glm::mat4& V = camera->getViewMatrix();
  const glm::mat4& P = camera->getProjectionMatrix();
  glm::mat4 MVP = P * V * M;
  GLint mvp_location = glGetUniformLocation(pointProgram, "MVP");
  glUniformMatrix4fv(mvp_location, 1, GL_FALSE, glm::value_ptr(MVP));
}

#endif