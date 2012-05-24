#ifndef HIGHSCORESDIALOG_H
#define HIGHSCORESDIALOG_H

#include <QtGui/QDialog>

namespace Ui
{
  class HighscoresDialog;
}

//! Displays highscores
class HighscoresDialog : public QDialog
{
  Q_OBJECT
  
public:
  HighscoresDialog(QWidget* parent = 0);
  ~HighscoresDialog();

private slots:
  void onLevelSelectionChanged(int index);
  
private:
  Ui::HighscoresDialog* _ui;
};

#endif // HIGHSCORESDIALOG_H
