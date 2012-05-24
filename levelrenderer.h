#ifndef LEVELRENDERER_H
#define LEVELRENDERER_H

#include "level.h"
#include "texturemanager.h"

enum RenderMode { DefaultMode, SelectMode };

class LevelRenderer
{
public:
  LevelRenderer(TextureManager* manager);
  void render(const Level& level, RenderMode mode) const;

private:
  void renderDefault(const Level& level) const;
  void renderSelect(const Level& level) const;
  void renderFace(const Level& level, const BlockFace& face) const;
  QHash<BlockFace, QList<BlockFace> > getFacesToRender(const Level& level) const;
  void renderQuad(BlockFaceType type, const QVector3D& translation) const;
  void renderPlainBlock() const;

  TextureManager* _textureManager;
};

#endif // LEVELRENDERER_H
