#ifndef LEVELSERIALIZER_H
#define LEVELSERIALIZER_H

#include "level.h"
#include <QtCore/QTextStream>

class LevelSerializer
{
public:
  static void save(const Level& level, const QString& filename);
  static Level load(const QString& filename);

private:
  static Division divisionForCount(int count);
};

#endif // LEVELSERIALIZER_H
