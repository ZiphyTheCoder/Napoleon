#ifndef SOLDIER_H
#define SOLDIER_H

#include "stb_image.h"

#include <glad/glad.h>
#include <string>
#include <iostream>
#include <vector>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Soldier {
public:
  GLuint ID;
  int width, height, nrChannels;
  bool isSelected;

  glm::vec2 velocity;
  glm::vec2 position;
  glm::vec4 color;

  Soldier();
  void TexSetup(const std::string& path);
  void Bind(GLuint slot = 0) const;
  void Unbind() const;
  void Delete();
  std::vector<float> loadRect(float width, float height);
  unsigned int indices[];

  

private:
  void LoadFromFile(const std::string& filePath);
};

#endif