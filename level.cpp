#include "level.h"
#include "texturemanager.h"
#include "glwidget.h"
#include "levelserializer.h"
#include <QtCore/QSettings>
#include <QtGui/QMessageBox>

Level::Level(GLWidget *parent) :
  _finished(false),
  _parentWidget(parent)
{
  initBlocks();
}

Level::Level(GLWidget* parent, const QString& filename) :
  _finished(false)
{
  *this = LevelSerializer::load(filename);
  _parentWidget = parent;
}

void Level::saveLevel(const QString &filename, const QString& name, const QString& solution)
{
  _name = name;
  _solution = solution;

  LevelSerializer::save(*this, filename);
}

void Level::setBlocks(const QVector<QVector<QVector<Block> > > &newBlocks)
{
  _blocks = newBlocks;
}

void Level::setFinished(bool finished)
{
  _finished = finished;
}

void Level::highlightBlock(int id)
{
  _currentHighlight = id;
}

void Level::blockClicked(int id, Qt::MouseButton button)
{
  if (_finished)
    return;

  if (id < 0)
    return;

  QSettings settings;
  Qt::MouseButton markButton = (Qt::MouseButton)settings.value(SettingsKey::markButton).toInt();

  if (button == markButton)
  {
    blockForId(id)._locked = !blockForId(id)._locked;
  }
  else
  {
    if (!blockForId(id)._locked)
    {
      if (blockForId(id)._exists)
      {
        _parentWidget->addPenalty();
        blockForId(id)._locked = true;
      }
      else
      {
        blockForId(id)._visible = false;
      }
    }
  }

  if (isDone())
  {
    _finished = true;
    glDisable(GL_TEXTURE_2D);
    _parentWidget->finish();
  }
}

int Level::maximumExtent()
{
  Coordinate3D max, min;
  extents(max, min);
  return qMax(max._x, qMax(max._y, max._z));
}

QString Level::getName() const
{
  return _name;
}

QString Level::getSolution() const
{
  return _solution;
}

QVector<QVector<QVector<Block> > > Level::getBlocks() const
{
  return _blocks;
}

bool Level::isFinished() const
{
  return _finished;
}

bool Level::isDone()
{
  for (int i = 0; i < Global::levelSize; ++i)
  {
    for (int j = 0; j < Global::levelSize; ++j)
    {
      for (int k = 0; k < Global::levelSize; ++k)
      {
        if (!block(i,j,k)._exists && block(i,j,k)._visible)
          return false;
      }
    }
  }

  return true;
}

void Level::initBlocks()
{
  int id = 0;
  _blocks.resize(Global::levelSize);

  for (int i = 0; i < Global::levelSize; ++i)
  {
    if (_blocks.at(i).isEmpty())
      _blocks[i].resize(Global::levelSize);

    for (int j = 0; j < Global::levelSize; ++j)
    {
      if (_blocks.at(i).at(j).isEmpty())
        _blocks[i][j].resize(Global::levelSize);

      for (int k = 0; k < Global::levelSize; ++k)
      {
        _blocks[i][j][k] = Block();
        _blocks[i][j][k]._id = id;
        _coordinates[id] = Coordinate3D(i,j,k);
        id++;
      }
    }
  }
}

void Level::extents(Coordinate3D& max, Coordinate3D& min)
{
  int minI = INT_MAX;
  int maxI = 0;
  int minJ = INT_MAX;
  int maxJ = 0;
  int minK = INT_MAX;
  int maxK = 0;

  for (int i = 0; i < Global::levelSize; ++i)
  {
    for (int j = 0; j < Global::levelSize; ++j)
    {
      for (int k = 0; k < Global::levelSize; ++k)
      {
        if (block(i,j,k)._exists)
        {
          minI = qMin(minI, i); maxI = qMax(maxI, i);
          minJ = qMin(minJ, j); maxJ = qMax(maxJ, j);
          minK = qMin(minK, k); maxK = qMax(maxK, k);
        }
      }
    }
  }

  max = Coordinate3D(maxI - minI, maxJ - minJ, maxK - minK);
  min = Coordinate3D(minI, minJ, minK);
}

Block& Level::blockForId(int id)
{
  Coordinate3D coord = _coordinates.value(id);
  return block(coord._x, coord._y, coord._z);
}

Block& Level::block(int x, int y, int z)
{
  return _blocks[x][y][z];
}

const Block &Level::block(int x, int y, int z) const
{
  return _blocks.at(x).at(y).at(z);
}

Coordinate3D Level::getBlockCoordinates(int id) const
{
  return _coordinates.value(id);
}

bool Level::isHighlighted(int id) const
{
  return _currentHighlight == id;
}
