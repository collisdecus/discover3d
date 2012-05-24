#ifndef EDITORDIALOG_H
#define EDITORDIALOG_H

#include "level.h"
#include <QtCore/QSignalMapper>
#include <QtGui/QDialog>
#include <QtGui/QColor>

namespace Ui
{
  class EditorDialog;
}

//! Level editor
class EditorDialog : public QDialog
{
  Q_OBJECT
  
public:
  EditorDialog(QWidget* parent = 0);
  ~EditorDialog();
  
signals:
  void levelChanged(const Level& newLevel);

private slots:
  void blockToggled(int id);
  void onUpClicked();
  void onDownClicked();
  void onSaveClicked();
  void onColorClicked();

private:
  Block& blockForId(int id);
  void initBlocks();
  void updateView();

private:
  QVector<QVector<QVector<Block> > > _currentBlocks;
  Ui::EditorDialog* _ui;
  QSignalMapper _signalMapper;
  int _currentLevel;
  QColor _currentColor;
};

#endif // EDITORDIALOG_H
