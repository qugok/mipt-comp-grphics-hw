#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

class Camera {
 public:
//  Camera(GLFWwindow *window);
  void init(GLFWwindow *window);
  void Update(float delta_time);
  glm::mat4 get_projection() const;
  glm::mat4 get_view() const;
  glm::vec3 get_position() const;
  /*
   * direction всегда длины 1
   */
  glm::vec3 get_direction() const;

 private:

  GLFWwindow *window;
  glm::mat4 Projection;
  glm::vec3 position;
  glm::vec3 direction;
  glm::mat4 rot;
  glm::mat4 View;

  double horizontalAngle;
  double verticalAngle;

  float speed; // 3 units / second
  float mouseSpeed;
  float initialFoV;

  static float limit;
};

