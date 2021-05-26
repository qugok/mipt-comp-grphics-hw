#include "OGLstrike.hpp"

int main() {
  OGLstrike s;
  s.init();
  s.spawn_enemy_random(15);
  s.run();
  return 0;
}
