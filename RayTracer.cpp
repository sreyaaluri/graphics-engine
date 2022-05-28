#ifndef __RAYTRACER_CPP__
#define __RAYTRACER_CPP__
#include "RayTracer.h"

using namespace std;
using json = nlohmann::json;

RayTracer::RayTracer(const InputParser &parser)
  : models(parser.getModels()), 
    dirLights(parser.getDirLights()),
    posLights(parser.getPosLights()),
    frameWidth(parser.frameWidth),
    frameHeight(parser.frameHeight),
    perspective(parser.perspective)
{
  cout << "creating raytracer" << endl;
  camera = parser.getCamera();
};

RayTracer::~RayTracer() {
  for (Model* tPtr: models){
   	delete tPtr;
  }
  for (DirectionalLight* dirLPtr: dirLights){
   	delete dirLPtr;
  }
  for (PositionalLight* posLPtr: posLights){
   	delete posLPtr;
  }
}

void RayTracer::initialize() {
  glClearColor(1.f, 1.f, 0.0f, 0.f); //yellow
  g_frame = std::make_unique<glm::vec4[]>(frameWidth*frameHeight);
}

void RayTracer::draw() {
  //clear
  for(int i = 0; i < frameWidth*frameHeight; ++i)
    g_frame[i] = glm::vec4(1.f, 1.f, 0.0f, 0.f);

  //draw
  for(int i = 0; i < frameWidth * frameHeight; ++i){
    int x = i % frameWidth;
    int y = (i-x)/frameWidth;
    g_frame[i] = getPixelColor(x,y);
  }

  glDrawPixels(frameWidth, frameHeight, GL_RGBA, GL_FLOAT, g_frame.get());
}

glm::vec4 RayTracer::getPixelColor(float i, float j) {
  //generate ray
  Ray ray = camera->getRay(i, j);

  //getting first collision
  Model* closestModel = nullptr;
  CollisionDet closestCollision;
  float minT = std::numeric_limits<float>::infinity(); //start at infinity
  for(auto& model: models){
    CollisionDet cDet = model->getCollisionDet(ray);
    if(cDet.exists() && cDet.getT() < minT){
      closestModel = model;
      closestCollision = cDet;
      minT = cDet.getT();
    }
  }

  //determining color at collision(if any) using light intensities
  if(!closestModel) return glm::vec4(1.f, 1.f, 0.0f, 0.f); //yellow if no collision

  //initializing lighting
  glm::vec3 col(0,0,0);

  for(auto& posLight: posLights){
    col += getColorPerPosLight(*posLight, *closestModel, closestCollision);
    
  }
  for(auto& dirLight: dirLights){
    col += getColorPerDirLight(*dirLight, *closestModel, closestCollision);
  }

  glm::vec4 finalCol = max(glm::vec4(0,0,0,0), min(glm::vec4(col,1.f), glm::vec4(1,1,1,1))); //keeping the color between 0 and 1

  // glm::vec3 col = glm::abs(closestCollision.getNormal());
  return finalCol;
}

const glm::vec3 RayTracer::getColorPerPosLight(const PositionalLight& posLight, const Model& model, const CollisionDet& colDet) const {
  const Material& mtl = model.getMaterial();

  //ambient
  glm::vec3 La = posLight.computeAmbient(mtl.getKa());

  // direction pointing from model collided with toward light
  glm::vec3 l = posLight.getDirToLight(colDet.getPoint());

  //shadow
  bool isShadowed = false;
  for(auto& t: models){
    Ray r(colDet.getPoint(), l);
    CollisionDet s = t->getCollisionDet(r);
    if(s.exists() && s.getT()<glm::distance(colDet.getPoint(), posLight.getPos())) isShadowed = true;
  }
  if(isShadowed) return La;
  
  // distance from model that is not shadowed to light
  float d = posLight.getDistToLight(colDet.getPoint());

  //diffuse
  glm::vec3 Ld = posLight.computeDiffuse(mtl.getKd(), colDet.getNormal(), l, d);

  //specular
  glm::vec3 v = camera->computeView(colDet.getPoint());
  glm::vec3 h = glm::normalize(v+l);
  glm::vec3 Ls = posLight.computeSpecular(mtl.getKs(), mtl.getP(), colDet.getNormal(), l, h, d);

  return La+Ld+Ls;
}

const glm::vec3 RayTracer::getColorPerDirLight(const DirectionalLight& dirLight, const Model& model, const CollisionDet& colDet) const {
  const Material& mtl = model.getMaterial();

  //ambient
  glm::vec3 La = dirLight.computeAmbient(mtl.getKa());

  // direction pointing from model collided with toward light
  glm::vec3 l = dirLight.getDirToLight();

  //shadow
  bool isShadowed = false;
  for(auto& t: models){
    Ray r(colDet.getPoint(), l);
    CollisionDet s = t->getCollisionDet(r);
    if(s.exists()) isShadowed = true;
  }
  if(isShadowed) return La;
  
  //diffuse
  glm::vec3 Ld = dirLight.computeDiffuse(mtl.getKd(), colDet.getNormal());

  //specular
  glm::vec3 v = camera->computeView(colDet.getPoint());
  glm::vec3 Ls = dirLight.computeSpecular(mtl.getKs(), mtl.getP(), colDet.getNormal(), v);

  return La+Ld+Ls;
}

#endif