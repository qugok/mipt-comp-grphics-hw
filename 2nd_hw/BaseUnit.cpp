#include <texture.hpp>
#include "BaseUnit.hpp"

ColoredUnit::ColoredUnit(const std::vector<GLfloat> &pos, const std::vector<GLfloat> &color) {
  pos_vertexes.resize(pos.size() / 3);
  color_vertexes.resize(pos.size() / 3);
  for (int i = 0; i < pos.size() / 3; ++i) {
    pos_vertexes[i] = {pos[i * 3], pos[i * 3 + 1], pos[i * 3 + 2]};
    color_vertexes[i] = {color[i * 3], color[i * 3 + 1], color[i * 3 + 2]};
  }
  g_vertexbuffer = GLint();
  c_vertexbuffer = GLint();
  glGenBuffers(1, &g_vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, g_vertexbuffer);
  glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(*pos_vertexes.data()) * pos_vertexes.size(),
      pos_vertexes.data(),
      GL_STATIC_DRAW);

  glGenBuffers(1, &c_vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, c_vertexbuffer);
  glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(*color_vertexes.data()) * color_vertexes.size(),
      color_vertexes.data(),
      GL_STATIC_DRAW);
}
void ColoredUnit::render() {
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, g_vertexbuffer);
  glVertexAttribPointer(
      0,                  // attribute. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void *) 0            // array buffer offset
  );
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, c_vertexbuffer);
  glVertexAttribPointer(
      1,                  // attribute. No particular reason for 0, but must match the layout in the shader.
      3,                  // size
      GL_FLOAT,           // type
      GL_FALSE,           // normalized?
      0,                  // stride
      (void *) 0            // array buffer offset
  );
  glDrawArrays(GL_TRIANGLES,
               0,
               static_cast<GLsizei>(color_vertexes.size()));

  glDisableVertexAttribArray(1);
  glDisableVertexAttribArray(0);
}
ColoredUnit::~ColoredUnit() {
  glDeleteBuffers(1, &g_vertexbuffer);
  glDeleteBuffers(1, &c_vertexbuffer);
}
ColoredUnit::ColoredUnit(ColoredUnit &&other)
    : c_vertexbuffer(other.c_vertexbuffer),
      g_vertexbuffer(other.g_vertexbuffer),
      color_vertexes(std::move(other.color_vertexes)),
      pos_vertexes(std::move(other.pos_vertexes)) {
  other.g_vertexbuffer = -1;
  other.c_vertexbuffer = -1;
}

ColoredUnit &ColoredUnit::operator=(ColoredUnit &&other) {
  c_vertexbuffer = other.c_vertexbuffer;
  g_vertexbuffer = other.g_vertexbuffer;
  color_vertexes = std::move(other.color_vertexes);
  pos_vertexes = std::move(other.pos_vertexes);
  other.g_vertexbuffer = -1;
  other.c_vertexbuffer = -1;
  return *this;
}
ColoredUnit::ColoredUnit(const std::vector<glm::vec3> &pos, const std::vector<glm::vec3> &color)
    : pos_vertexes(pos), color_vertexes(color) {
  g_vertexbuffer = GLint();
  c_vertexbuffer = GLint();
  glGenBuffers(1, &g_vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, g_vertexbuffer);
  glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(*pos_vertexes.data()) * pos_vertexes.size(),
      pos_vertexes.data(),
      GL_STATIC_DRAW);

  glGenBuffers(1, &c_vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, c_vertexbuffer);
  glBufferData(
      GL_ARRAY_BUFFER,
      sizeof(*color_vertexes.data()) * color_vertexes.size(),
      color_vertexes.data(),
      GL_STATIC_DRAW);
}
TexturedUnit::TexturedUnit(GLuint pid, const std::vector<glm::vec3>& vertices, const std::string &path_texture, const std::vector<glm::vec2>& uvs) {

  pos_vertexes = vertices;
  uvs_vertexes = uvs;

  Texture = loadDDS("textures/lava.dds");
  TextureID = glGetUniformLocation(pid, "myTextureSampler");

  glGenBuffers(1, &vertexBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(*pos_vertexes.data()) * pos_vertexes.size(),
               pos_vertexes.data(), GL_STATIC_DRAW);

  glGenBuffers(1, &uvBuffer);
  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  glBufferData(GL_ARRAY_BUFFER,
               sizeof(*uvs_vertexes.data()) * uvs_vertexes.size(),
               uvs_vertexes.data(),
               GL_STATIC_DRAW);
}
TexturedUnit::~TexturedUnit() {
  glDeleteBuffers(1, &vertexBuffer);
  glDeleteBuffers(1, &uvBuffer);
  glDeleteTextures(1, &Texture);
}

void TexturedUnit::render() {
 int ret = 0;
  glActiveTexture(GL_TEXTURE0);
  glBindTexture(GL_TEXTURE_2D, Texture);
  // Set our "myTextureSampler" sampler to use Texture Unit 0
  glUniform1i(TextureID, 0);

  // 1rst attribute buffer : vertices
  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexAttribPointer(0,         // attribute
                        3,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void*)0   // array buffer offset
  );

  // 2nd attribute buffer : UVs
  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, uvBuffer);
  glVertexAttribPointer(1,         // attribute
                        2,         // size
                        GL_FLOAT,  // type
                        GL_FALSE,  // normalized?
                        0,         // stride
                        (void*)0   // array buffer offset
  );

  // Draw the triangle !
  glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei> (pos_vertexes.size()));

  glDisableVertexAttribArray(0);
  glDisableVertexAttribArray(1);
}
TexturedUnit &TexturedUnit::operator=(TexturedUnit && other) {
  vertexBuffer = other.vertexBuffer;
  uvBuffer = other.uvBuffer;
  TextureID = other.TextureID;
  Texture = other.Texture;
  pos_vertexes = std::move(other.pos_vertexes);
  uvs_vertexes = std::move(other.uvs_vertexes);
  other.vertexBuffer = -1;
  other.uvBuffer = -1;
  other.TextureID = -1;
  other.Texture = -1;
  return *this;
}
void BaseUnit::render() {
  throw "error";
}
