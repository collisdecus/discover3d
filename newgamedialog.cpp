#include "newgamedialog.h"
#include "ui_newgamedialog.h"
#include "global.h"
#include "util.h"
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QSettings>
#include <QtCore/QTextStream>

NewGameDialog::NewGameDialog(QWidget* parent) :
  QDialog(parent),
  _ui(new Ui::NewGameDialog)
{
  _ui->setupUi(this);
  setWindowTitle(tr("New game"));
  Util::fillListWidgetWithLevels(_ui->levelListWidget);
  connect(_ui->levelListWidget, SIGNAL(itemSelectionChanged()), this, SLOT(onLevelSelected()));
  connect(_ui->startGameButton, SIGNAL(clicked()), this, SLOT(accept()));
}

NewGameDialog::~NewGameDialog()
{
  delete _ui;
}

QString NewGameDialog::getSelectedLevel() const
{
  return _selectedLevel;
}

void NewGameDialog::onLevelSelected()
{
  _ui->startGameButton->setEnabled(true);
  int selected = _ui->levelListWidget->currentRow();
  _selectedLevel = _ui->levelListWidget->item(selected)->data(Qt::UserRole).toString();
}
