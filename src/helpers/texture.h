#ifndef TEXTURE_H
#define TEXTURE_H

class Texture {
public:
  Texture(const char *texturePath);
  unsigned int getId();
  void active(int offset);

private:
  unsigned int id;
};
#endif