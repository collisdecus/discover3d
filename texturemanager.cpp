#include "texturemanager.h"
#include "glwidget.h"
#include <QtGui/QPainter>
#include <QtOpenGL/QGLWidget>
#include <GL/gl.h>
#include <GL/glu.h>

// has to be global or else QGLWidget crashes (?)
GLint textureNames[144];

const QString numbers[] = { ":/assets/nnull.png",
                            ":/assets/none.png",
                            ":/assets/ntwo.png",
                            ":/assets/nthree.png",
                            ":/assets/nfour.png",
                            ":/assets/nfive.png",
                            ":/assets/nsix.png",
                            ":/assets/nseven.png",
                            ":/assets/neight.png",
                            ":/assets/nnine.png",
                            ":/assets/nten.png" };


TextureManager::TextureManager(GLWidget* glWidget)
{
  _glWidget = glWidget;
}

void TextureManager::init()
{
  int id = 0;

  for (int i = -1; i < 11; ++i)
  {
    for (int j = 0; j < 2; ++j)
    {
      for (int k = 0; k < 2; ++k)
      {
        for (int l = 0; l < 3; ++l)
        {
          Texture tex(i, j, k, (Division)l);
          QPixmap result = background(tex.locked, tex.highlighted);

          if (tex.division == TwoParts)
            result = mergeImages(result, QPixmap(":/assets/two.png"));
          else if (tex.division == ThreePlusParts)
            result = mergeImages(result, QPixmap(":/assets/threeplus.png"));

          if (i >= 0)
          {
            QPixmap number(numbers[i]);
            result = mergeImages(result, number);
          }

          #ifndef Q_WS_WIN
          {
            QTransform transform;
            result = result.transformed(transform.rotate( -180,Qt::XAxis ), Qt::FastTransformation);
          }
          #endif

          pixmaps.append(result);
          _textureIds[tex] = id;
          textureNames[id] = _glWidget->bindTexture(result, GL_TEXTURE_2D);
          ++id;
        }
      }
    }
  }
}

QPixmap TextureManager::mergeImages(const QPixmap &baseImage, const QPixmap &overlayImage) const
{
  QPixmap imageWithOverlay = QPixmap(baseImage.size());
  QPainter painter(&imageWithOverlay);

  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  painter.drawPixmap(0, 0, baseImage);

  painter.setCompositionMode(QPainter::CompositionMode_SourceOver);
  painter.drawPixmap(0, 0, overlayImage);

  painter.end();

  return imageWithOverlay;
}

void TextureManager::bindTexture(const Texture &texture)
{
  glBindTexture(GL_TEXTURE_2D, textureNames[_textureIds.value(texture)]);
}

QPixmap TextureManager::background(bool locked, bool highlighted) const
{
  if (locked && highlighted)
    return QPixmap(":/assets/back_marked_highlighted.png");
  else if (locked && !highlighted)
    return QPixmap(":/assets/back_marked.png");
  else if (!locked && highlighted)
    return QPixmap(":/assets/back_highlighted.png");
  else
    return QPixmap(":/assets/back.png");
}
