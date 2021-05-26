#pragma once

#include <vector>
// Include GLEW
#include <GL/glew.h>

// Include GLM
#include <glm/glm.hpp>
#include "Camera.hpp"
#include "BaseUnit.hpp"

class Unit {
 public:
  std::string to_string() const;
  static Unit from_string(const std::string& s, GLuint MatrixID, GLuint pid, BaseUnit* base);
  Unit(GLuint MatrixID, GLuint pid, glm::vec3 pos, BaseUnit* base, glm::vec2 rotation = {0, 0}, float radus = 1);

  void set_live_time(double live_time=0, bool can_dead = false);

  bool collision(const Unit& unit) const;
  bool is_dead() const;
  void set_moving(glm::vec3 direction, GLfloat speed);
  void render(const Camera& camera, float deltaTime);

 private:
  void move(float deltaTime);
 private:
  float collider_radius;
  glm::vec3 pos; // coordinates
  glm::vec2 rotation;
  glm::vec3 delta; // moving

  BaseUnit* base;

  double live_time;
  bool undead = true;
  GLuint MatrixID;
  GLuint pid;
};
