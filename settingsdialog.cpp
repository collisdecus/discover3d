#include "settingsdialog.h"
#include "ui_settingsdialog.h"
#include "global.h"
#include <QtCore/QSettings>

SettingsDialog::SettingsDialog(QWidget* parent) :
  QDialog(parent),
  _ui(new Ui::SettingsDialog)
{
  _ui->setupUi(this);
  setWindowTitle(tr("Settings"));
  connect(_ui->pushButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(_ui->markLeftButton, SIGNAL(clicked()), this, SLOT(onSettingsChanged()));
  connect(_ui->markRightButton, SIGNAL(clicked()), this, SLOT(onSettingsChanged()));

  QSettings settings;
  if (settings.value(SettingsKey::markButton).toInt() == (int)Qt::LeftButton)
    _ui->markLeftButton->setChecked(true);
  else
    _ui->markRightButton->setChecked(true);
}

SettingsDialog::~SettingsDialog()
{
  delete _ui;
}

void SettingsDialog::onSettingsChanged()
{
  QSettings settings;
  if (_ui->markLeftButton->isChecked())
    settings.setValue(SettingsKey::markButton, (int)Qt::LeftButton);
  else
    settings.setValue(SettingsKey::markButton, (int)Qt::RightButton);
}
