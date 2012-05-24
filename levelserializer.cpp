#include "levelserializer.h"
#include "global.h"
#include <QtCore/QFile>

void LevelSerializer::save(const Level &level, const QString &filename)
{
  QFile file(filename);
  file.open(QIODevice::WriteOnly);
  QTextStream out(&file);

  out << level.getName() << "\n";
  out << level.getSolution() << "\n";

  QVector<QVector<QVector<int> > > numbers;
  numbers.resize(3);

  for (int i = 0; i < Global::levelSize; ++i)
  {
    numbers[0].append(QVector<int>(Global::levelSize));
    numbers[1].append(QVector<int>(Global::levelSize));
    numbers[2].append(QVector<int>(Global::levelSize));
  }

  for (int i = 0; i < Global::levelSize; ++i)
  {
    for (int j = 0; j < Global::levelSize; ++j)
    {
      for (int k = 0; k < Global::levelSize; ++k)
      {
        if (!level.block(i,j,k)._exists)
          continue;

        numbers[0][i][j]++;
        numbers[1][i][k]++;
        numbers[2][j][k]++;
        out << i << "," << j << "," << k;
        out << ":";
        out << level.block(i,j,k)._color.red() << ",";
        out << level.block(i,j,k)._color.green() << ",";
        out << level.block(i,j,k)._color.blue() << "\n";
      }
    }
  }

  out << "-\n";

  for (int k = 0; k < 3; ++k)
  {
    for (int i = 0; i < Global::levelSize; ++i)
    {
      for (int j = 0; j < Global::levelSize; ++j)
        out << ((j==0) ? "" : ",") << numbers.at(k).at(i).at(j);

      out << "\n";
    }

    out << Global::levelPartBreak << "\n";
  }

  file.close();
}

Level LevelSerializer::load(const QString &filename)
{
  Level result;
  result.initBlocks();

  QFile file(filename);
  file.open(QIODevice::ReadOnly);
  QTextStream in(&file);

  result._name = in.readLine();
  result._solution = in.readLine();

  QString currentBlock = in.readLine();
  while (currentBlock != Global::levelPartBreak)
  {
    QStringList parts = currentBlock.split(":");
    QStringList coord = parts.at(0).split(",");
    QStringList col = parts.at(1).split(",");
    QColor color = QColor(col.at(0).toInt(), col.at(1).toInt(), col.at(2).toInt());
    int x = coord.at(0).toInt();
    int y = coord.at(1).toInt();
    int z = coord.at(2).toInt();
    result.block(x, y, z)._exists = true;
    result.block(x, y, z)._color = color;
    result.block(x, y, z).setFace(Front, BlockFace(result.block(x, y, z)._id, Front));
    result.block(x, y, z).setFace(Back, BlockFace(result.block(x, y, z)._id, Back));
    result.block(x, y, z).setFace(Top, BlockFace(result.block(x, y, z)._id, Top));
    result.block(x, y, z).setFace(Bottom, BlockFace(result.block(x, y, z)._id, Bottom));
    result.block(x, y, z).setFace(Left, BlockFace(result.block(x, y, z)._id, Left));
    result.block(x, y, z).setFace(Right, BlockFace(result.block(x, y, z)._id, Right));
    currentBlock = in.readLine();
  }

  Coordinate3D len, min;
  result.extents(len, min);

  for (int i = 0; i < Global::levelSize; ++i)
  {
    for (int j = 0; j < Global::levelSize; ++j)
    {
      for (int k = 0; k < Global::levelSize; ++k)
      {
        if ( (i >= min._x && j >= min._y && k >= min._z) &&
             (i <= min._x + len._x && j <= min._y + len._y && k <= min._z + len._z) )
        {
          result.block(i,j,k)._visible = true;
        }
      }
    }
  }

  QVector<QVector<QVector<int> > > numbers;
  numbers.resize(3);

  for (int i = 0; i < 3; ++i)
  {
    currentBlock = in.readLine();
    while (currentBlock != Global::levelPartBreak)
    {
      numbers[i].append(QVector<int>());
      QStringList line = currentBlock.split(",");
      for (int j = 0; j < 10; ++j)
        numbers[i][numbers[i].size() - 1].append(line.at(j).toInt());

      currentBlock = in.readLine();
    }
  }

  for (int i = 0; i < result._blocks.size(); ++i)
  {
    for (int j = 0; j < result._blocks.at(i).size(); ++j)
    {
      for (int k = 0; k < Global::levelSize; ++k)
      {
        int divisions = 0;
        int last = 0;
        for (int k2 = 0; k2 < result._blocks.at(i).at(j).size(); ++k2)
        {
          if (last == 0 && result.block(i,j,k2)._exists)
            divisions++;

          last = result.block(i,j,k2)._exists ? 1 : 0;
        }

        result.block(i,j,k).setFace(Front, BlockFace(result.block(i,j,k)._id, Front, numbers[0][i][j], divisionForCount(divisions)));
        result.block(i,j,k).setFace(Back, BlockFace(result.block(i,j,k)._id, Back, numbers[0][i][j], divisionForCount(divisions)));

        divisions = 0;
        last = 0;
        for (int j2 = 0; j2 < Global::levelSize; ++j2)
        {
          if (last == 0 && result.block(i,j2,k)._exists)
            divisions++;

          last = result.block(i,j2,k)._exists ? 1 : 0;
        }

        result.block(i,j,k).setFace(Top, BlockFace(result.block(i,j,k)._id, Top, numbers[1][i][k], divisionForCount(divisions)));
        result.block(i,j,k).setFace(Bottom, BlockFace(result.block(i,j,k)._id, Bottom, numbers[1][i][k], divisionForCount(divisions)));

        divisions = 0;
        last = 0;
        for (int i2 = 0; i2 < Global::levelSize; ++i2)
        {
          if (last == 0 && result.block(i2,j,k)._exists)
            divisions++;

          last = result.block(i2,j,k)._exists ? 1 : 0;
        }

        result.block(i,j,k).setFace(Left, BlockFace(result.block(i,j,k)._id, Left, numbers[2][j][k], divisionForCount(divisions)));
        result.block(i,j,k).setFace(Right, BlockFace(result.block(i,j,k)._id, Right, numbers[2][j][k], divisionForCount(divisions)));
      }
    }
  }

  file.close();
  return result;
}

Division LevelSerializer::divisionForCount(int count)
{
  if (count <= 1)
    return NoDivision;
  else if (count == 2)
    return TwoParts;
  else
    return ThreePlusParts;
}

