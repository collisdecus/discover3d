#ifndef NEWGAMEDIALOG_H
#define NEWGAMEDIALOG_H

#include <QtGui/QDialog>

namespace Ui
{
  class NewGameDialog;
}

//! Starts a new game
class NewGameDialog : public QDialog
{
  Q_OBJECT
  
public:
  NewGameDialog(QWidget* parent = 0);
  ~NewGameDialog();
  
  QString getSelectedLevel() const;

private slots:
  void onLevelSelected();

private:
  Ui::NewGameDialog* _ui;
  QString _selectedLevel;
};

#endif // NEWGAMEDIALOG_H
