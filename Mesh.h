#ifndef __MESH_H__
#define __MESH_H__

////////////////////////////////////////////////////////////////////////////////
// Includes

// STL
#include <iostream>

// GL / GLM
#include "GLInclude.h"

// JSON
#include <nlohmann/json.hpp> 

// Objects
#include "Material.h"

////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief Simple vertex class.
struct vertex
{
  glm::vec4 p; ///< Position
  glm::vec4 n; ///< Normal
  glm::vec2 t; ///< Texture

  vertex(const glm::vec4 &pos, const glm::vec4 &n, const glm::vec2 &t) : p{pos}, n{glm::normalize(n)}, t{t} {};
  void print(int i) {
    std::cout << std::endl << i << " p: " << glm::to_string(p) << "\t n: " << glm::to_string(n) << "\t t: " << glm::to_string(t) << std::endl << std::endl;
  };
};

class Mesh
{
  private:
    std::vector<vertex> vertices;
    glm::mat4 M;
    Material* mtl;

  public:
    GLuint vao; //make private
    GLuint vbo; //make private
    Mesh(const std::vector<vertex>& vertices, const glm::mat4& M, Material* mtl)
      : vertices(vertices), M(M), mtl(mtl) {};
    Mesh(const nlohmann::json &mesh);
    const std::vector<vertex>& getVertices() const { return vertices; };
    const Material& getMaterial() const { return *mtl; };
    const glm::mat4& getModelMatrix() const { return M; };
    void initialize();
    void draw();
    void print() {
      for(int i = 0; i < vertices.size(); i++){
        vertices[i].print(i);
      }
    }
};

#endif