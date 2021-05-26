#include <iostream>
#include <algorithm>
#include "OGLstrike.hpp"
#include "models.hpp"
#include <random>
#include <utility>
#include <sstream>
#include <fstream>

void OGLstrike::init() {
  std::cout << "controls: WASD or arrows\n"
               "lshift - up; lctrl - down; or mouse scroll\n"
               "left mouse - fire\n"
               "page up - time speed up; page down - time speed down; \\ - reset time speed\n"
               "+ - detalization up; - - detalization down;\n"
               "[ - save scene to file; ] - load scene from file\n";
//  ShowCursor(false);
  if (!glfwInit()) {
    fprintf(stderr, "Failed to initialize GLFW\n");
    getchar();
    throw std::exception();
  }

  glfwWindowHint(GLFW_SAMPLES, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

  // Open a window and create its OpenGL context
  window = glfwCreateWindow(1024, 768, "HW2", NULL, NULL);
  if (window == NULL) {
    fprintf(stderr,
            "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n");
    getchar();
    glfwTerminate();
    throw std::exception();
  }
  glfwMakeContextCurrent(window);

  // Initialize GLEW
  glewExperimental = true; // Needed for core profile
  if (glewInit() != GLEW_OK) {
    fprintf(stderr, "Failed to initialize GLEW\n");
    getchar();
    glfwTerminate();
    throw std::exception();
  }

  // Ensure we can capture the escape key being pressed below
  glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
//  glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
//  set_background_color(0.0f, 0.0f, 0.4f, 0.0f);
  set_background_color(0.0f, 0.5f, 0.4f, 0.5f);

  glEnable(GL_DEPTH_TEST);

  // Отсечение тех треугольников, нормаль которых направлена от камеры
  glEnable(GL_CULL_FACE);
//   Accept fragment if it closer to the camera than the former one
  glDepthFunc(GL_LESS);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glGenVertexArrays(1, &VertexArrayID);
  glBindVertexArray(VertexArrayID);

  enemy_pid = LoadShaders("shaders/main.vertexshader", "shaders/default.fragmentshader");
  fireball_pid = LoadShaders("shaders/textured.vertexshader", "shaders/textured.fragmentshader");

  enemy = std::move(ColoredUnit(figure::enemy::g_vertex_buffer_data, figure::enemy::c_vertex_buffer_data));

  std::vector<glm::vec3> pos_fireball;
//  std::vector<glm::vec3> color_fireball;
  std::vector<glm::vec2> uvs_fireball;
  figure::fireball::get_sphere(pos_fireball, uvs_fireball, 1, detalization, false);
//  for(auto i : pos_fireball){
//    std::cout << i.x << i.y << i.z << " \t\t ";
//  }
//  std::cout << "\n";
//  for(auto i : uvs_fireball){
//    std::cout << i.x << " " << i.y << "\t\t\t";
//  }

  fireball = std::move(TexturedUnit(fireball_pid, pos_fireball, "textures/lava.dds", uvs_fireball));

  MatrixID = glGetUniformLocation(enemy_pid, "MVP");

  camera.init(window);

  glfwSetWindowUserPointer(window, this);

  glfwSetMouseButtonCallback(window, [](GLFWwindow *window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
      auto eng = static_cast<OGLstrike *>(glfwGetWindowUserPointer(window));
      auto position = eng->camera.get_position();
      auto direction = eng->camera.get_direction();
      std::random_device rd;
      std::default_random_engine e(rd());
      std::uniform_real_distribution<> r(0., 1.);
      auto spray = glm::cross(glm::vec3(r(e), r(e), r(e)), direction);
      spray *= r(e) * 0.3 / glm::length(spray);
      eng->spawn_fireball(position + direction * 2.f, direction + spray, 30);
//      eng->spawn_fireball(position + direction * 2.f, direction, 0);
    }
  });

  glfwSetKeyCallback(window,[](GLFWwindow * window, int key, int scancode, int action, int mods){
    if (key == GLFW_KEY_LEFT_BRACKET && action == GLFW_PRESS){
      auto eng = static_cast<OGLstrike *>(glfwGetWindowUserPointer(window));
//      eng->temp = eng->to_string();
      eng->save();
    }
    if (key == GLFW_KEY_RIGHT_BRACKET && action == GLFW_PRESS){
      auto eng = static_cast<OGLstrike *>(glfwGetWindowUserPointer(window));
//      eng->from_string(eng->temp);
      eng->load();
    }
    if (key == GLFW_KEY_PAGE_UP){// && action == GLFW_PRESS){
      auto eng = static_cast<OGLstrike *>(glfwGetWindowUserPointer(window));
//      eng->from_string(eng->temp);
      eng->time_speed *= 1.3;
    }
    if (key == GLFW_KEY_PAGE_DOWN){// && action == GLFW_PRESS){
      auto eng = static_cast<OGLstrike *>(glfwGetWindowUserPointer(window));
//      eng->from_string(eng->temp);
      eng->time_speed /= 1.3;
    }
    if (key == GLFW_KEY_BACKSLASH && action == GLFW_PRESS){
      auto eng = static_cast<OGLstrike *>(glfwGetWindowUserPointer(window));
//      eng->from_string(eng->temp);
      eng->time_speed = 1;
    }

    if (key == 	GLFW_KEY_MINUS && action == GLFW_PRESS){
      auto eng = static_cast<OGLstrike *>(glfwGetWindowUserPointer(window));
      eng->change_detalization(-1);

    }
    if (key == 	GLFW_KEY_EQUAL && action == GLFW_PRESS){
      auto eng = static_cast<OGLstrike *>(glfwGetWindowUserPointer(window));
      eng->change_detalization(1);
    }
  });
}

void OGLstrike::set_background_color(float red, float green, float blue, float alpha) {
  // Dark blue background
  glClearColor(red, green, blue, alpha);
}

void OGLstrike::spawn_fireball(glm::vec3 pos, glm::vec3 direction, GLfloat speed) {
  fireballs.emplace_back(MatrixID, fireball_pid, pos, &fireball);
//  fireballs.emplace_back(MatrixID, enemy_pid, pos, &fireball);
  (fireballs.rbegin())->set_moving(direction, speed);
  (fireballs.rbegin())->set_live_time(10, true);
}

void OGLstrike::spawn_enemy(
    glm::vec3 pos, glm::vec3 direction, GLfloat speed, glm::vec2 rotation) {

  enemies.emplace_back(MatrixID, enemy_pid, pos, &enemy, rotation);
  (enemies.rbegin())->set_moving(direction, speed);
}

OGLstrike::~OGLstrike() {
  glDeleteProgram(enemy_pid);
  glDeleteProgram(fireball_pid);
//  glDeleteBuffers(1, &g_vertex_buffer);
//  glDeleteBuffers(1, &c_vertex_buffer);
  glDeleteVertexArrays(1, &VertexArrayID);

  glfwTerminate();
}
void OGLstrike::step() {

  process_collision();
  process_dead();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  double currentTime = glfwGetTime();
  float deltaTime = float(currentTime - lastTime);
  lastTime = currentTime;

  enemy_spawner.spawn_unit(time_speed * currentTime);
  // изменение скорости течения времени не затрагивает камеру чтобы можно было рассмотреть со всех сторон
  camera.Update(deltaTime);

  for (auto &enem : enemies) {
    enem.render(camera, time_speed * deltaTime);
  }
  for (auto &firebal : fireballs) {
    firebal.render(camera, time_speed * deltaTime);
  }
  // Swap buffers
  glfwSwapBuffers(window);
  glfwPollEvents();

}
void OGLstrike::run() {
  do {
    step();
  } while (
      glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
          glfwWindowShouldClose(window) == 0);
}
void OGLstrike::process_collision() {
  for (int i = 0; i < fireballs.size(); ++i){
    for (int j = 0; j < enemies.size(); ++j) {
      if (fireballs[i].collision(enemies[j])){
        fireballs.erase(fireballs.begin() + i);
        enemies.erase(enemies.begin() + j);
        i--;
        break;
      }
    }
  }
}
void OGLstrike::process_dead() {
  for (int i = 0; i < fireballs.size(); ++i) {
    if (fireballs[i].is_dead())
    {
      fireballs.erase(fireballs.begin() + i);
      i--;
    }
  }
}
void OGLstrike::spawn_enemy_random(int count) {
  if (count > 1){
    for (int i = 0; i < count; ++i) {
      spawn_enemy_random();
    }
    return;
  }
  std::random_device rd;
  std::default_random_engine eng(rd());
  std::uniform_real_distribution<> speed(0.5, 5);
  std::uniform_real_distribution<> direction(0., 1.);
  std::uniform_int_distribution<> position(-25, 25);
  std::uniform_real_distribution<> rotation(-M_PI,M_PI);

  spawn_enemy(
      {position(eng), position(eng), position(eng)},
      {direction(eng), direction(eng), direction(eng)},
      speed(eng),
      {rotation(eng), rotation(eng)}
      );
}
OGLstrike::OGLstrike(const std::string& path)
: enemy_spawner([this](){this->spawn_enemy_random();}, 2)
, path(path)
{}
std::string OGLstrike::to_string() {

  std::ostringstream out;
  out << fireballs.size() << " " << enemies.size() << "\n";
  for (const auto& i : fireballs){
    out << i.to_string() << "\n";
  }

  for (const auto& i : enemies){
    out << i.to_string() << "\n";
  }

  return out.str();
}
void OGLstrike::from_string(const std::string &s) {
  std::istringstream in(s);
  int enem, fireba;
  std::string tmp;
  in  >> fireba >> enem;
  fireballs.clear();
  enemies.clear();
  for (int i = 0; i < fireba; ++i) {
    getline(in, tmp);
    fireballs.push_back(Unit::from_string(tmp, MatrixID, fireball_pid, &fireball));
  }
  for (int i = 0; i < enem; ++i) {
    getline(in, tmp);
    enemies.push_back(Unit::from_string(tmp, MatrixID, enemy_pid, &enemy));
  }
}
void OGLstrike::save() {
  std::ofstream out(path);
  out << this->to_string();
}
void OGLstrike::load() {
  std::ifstream in(path);
  std::string str((std::istreambuf_iterator<char>(in)),
                  std::istreambuf_iterator<char>());
  this->from_string(str);
}
void OGLstrike::change_detalization(int delta) {
  if (detalization + delta < 3){
    return;
  }
  detalization += delta;
  std::vector<glm::vec3> pos_fireball;
  std::vector<glm::vec2> uvs_fireball;
  figure::fireball::get_sphere(pos_fireball, uvs_fireball, 1, detalization, false);
  fireball = std::move(TexturedUnit(fireball_pid, pos_fireball, "textures/lava.dds", uvs_fireball));
}

OGLstrike::RandomSpawner::RandomSpawner(std::function<void()> func, double period) :func(std::move(func)), period(period){
  lastTime = 0;
}
void OGLstrike::RandomSpawner::spawn_unit(double current_time) {
//  std::cout << lastTime  << " " << period << " " << current_time << " ";
  if (lastTime + period > current_time){
    return;
  }
  lastTime = current_time;
  func();
}
