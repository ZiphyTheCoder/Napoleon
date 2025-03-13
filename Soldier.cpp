#include "Soldier.h"

Soldier::Soldier() {
  ID = 0;
  width = height = nrChannels = 0;
  std::cout << "Default Soldier created" << std::endl;
}


void Soldier::TexSetup(const std::string& path)
{
  glGenTextures(1, &ID);
  glBindTexture(GL_TEXTURE_2D, ID);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  LoadFromFile(path);
}

void Soldier::LoadFromFile(const std::string& filePath) {
  stbi_set_flip_vertically_on_load(true); // Flip the image to match OpenGL's origin
  unsigned char* data = stbi_load(filePath.c_str(), &width, &height, &nrChannels, 0);

  if (data) {
    GLenum format = (nrChannels == 4) ? GL_RGBA : GL_RGB;
    glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  }
  else {
    std::cerr << "Failed to load texture: " << filePath << std::endl;
  }

  stbi_image_free(data);
}

void Soldier::Bind(GLuint slot) const {
  glActiveTexture(GL_TEXTURE0 + slot);
  glBindTexture(GL_TEXTURE_2D, ID);
}

void Soldier::Unbind() const {
  glBindTexture(GL_TEXTURE_2D, 0);
}

void Soldier::Delete() {
  glDeleteTextures(1, &ID);
}


std::vector<float> Soldier::loadRect(float width, float height)
{
  return {
    // First triangle: positions (x,y) and texture coordinates (u,v)
    -width / 2.0f, -0.27f,    0.0f, 0.0f,  // Bottom-left
    width / 2.0f, -0.27f,   1.0f, 0.0f,  // Bottom-right
    width / 2.0f, height - 0.27f, 1.0f, 1.0f,  // Top-right

    // Second triangle
    -width / 2.0f, -0.27f,    0.0f, 0.0f,  // Bottom-left
    width / 2.0f, height - 0.27f, 1.0f, 1.0f,  // Top-right
    -width / 2.0f, height - 0.27f,  0.0f, 1.0f   // Top-left
  };
}

glm::vec2 getUnitVer(glm::vec2 vec)
{
  glm::vec2 unit;
  float magnitude = sqrt((vec.x * vec.x) + (vec.y) * (vec.y));

  return unit = vec / magnitude;
}