#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
  Texture(const char *texturePath);
  unsigned int getId();
  void active();

private:
  unsigned int id;
};
#endif