#include "Unit.hpp"
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.hpp"
#include <sstream>

void Unit::render(const Camera &camera, float deltaTime) {
  auto Projection = camera.get_projection();
  auto View = camera.get_view();
  glm::mat4 Model = glm::translate(glm::mat4(), pos) * glm::mat4(1.0f);
  Model = glm::rotate(Model, rotation.y, glm::vec3(0, 0, 1));
  Model = glm::rotate(Model, rotation.x, glm::vec3(0, 1, 0));

  glm::mat4 MVP = Projection * View * Model;
//  MVP = glm::rotate(MVP, rotation.y, glm::vec3(0, 0, 1));
//  MVP = glm::rotate(MVP, rotation.x, glm::vec3(0, 1, 0));

  move(deltaTime);
  if (not undead){
    live_time -= deltaTime;
//    std::cout << deltaTime << "\n";
  }
//  std::cout << deltaTime << "\t" << delta.x << " " << delta.y << " " << delta.z << "\n";
//  MVP = translate(MVP, pos);
//  glGetUniformLocation();
  glUseProgram(pid);
  glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &MVP[0][0]);

  base->render();
}
Unit::Unit(
    GLuint MatrixID,
    GLuint pid,
    glm::vec3 pos,
    BaseUnit *base,
    glm::vec2 rotation,
    float radus)
//    : MatrixID(glGetUniformLocation(enemy_pid, "MVP")),
    : MatrixID(MatrixID),
      pid(pid),
//      enemy_pid(LoadShaders("main.vertexshader", "default.fragmentshader")),
      pos(pos),
      rotation(rotation),
      base(base),
      collider_radius(radus)
      {}

void Unit::set_moving(glm::vec3 direction, GLfloat speed) {
  if (glm::length(direction) < 0.0001) {
    delta = {0, 0, 0};
    return;
  }
  delta = (direction / glm::length(direction)) * speed;
//  std::cout << delta.x << "\t" << delta.y << "\t"<< delta.z << "\n";

}
void Unit::move(float deltaTime) {
  auto can_move = [this, &deltaTime]()
      {return glm::length(pos + delta * deltaTime) < 100
       || glm::length(pos + delta * deltaTime) < glm::length(pos);};
  if (can_move()) {
    pos += delta * deltaTime;
  } else
  {
    float cos_ = glm::dot(pos, delta) / glm::length(delta) / glm::length(pos);
    delta -= 2 * cos_ * glm::length(delta) * pos / glm::length(pos);
    if (not can_move()){
      delta = - glm::length(delta) * pos / glm::length(pos);
    }
  }
}
bool Unit::collision(const Unit &unit) const{
  return glm::length(pos - unit.pos) < collider_radius + unit.collider_radius;
}
bool Unit::is_dead() const {
  return live_time < 0;
}
void Unit::set_live_time(double live_time_, bool can_dead) {
  live_time = live_time_;
  undead = not can_dead;
}
std::string Unit::to_string() const{
  std::ostringstream out;

  out << collider_radius << " ";
  out << pos.x << " " << pos.y << " " << pos.z << " "; // coordinates
  out << rotation.x << " " << rotation.y << " "; // coordinates
  out << delta.x << " " << delta.y << " " << delta.z << " "; // coordinates
  out << live_time << " " << undead;
  return out.str();
}
Unit Unit::from_string(const std::string &s, GLuint MatrixID, GLuint pid, BaseUnit *base) {
  std::istringstream in(s);

  float collider_radius;
  glm::vec3 pos; // coordinates
  glm::vec2 rotation;
  glm::vec3 delta; // moving
  double live_time;
  bool undead = false;

  in >> collider_radius;
  in >> pos.x >> pos.y >> pos.z; // coordinates
  in >> rotation.x >>  rotation.y ; // coordinates
  in >> delta.x >>  delta.y >> delta.z ; // coordinates
  in >> live_time >> undead;

  auto ans = Unit(MatrixID, pid, pos, base, rotation, collider_radius);
  ans.set_moving(delta, glm::length(delta));
  ans.set_live_time(live_time, not undead);
  return ans;
}
