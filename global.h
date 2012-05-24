#ifndef GLOBAL_H
#define GLOBAL_H

#include <QtCore/QString>
#include <QtGui/QVector3D>

namespace SettingsKey
{
  // Settings keys
  const QString markButton = "mark_button";
  const QString highscoresScoresPrefix = "scores_";
  const QString highscoresNamesPrefix = "names_";
  const QString levelsDone = "levels_done";
  const QString windowGeometry = "window_geometry";
}

namespace Global
{
  // Other globals
  const QString levelsDirectory = "levels";
  const QString levelPartBreak = "-";
  const int highscoresEntries = 10;
  const int levelSize = 10;
  const int maximumPenalty = 8*60; // 8 minutes
}

struct Coordinate3D
{
  Coordinate3D() : _x(0), _y(0), _z(0) {}
  Coordinate3D(int x, int y, int z) : _x(x), _y(y), _z(z) {}
  operator QVector3D() const { return QVector3D(_x, _y, _z); }

  int _x,_y,_z;
};

#endif // GLOBAL_H
