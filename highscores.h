#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#include <QtCore/QString>

class QWidget;

class Highscores
{
public:
  static int lowestScore(const QString& levelName);
  static void enter(QWidget* parent, const QString& levelName, int score);
  static QString getFormattedForLevel(const QString& levelName);
  static QString secondsFormatted(int seconds);
};

#endif // HIGHSCORES_H
