#include "highscores.h"
#include "global.h"
#include "util.h"
#include <QtCore/QMap>
#include <QtCore/QSettings>
#include <QtCore/QStringList>
#include <QtGui/QWidget>
#include <QtGui/QInputDialog>

int Highscores::lowestScore(const QString &levelName)
{
  QSettings settings;
  QStringList scores = settings.value(SettingsKey::highscoresScoresPrefix + levelName).toStringList();
  QStringList names = settings.value(SettingsKey::highscoresNamesPrefix + levelName).toStringList();

  if (scores.size() < Global::highscoresEntries)
    return INT_MAX;
  else
    return scores.at(scores.size() - 1).toInt();
}

void Highscores::enter(QWidget* parent, const QString &levelName, int score)
{
  QString name = QInputDialog::getText(parent, parent->tr("Enter name"), parent->tr("You made the highscores list! Please enter your name:"), QLineEdit::Normal, parent->tr("Anonymous"));

  QSettings settings;

  QStringList scores = settings.value(SettingsKey::highscoresScoresPrefix + levelName).toStringList();
  QStringList names = settings.value(SettingsKey::highscoresNamesPrefix + levelName).toStringList();

  bool inserted = false;

  if (scores.isEmpty())
  {
    scores.append(QString::number(score));
    names.append(name);
    inserted = true;
  }
  else
  {
    for (int i = 0; i < scores.size(); ++i)
    {
      if (score < scores.at(i).toInt())
      {
        scores.insert(i, QString::number(score));
        names.insert(i, name);
        inserted = true;
        break;
      }
    }
  }

  if (!inserted)
  {
    scores.append(QString::number(score));
    names.append(name);
  }

  if (scores.size() > Global::highscoresEntries)
  {
    scores.removeLast();
    names.removeLast();
  }

  settings.setValue(SettingsKey::highscoresScoresPrefix + levelName, scores);
  settings.setValue(SettingsKey::highscoresNamesPrefix + levelName, names);
}

QString Highscores::getFormattedForLevel(const QString &levelName)
{
  QString result;
  QSettings settings;

  QStringList scores = settings.value(SettingsKey::highscoresScoresPrefix + levelName).toStringList();
  QStringList names = settings.value(SettingsKey::highscoresNamesPrefix + levelName).toStringList();

  for (int i = 0; i < scores.size(); ++i)
  {
    result += "<tr>";
    result += ("<td>" + QString::number(i + 1) + ".</td>");
    result += ("<td><b>" + names.at(i) + "</b></td>");
    result += ("<td>" + Util::secondsToTimeString(scores.at(i).toInt()) + "</td>");
    result += "</tr>";
  }

  if (scores.isEmpty())
    return QT_TR_NOOP("No scores yet");

  return "<table width = '100%'>" + result + "</table>";
}
