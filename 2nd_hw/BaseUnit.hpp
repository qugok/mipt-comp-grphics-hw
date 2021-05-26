#pragma once

#include <vector>
// Include GLEW
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>
#include <string>
#include "Camera.hpp"

class BaseUnit{
 public:
  virtual void render();
  virtual ~BaseUnit() = default;
};

class ColoredUnit : public BaseUnit{
 public:
  ColoredUnit() = default;
  ColoredUnit(const std::vector<GLfloat>& pos, const std::vector<GLfloat>& color);
  ColoredUnit(const std::vector<glm::vec3>& pos, const std::vector<glm::vec3>& color);
  ColoredUnit &operator=(ColoredUnit &) = delete;
  ColoredUnit &operator=(ColoredUnit &&);
  ColoredUnit(ColoredUnit &) = delete;
  ColoredUnit(ColoredUnit &&other);

  void render() override;
  ~ColoredUnit();
 private:
  GLuint g_vertexbuffer;
  GLuint c_vertexbuffer;
  std::vector<glm::vec3> pos_vertexes;
  std::vector<glm::vec3> color_vertexes;
};

class TexturedUnit : public BaseUnit{
 public:
  TexturedUnit() = default;
  TexturedUnit(GLuint pid
               , const std::vector<glm::vec3>& pos_fireball
               , const std::string& path_texture
               , const std::vector<glm::vec2>& uvs
               );
//  TexturedUnit(const std::vector<glm::vec3>& pos, const std::vector<glm::vec3>& color);
  TexturedUnit &operator=(TexturedUnit &) = delete;
  TexturedUnit &operator=(TexturedUnit &&);
  TexturedUnit(TexturedUnit &) = delete;
  TexturedUnit(TexturedUnit &&other) = delete;

  void render() override;
  ~TexturedUnit() override;
 private:
  GLuint vertexBuffer;
  GLuint uvBuffer;
  GLint TextureID;
  GLuint Texture;
  std::vector<glm::vec3> pos_vertexes;
  std::vector<glm::vec2> uvs_vertexes;
};
