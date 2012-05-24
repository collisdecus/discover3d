#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QtGui/QDialog>

namespace Ui
{
  class AboutDialog;
}

//! Displays information about the game
class AboutDialog : public QDialog
{
  Q_OBJECT
    
public:
  AboutDialog(QWidget* parent = 0);
  ~AboutDialog();
    
private:
  Ui::AboutDialog* _ui;
};

#endif // ABOUTDIALOG_H
