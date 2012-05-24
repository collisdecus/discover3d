#ifndef LEVEL_H
#define LEVEL_H

#include "global.h"
#include <QtCore/QVector>
#include <QtCore/QHash>
#include <QtCore/QMetaType>
#include <QtGui/QColor>
#include <QtGui/QVector3D>

enum Division { NoDivision = 0, TwoParts = 1, ThreePlusParts = 2 };
enum BlockFaceType { Top = 0, Bottom = 1, Left = 2, Right = 3, Front = 4, Back = 5};

class TextureManager;
class GLWidget;
struct Block;

struct BlockFace
{
  BlockFace()
    : _count(-1)
    , _division(NoDivision)
    , _type(Top)
    , _parentId(0)
    , _highlighted(false)
    , _locked(false) {}

  BlockFace(int parentId, BlockFaceType type, int count = -1, Division division = NoDivision)
    : _count(count)
    , _division(division)
    , _type(type)
    , _parentId(parentId)
    , _highlighted(false)
    , _locked(false) {}

  int _count;
  Division _division;
  BlockFaceType _type;
  int _parentId;
  bool _highlighted;
  bool _locked;
  QColor _resultColor;

  bool operator==(const BlockFace& other) const
  {
    return _count == other._count &&
           _division == other._division &&
           _highlighted == other._highlighted &&
           _locked == other._locked;
  }
};

Q_DECLARE_METATYPE(BlockFace)

inline uint qHash(const BlockFace& key)
{
  union { uint hash; qint8 part[4]; } result;

  result.part[0] = key._count;
  result.part[1] = key._division;
  result.part[2] = key._highlighted;
  result.part[3] = key._locked;

  return result.hash;
}

//! A single block in a level
struct Block
{
  Block()
    : _count(-1)
    , _id(0)
    , _exists(false)
    , _visible(false)
    , _highlighted(false)
    , _locked(false)
  {
  }

  void setFace(BlockFaceType type, const BlockFace& face)
  {
    _faces[(int)type] = face;
    _faces[(int)type]._parentId = _id;
  }

  BlockFace _faces[6];

  int _count;
  int _id;
  bool _exists;
  bool _visible;
  bool _highlighted;
  QColor _color;
  bool _locked;
};

Q_DECLARE_METATYPE(Block)

//! A complete level
class Level
{
  friend class LevelSerializer;

public:
  Level(GLWidget* parent = 0);
  Level(GLWidget* parent, const QString& filename);
  void saveLevel(const QString& filename, const QString &name, const QString &solution);
  void setBlocks(const QVector<QVector<QVector<Block> > >& newBlocks);
  Block& blockForId(int id);
  Block& block(int x, int y, int z);
  const Block &block(int x, int y, int z) const;
  Coordinate3D getBlockCoordinates(int id) const;
  bool isHighlighted(int id) const;

  void setFinished(bool finished);
  void highlightBlock(int id);
  void blockClicked(int id, Qt::MouseButton buttons);
  int maximumExtent();
  QString getName() const;
  QString getSolution() const;
  QVector<QVector<QVector<Block> > > getBlocks() const;
  bool isFinished() const;

private:
  bool isDone();
  void initBlocks();
  void extents(Coordinate3D& max, Coordinate3D& min);

  QHash<int, Coordinate3D> _coordinates;
  QVector<QVector<QVector<Block> > > _blocks;
  bool _finished;
  int _currentHighlight;
  GLWidget* _parentWidget;
  QString _name;
  QString _solution;
};

Q_DECLARE_METATYPE(Level)

#endif // LEVEL_H
