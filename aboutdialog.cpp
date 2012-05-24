#include "aboutdialog.h"
#include "ui_aboutdialog.h"

AboutDialog::AboutDialog(QWidget* parent)
  : QDialog(parent)
  , _ui(new Ui::AboutDialog)
{
  _ui->setupUi(this);
  connect(_ui->pushButton, SIGNAL(clicked()), this, SLOT(accept()));
  setWindowTitle(tr("About"));
}

AboutDialog::~AboutDialog()
{
  delete _ui;
}
