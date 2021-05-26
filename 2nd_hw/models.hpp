#pragma once

namespace figure {
namespace enemy {
const std::vector<GLfloat> g_vertex_buffer_data = {
    1.0f, 0.0f, 0.0f,          // a
    0.0f, 1.0f, 0.0f,          // c
    0.0f, 0.0f, 1.0f,          // b

//      -0.7f, 0.0f, -0.7f,        // d
//      1.0f, 0.0f, 0.0f,          // a
//      0.0f, 0.0f, 1.0f,          // b

    -0.7f, 0.0f, -0.7f,        // d
    0.0f, 0.0f, 1.0f,          // b
    0.0f, 1.0f, 0.0f,          // c

    -0.7f, 0.0f, -0.7f,        // d
    0.0f, 1.0f, 0.0f,          // c
    1.0f, 0.0f, 0.0f,          // a

    1.0f, -1.0f, 0.0f,          // a1
    0.0f, -1.0f, 1.0f,          // b1
    -0.7f, -1.0f, -0.7f,        // d1

    1.0f, -1.0f, 0.0f,          // a1
    1.0f, 0.0f, 0.0f,           // a
    0.0f, -1.0f, 1.0f,          // b1
    1.0f, 0.0f, 0.0f,           // a
    0.0f, 0.0f, 1.0f,           // b
    0.0f, -1.0f, 1.0f,          // b1

    0.0f, -1.0f, 1.0f,          // b1
    -0.7f, 0.0f, -0.7f,        // d
    -0.7f, -1.0f, -0.7f,        // d1
    -0.7f, 0.0f, -0.7f,        // d
    0.0f, -1.0f, 1.0f,          // b1
    0.0f, 0.0f, 1.0f,          // b

    -0.7f, -1.0f, -0.7f,        // d1
    1.0f, 0.0f, 0.0f,           // a
    1.0f, -1.0f, 0.0f,          // a1
    1.0f, 0.0f, 0.0f,           // a
    -0.7f, -1.0f, -0.7f,        // d1
    -0.7f, 0.0f, -0.7f,        // d
};
const std::vector<GLfloat> c_vertex_buffer_data = {
    1.0f, 0.0f, 0.0f,          // a
    1.0f, 0.0f, 0.0f,          // c
    1.0f, 0.0f, 0.0f,          // b

//      0.5f, 0.5f, 0.5f,          // d
//      0.5f, 0.5f, 0.5f,          // a
//      0.5f, 0.5f, 0.5f,          // b

    0.0f, 0.0f, 1.0f,          // d
    0.0f, 0.0f, 1.0f,          // b
    0.0f, 0.0f, 1.0f,          // c

    0.0f, 1.0f, 0.0f,          // d
    0.0f, 1.0f, 0.0f,          // c
    0.0f, 1.0f, 0.0f,          // a

    0.5f, 0.5f, 0.5f,          // a1
    0.5f, 0.5f, 0.5f,          // b1
    0.5f, 0.5f, 0.5f,          // d1

    0.5f, 0.1f, 0.1f,          // a1
    0.5f, 0.1f, 0.1f,          // a
    0.5f, 0.1f, 0.1f,          // b1
    0.5f, 0.1f, 0.1f,          // a
    0.5f, 0.1f, 0.1f,          // b
    0.5f, 0.1f, 0.1f,          // b1

    0.1f, 0.1f, 0.5f,          // b1
    0.1f, 0.1f, 0.5f,          // d
    0.1f, 0.1f, 0.5f,          // d1
    0.1f, 0.1f, 0.5f,          // d
    0.1f, 0.1f, 0.5f,          // b1
    0.1f, 0.1f, 0.5f,          // b

    0.1f, 0.5f, 0.1f,          // d1
    0.1f, 0.5f, 0.1f,          // a
    0.1f, 0.5f, 0.1f,          // a1
    0.1f, 0.5f, 0.1f,          // a
    0.1f, 0.5f, 0.1f,          // d1
    0.1f, 0.5f, 0.1f,          // d
};
}
namespace fireball {
static const float EPS = 1e-6;

void get_sphere(std::vector<glm::vec3> &verteces, std::vector<glm::vec2> &uvs, float radius, size_t parallel_count, bool right = false) {
  auto vertex_by_angles = [&radius](float vertical_angle, float horizontal_angle) {
    return glm::vec3(sinf(vertical_angle) * sinf(horizontal_angle),
                     -cosf(vertical_angle),
                     sinf(vertical_angle) * cosf(horizontal_angle)) * radius;
  };
  auto angles_by_vertex = [&radius](glm::vec3 point) {
    float vertical_angle = acos(point.y / radius);
    float horizontal_angle = asin(point.x / radius / sinf(vertical_angle));
    if (std::isnan(horizontal_angle)){
      horizontal_angle = 0;
    }
    return glm::vec2(vertical_angle, horizontal_angle);
  };

  verteces.clear();
  uvs.clear();

  size_t meridian_count = parallel_count;
  std::vector<float> vertical_angles(parallel_count + 1);
  for (int i = 0; i < parallel_count + 1; ++i) {
    vertical_angles[i] = M_PI * (i) / (parallel_count);
  }
//  for (auto i : vertical_angles) {
//    std::cout << i << "\t";
//  }
  std::vector<glm::vec3> cur;
  std::vector<glm::vec3> prev;

  for (int i = 0; i < meridian_count; ++i) {
    cur.push_back(vertex_by_angles(0, i * 2 * M_PI / meridian_count));
  }

  float step = M_PI * 2 / meridian_count;
  float start = 0;

  for (auto angle : vertical_angles) {
    if (right) {
      start -= M_PI / meridian_count;
    }
    else{
      start +=  M_PI / meridian_count;
    }

    prev = cur;
    cur.clear();
    glm::vec3 prev_v = prev.back();
    float cur_horisontal_angle = start;
    for (auto cur_v : prev) {
      cur.push_back(vertex_by_angles(angle, cur_horisontal_angle));
      cur_horisontal_angle += step;
    }
    auto prev_up = cur.back();
    auto prev_down = prev.back();
    for (int i = 0; i < meridian_count; ++i) {
      auto cur_up = cur[i];
      auto cur_down = prev[i];
      if (cur_up != prev_up) {
        verteces.push_back(prev_up);
        uvs.emplace_back(angles_by_vertex(prev_up));
        verteces.push_back(prev_down);
        uvs.emplace_back(angles_by_vertex(prev_down));
        verteces.push_back(cur_up);
        uvs.emplace_back(angles_by_vertex(cur_up));
      }

      if (cur_down != prev_down) {
        verteces.push_back(prev_down);
        uvs.emplace_back(angles_by_vertex(prev_down));
        verteces.push_back(cur_down);
        uvs.emplace_back(angles_by_vertex(cur_down));
        verteces.push_back(cur_up);
        uvs.emplace_back(angles_by_vertex(cur_up));
      }

      prev_down = cur_down;
      prev_up = cur_up;
    }
  }
}
}

}
