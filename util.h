#ifndef UTIL_HPP
#define UTIL_HPP

#include <QtCore/QString>
#include <QtGui/QMatrix4x4>

class QListWidget;

class Util
{
public:
  static void fillListWidgetWithLevels(QListWidget* widget);
  static void multMatrix(const QMatrix4x4& m);
  static QString secondsToTimeString(int seconds);
};

#endif // UTIL_HPP
