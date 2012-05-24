#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui/QDialog>

namespace Ui
{
  class SettingsDialog;
}

//! Game settings
class SettingsDialog : public QDialog
{
  Q_OBJECT
  
public:
  SettingsDialog(QWidget* parent = 0);
  ~SettingsDialog();
  
private slots:
  void onSettingsChanged();

private:
  Ui::SettingsDialog* _ui;
};

#endif // SETTINGSDIALOG_H
