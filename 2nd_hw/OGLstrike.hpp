#pragma once

// Include standard headers
#include <stdio.h>
#include <stdlib.h>
#include <optional>
#include <functional>

// Include GLEW
#include <GL/glew.h>

// Include GLFW
#include <GLFW/glfw3.h>

// Include GLM
#include <glm/glm.hpp>
#include <shader.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

#include "Camera.hpp"
#include "Unit.hpp"

class OGLstrike {
 private:
 friend class Camera;

  struct  RandomSpawner{
    RandomSpawner(std::function<void()>  func, double period = 2);
    void spawn_unit(double current_time);

    std::function<void()> func;
    double lastTime;
    double period;
  };

 public:
  OGLstrike(const std::string& path = "my_save.txt");
  void init();
  void set_background_color(float red, float green, float blue, float alpha);
  void run();
  void spawn_enemy(glm::vec3 pos, glm::vec3 direction = {0, 0, 0}, GLfloat speed=5, glm::vec2 rotation = {0, 0});
  void spawn_enemy_random(int count = 1);
  void spawn_fireball(glm::vec3 pos, glm::vec3 direction = {0, 0, 0}, GLfloat speed=30);
  void step();
  ~OGLstrike();

 private:
  void change_detalization(int delta);
  void save();
  void load();
  std::string to_string();
  void from_string(const std::string& s);
  void process_collision();
  void process_dead();

 private:
  int detalization = 3;
  std::string path;
  ColoredUnit enemy;
//  ColoredUnit fireball;
  TexturedUnit fireball;

  std::vector<Unit> fireballs;
  std::vector<Unit> enemies;
//  GLuint g_vertex_buffer;
//  GLuint c_vertex_buffer;
  GLFWwindow *window;
  double time_speed = 1;
  GLuint VertexArrayID;
  GLuint enemy_pid;
  GLuint fireball_pid;
  GLuint MatrixID;
  double lastTime;
//  std::string temp;
  Camera camera;
  RandomSpawner enemy_spawner;
};

