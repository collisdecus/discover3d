#include "highscoresdialog.h"
#include "ui_highscoresdialog.h"
#include "highscores.h"
#include "global.h"
#include "util.h"
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>
#include <QtCore/QSettings>
#include <QtCore/QTextStream>

HighscoresDialog::HighscoresDialog(QWidget* parent) :
  QDialog(parent),
  _ui(new Ui::HighscoresDialog)
{
  _ui->setupUi(this);
  setWindowTitle(tr("Highscores"));
  Util::fillListWidgetWithLevels(_ui->listWidget);
  connect(_ui->pushButton, SIGNAL(clicked()), this, SLOT(accept()));
  connect(_ui->listWidget, SIGNAL(currentRowChanged(int)), this, SLOT(onLevelSelectionChanged(int)));
}

HighscoresDialog::~HighscoresDialog()
{
  delete _ui;
}

void HighscoresDialog::onLevelSelectionChanged(int index)
{
  QString levelName = _ui->listWidget->item(index)->text();
  Highscores highscores;
  _ui->textBrowser->setHtml(highscores.getFormattedForLevel(levelName));
}
