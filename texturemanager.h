#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H

#include <QtCore/QHash>
#include <QtGui/QImage>
#include <GL/gl.h>
#include "level.h"

class GLWidget;

struct Texture
{
  Texture(qint8 num, bool lock, bool highlight, Division div) :
    number(num),
    locked(lock),
    highlighted(highlight),
    division(div) {}

  bool operator==(const Texture& other) const
  {
    return number == other.number &&
           locked == other.locked &&
           highlighted == other.highlighted &&
           division == other.division;
  }

  int number;
  bool locked;
  bool highlighted;
  Division division;
};

class TextureManager
{
public:
  TextureManager(GLWidget *glWidget);
  void init();
  QPixmap mergeImages(const QPixmap &baseImage, const QPixmap &overlayImage) const;
  void bindTexture(const Texture& texture);

private:
  QPixmap background(bool locked, bool highlighted) const;

  QHash<Texture, int> _textureIds;
  GLWidget* _glWidget;
};

inline uint qHash(const Texture &key)
{
  union { uint hash; qint8 part[4]; } result;

  result.part[0] = key.number;
  result.part[1] = key.locked;
  result.part[2] = key.highlighted;
  result.part[3] = key.division;

  return result.hash;
}

#endif // TEXTUREMANAGER_H
