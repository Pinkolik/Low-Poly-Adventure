#define STB_IMAGE_IMPLEMENTATION
#include "texture.h"
#include "stb_image.h"
#include <glad/glad.h>
#include <stdexcept>

Texture::Texture(const char *texturePath) {
  glGenTextures(1, &id);

  int width, height, nrChannels;
  glBindTexture(GL_TEXTURE_2D, id);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                  GL_NEAREST_MIPMAP_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
  if (data) {
    if (nrChannels == 3) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                   GL_UNSIGNED_BYTE, data);
    } else if (nrChannels == 4) {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                   GL_UNSIGNED_BYTE, data);
    }
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    throw std::runtime_error("Failed to load image. " +
                             std::string(texturePath));
  }

  stbi_image_free(data);
}

unsigned int Texture::getId() { return id; }

void Texture::active(int offset) {
  glActiveTexture(GL_TEXTURE0 + offset);
  glBindTexture(GL_TEXTURE_2D, id);
}