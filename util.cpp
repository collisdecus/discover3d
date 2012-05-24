#include "util.h"
#include "global.h"
#include <QtCore/QSettings>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QTextStream>
#include <QtGui/QListWidget>
#include <GL/gl.h>

void Util::fillListWidgetWithLevels(QListWidget *widget)
{
  QSettings settings;
  QStringList levelsDone = settings.value(SettingsKey::levelsDone).toStringList();

  QDir levelsDir(Global::levelsDirectory);

  foreach (QFileInfo levelFile, levelsDir.entryInfoList(QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot | QDir::Readable))
  {
    QFile file(levelFile.absoluteFilePath());
    file.open(QIODevice::ReadOnly);
    QTextStream in(&file);
    QString name = in.readLine();
    file.close();

    QListWidgetItem* item = new QListWidgetItem();
    item->setText(name);
    item->setData(Qt::UserRole, levelFile.absoluteFilePath());

    if (levelsDone.contains(name))
      item->setIcon(QIcon(":/assets/done.png"));
    else
      item->setIcon(QIcon(":/assets/undone.png"));

    widget->addItem(item);
  }
}

void Util::multMatrix(const QMatrix4x4& m)
{
  static GLfloat mat[16];
  const qreal *data = m.constData();
  for (int index = 0; index < 16; ++index)
    mat[index] = data[index];
  glMultMatrixf(mat);
}

QString Util::secondsToTimeString(int seconds)
{
  int minutes = seconds / 60;
  seconds = seconds - (minutes*60);

  QString minutesString = QString::number(minutes);
  if (minutesString.size() == 1)
    minutesString.insert(0, "  ");
  else if (minutesString.size() == 2)
    minutesString.insert(0, " ");

  QString secondsString = QString::number(seconds);
  if (secondsString.size() == 1)
    secondsString.insert(0, "0");

  return minutesString + ":" + secondsString;
}
