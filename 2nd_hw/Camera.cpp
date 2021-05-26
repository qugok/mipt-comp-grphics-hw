#include "Camera.hpp"
#include "OGLstrike.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>

float Camera::limit = 200;

void Camera::init(GLFWwindow *windo) {
  window = windo;

  speed = 10.0f;
  mouseSpeed = 0.005;
  initialFoV = 120.0f;

  horizontalAngle = M_PI * 5 / 4;
// вертикальный угол
  verticalAngle = 0.0f;

  Projection = glm::perspective(glm::radians(initialFoV), 4.0f / 3.0f, 0.1f, limit * 4.1f);
  // Camera matrix
  position = glm::vec3(5, 5, 5);
  glfwSetCursorPos(window, 1024/2, 768/2);
//  rot = glm::rotate(glm::mat4(), glm::radians(0.3f), glm::vec3(0, 1, 0));

  direction = glm::vec3(
      cos(verticalAngle) * sin(horizontalAngle),
      sin(verticalAngle),
      cos(verticalAngle) * cos(horizontalAngle)
  );


  glfwSetScrollCallback(window, [](GLFWwindow *w, double x, double y) {
    auto& cam = static_cast<OGLstrike*>(glfwGetWindowUserPointer( w ))->camera;
    if (glm::length(cam.position + glm::vec3{0, y / 2, 0}) < limit) {
      cam.position.y += y / 2;
    }
  });

  // Right vector
  glm::vec3 right = glm::vec3(
      sin(horizontalAngle - 3.14f/2.0f),
      0,
      cos(horizontalAngle - 3.14f/2.0f)
  );

  // Up vector
  glm::vec3 up = glm::cross( right, direction );

  View       = glm::lookAt(
      position,           // Camera is here
      position+direction, // and looks here : at the same position, plus "direction"
      up                  // Head is up (set to 0,-1,0 to look upside-down)
  );
//  MatrixID = glGetUniformLocation(enemy_pid, "MVP");
}
void Camera::Update(float delta_time) {
  // Get mouse position
  double xpos, ypos;
  int xsize, ysize;
  glfwGetCursorPos(window, &xpos, &ypos);

  // Reset mouse position for next frame
  glfwGetWindowSize(window, &xsize, &ysize);
  glfwSetCursorPos(window, xsize/2, ysize/2);

  // Compute new orientation
  horizontalAngle += mouseSpeed * float(xsize/2 - xpos );
  verticalAngle   += mouseSpeed * float( ysize/2 - ypos );

  if (verticalAngle > M_PI_2){
    verticalAngle = M_PI_2;
  }
  if (verticalAngle < -M_PI_2){
    verticalAngle = -M_PI_2;
  }
  // Direction : Spherical coordinates to Cartesian coordinates conversion
  direction = glm::vec3(
      cos(verticalAngle) * sin(horizontalAngle),
      sin(verticalAngle),
      cos(verticalAngle) * cos(horizontalAngle)
  );

  // Right vector
  glm::vec3 right = glm::vec3(
      sin(horizontalAngle - 3.14f/2.0f),
      0,
      cos(horizontalAngle - 3.14f/2.0f)
  );

  // Up vector
  glm::vec3 up = glm::cross( right, direction );

  // Move forward
//  glm::vec3 temp_dir = {direction.x, 0, direction.z};
  if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_W ) == GLFW_PRESS){
    if (glm::length(position + direction * delta_time * speed) < limit) {
//    if (glm::length(position + temp_dir * delta_time * speed) < limit) {
      position += direction * delta_time * speed;
//      position += temp_dir * delta_time * speed;
    }
  }
  // Move backward
  if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_S ) == GLFW_PRESS){
    if (glm::length(position - direction * delta_time * speed) < limit) {
//    if (glm::length(position - temp_dir * delta_time * speed) < limit) {
      position -= direction * delta_time * speed;
//      position -= temp_dir * delta_time * speed;
    }
  }
  // Strafe right
  if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_D ) == GLFW_PRESS){
    if (glm::length(position + right * delta_time * speed) < limit) {
      position += right * delta_time * speed;
    }
  }
  // Strafe left
  if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS || glfwGetKey( window, GLFW_KEY_A ) == GLFW_PRESS){
    if (glm::length(position - right * delta_time * speed) < limit) {
      position -= right * delta_time * speed;
    }
  }


  if (glfwGetKey( window, GLFW_KEY_LEFT_SHIFT  ) == GLFW_PRESS){
    glm::vec3 delta = {0, delta_time * speed, 0};
    if (glm::length(position + delta) < limit) {
      position += delta;
    }
  }

  if (glfwGetKey( window, GLFW_KEY_LEFT_CONTROL   ) == GLFW_PRESS){
    glm::vec3 delta = {0, delta_time * speed, 0};
    if (glm::length(position - delta) < limit) {
      position -= delta;
    }
  }

  Projection = glm::perspective(glm::radians(initialFoV), (GLfloat)xsize/(GLfloat)ysize, 0.1f, limit * 4.1f);

  View       = glm::lookAt(
      position,           // Camera is here
      position+direction, // and looks here : at the same position, plus "direction"
      up                  // Head is up (set to 0,-1,0 to look upside-down)
  );

}

glm::mat4 Camera::get_projection() const {
  return Projection;
}

glm::mat4 Camera::get_view() const {
  return View;
}

glm::vec3 Camera::get_position() const {
  return position;
}

glm::vec3 Camera::get_direction() const {
  return direction;
}
