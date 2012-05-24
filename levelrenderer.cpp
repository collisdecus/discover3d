#include "levelrenderer.h"
#include "global.h"

LevelRenderer::LevelRenderer(TextureManager *manager)
  : _textureManager(manager)
{
}

void LevelRenderer::render(const Level &level, RenderMode mode) const
{
  glTranslatef(-Global::levelSize, -Global::levelSize, -Global::levelSize);

  if (mode == DefaultMode)
    renderDefault(level);
  else
    renderSelect(level);
}

QHash<BlockFace, QList<BlockFace> > LevelRenderer::getFacesToRender(const Level &level) const
{
  QHash<BlockFace, QList<BlockFace> > result;

  for (int i = 0; i < Global::levelSize; ++i)
  {
    for (int j = 0; j < Global::levelSize; ++j)
    {
      for (int k = 0; k < Global::levelSize; ++k)
      {
        if (level.block(i,j,k)._visible)
        {
          for (int l = 0; l < 6; ++l)
          {
            BlockFace face = level.block(i,j,k)._faces[l];
            face._resultColor = level.block(i,j,k)._color;
            if (level.isHighlighted(level.block(i,j,k)._id))
              face._highlighted = true;
            if (level.block(i,j,k)._locked)
              face._locked = true;
            result[face].append(face);
          }
        }
      }
    }
  }

  return result;
}


void LevelRenderer::renderDefault(const Level &level) const
{
  if (!level.isFinished())
    glEnable(GL_TEXTURE_2D);

  QHash<BlockFace, QList<BlockFace> > facesToRender = getFacesToRender(level);

  foreach (BlockFace key, facesToRender.keys())
  {
    Texture tex(key._count, key._locked, key._highlighted, key._division);
    _textureManager->bindTexture(tex);

    glBegin(GL_QUADS);

    foreach (BlockFace face, facesToRender.value(key))
      renderFace(level, face);

    glEnd();
  }
}

void LevelRenderer::renderFace(const Level &level, const BlockFace &face) const
{
  QVector3D translate = level.getBlockCoordinates(face._parentId);
  translate *= 2;

  if (level.isFinished())
    glColor3f(face._resultColor.redF(), face._resultColor.greenF(), face._resultColor.blueF());

  renderQuad(face._type, translate);
}

void LevelRenderer::renderQuad(BlockFaceType type, const QVector3D& translate) const
{
  if (type == Top)
  {
    glNormal3f( 0.0f, 1.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f + translate.x(), 1.0f + translate.y(),-1.0f + translate.z());
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f + translate.x(), 1.0f + translate.y(),-1.0f + translate.z());
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f + translate.x(), 1.0f + translate.y(), 1.0f + translate.z());
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f + translate.x(), 1.0f + translate.y(), 1.0f + translate.z());
  }
  else if (type == Bottom)
  {
    glNormal3f( 0.0f,-1.0f, 0.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f + translate.x(),-1.0f + translate.y(), 1.0f + translate.z());
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f + translate.x(),-1.0f + translate.y(), 1.0f + translate.z());
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f + translate.x(),-1.0f + translate.y(),-1.0f + translate.z());
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f + translate.x(),-1.0f + translate.y(),-1.0f + translate.z());
  }
  else if (type == Front)
  {
    glNormal3f( 0.0f, 0.0f, 1.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f + translate.x(), 1.0f + translate.y(), 1.0f + translate.z());
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f + translate.x(), 1.0f + translate.y(), 1.0f + translate.z());
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f + translate.x(),-1.0f + translate.y(), 1.0f + translate.z());
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f + translate.x(),-1.0f + translate.y(), 1.0f + translate.z());
  }
  else if (type == Back)
  {
    glNormal3f( 0.0f, 0.0f,-1.0f);
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f + translate.x(),-1.0f + translate.y(),-1.0f + translate.z());
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f + translate.x(),-1.0f + translate.y(),-1.0f + translate.z());
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f + translate.x(), 1.0f + translate.y(),-1.0f + translate.z());
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f + translate.x(), 1.0f + translate.y(),-1.0f + translate.z());
  }
  else if (type == Left)
  {
    glNormal3f(-1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f + translate.x(), 1.0f + translate.y(), 1.0f + translate.z());
    glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f + translate.x(), 1.0f + translate.y(),-1.0f + translate.z());
    glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f + translate.x(),-1.0f + translate.y(),-1.0f + translate.z());
    glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f + translate.x(),-1.0f + translate.y(), 1.0f + translate.z());
  }
  else if (type == Right)
  {
    glNormal3f( 1.0f, 0.0f, 0.0f);
    glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f + translate.x(), 1.0f + translate.y(),-1.0f + translate.z());
    glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f + translate.x(), 1.0f + translate.y(), 1.0f + translate.z());
    glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f + translate.x(),-1.0f + translate.y(), 1.0f + translate.z());
    glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f + translate.x(),-1.0f + translate.y(),-1.0f + translate.z());
  }
}

void LevelRenderer::renderSelect(const Level &level) const
{
  for (int i = 0; i < Global::levelSize; ++i)
  {
    glTranslatef(i * 2.0f, 0.0f, 0.0f);
    for (int j = 0; j < Global::levelSize; ++j)
    {
      glTranslatef(0.0f, j * 2.0f, 0.0f);
      for (int k = 0; k < Global::levelSize; ++k)
      {
        glTranslatef(0.0f, 0.0f, k * 2.0f);
        if (level.block(i,j,k)._visible)
        {
          glPushName(level.block(i,j,k)._id);
          renderPlainBlock();
          glPopName();
        }
        glTranslatef(0.0f, 0.0f, -k * 2.0f);
      }
      glTranslatef(0.0f, -j * 2.0f, 0.0f);
    }
    glTranslatef(-i * 2.0f, 0.0f, 0.0f);
  }
}

void LevelRenderer::renderPlainBlock() const
{
  glBegin(GL_QUADS);

  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, 1.0f, 1.0f);

  glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 1.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,-1.0f,-1.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,-1.0f,-1.0f);

  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 1.0f);

  glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);
  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);

  glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f, 1.0f,-1.0f);
  glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,-1.0f,-1.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f,-1.0f, 1.0f);

  glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f, 1.0f,-1.0f);
  glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, 1.0f, 1.0f);
  glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f,-1.0f, 1.0f);
  glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,-1.0f,-1.0f);

  glEnd();
}
