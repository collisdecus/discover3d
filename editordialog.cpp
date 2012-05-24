#include "editordialog.h"
#include "ui_editordialog.h"
#include <QtCore/QSignalMapper>
#include <QtGui/QCheckBox>
#include <QtGui/QGridLayout>
#include <QtGui/QFileDialog>
#include <QtGui/QColorDialog>

EditorDialog::EditorDialog(QWidget* parent) :
  QDialog(parent),
  _ui(new Ui::EditorDialog),
  _signalMapper(this),
  _currentLevel(0)
{
  _ui->setupUi(this);
  setWindowTitle(tr("Editor"));
  setAttribute(Qt::WA_DeleteOnClose);

  int id = 0;

  for (int k = 0; k < 10; ++k)
  {
    QGridLayout* gridLayout = new QGridLayout(_ui->editorStackedWidget->widget(k));
    _ui->editorStackedWidget->widget(k)->setLayout(gridLayout);

    for (int i = 0; i < 10; ++i)
    {
      for (int j = 0; j < 10; ++j)
      {
        QCheckBox* checkBox = new QCheckBox(_ui->editorStackedWidget->widget(k));
        connect(checkBox, SIGNAL(toggled(bool)), &_signalMapper, SLOT(map()));
        _signalMapper.setMapping(checkBox, id);
        gridLayout->addWidget(checkBox, i, j);
        ++id;
      }
    }
  }

  connect(&_signalMapper, SIGNAL(mapped(int)), this, SLOT(blockToggled(int)));
  connect(_ui->upButton, SIGNAL(clicked()), this, SLOT(onUpClicked()));
  connect(_ui->downButton, SIGNAL(clicked()), this, SLOT(onDownClicked()));
  connect(_ui->colorButton, SIGNAL(clicked()), this, SLOT(onColorClicked()));
  connect(_ui->saveButton, SIGNAL(clicked()), this, SLOT(onSaveClicked()));

  initBlocks();

  _ui->editorStackedWidget->setCurrentIndex(4);
  _ui->levelLabel->setText(QString::number(4));
  _currentLevel = 4;
}

EditorDialog::~EditorDialog()
{
  delete _ui;
}

void EditorDialog::blockToggled(int id)
{
  blockForId(id)._visible = !blockForId(id)._visible;
  blockForId(id)._exists = !blockForId(id)._exists;
  blockForId(id)._color = _currentColor;

  QPalette newPalette = _ui->colorLabel->palette();
  newPalette.setColor(QPalette::Window, blockForId(id)._color);
  _ui->colorLabel->setPalette(newPalette);

  updateView();
}

void EditorDialog::onUpClicked()
{
  if (_currentLevel > 0)
    _currentLevel--;

  _ui->editorStackedWidget->setCurrentIndex(_currentLevel);
  _ui->levelLabel->setText(QString::number(_currentLevel));
}

void EditorDialog::onDownClicked()
{
  if (_currentLevel < 9)
    _currentLevel++;

  _ui->editorStackedWidget->setCurrentIndex(_currentLevel);
  _ui->levelLabel->setText(QString::number(_currentLevel));
}

Block &EditorDialog::blockForId(int id)
{
  for (int i = 0; i < 10; ++i)
  {
    for (int j = 0; j < 10; ++j)
    {
      for (int k = 0; k < 10; ++k)
      {
        if (_currentBlocks.at(i).at(j).at(k)._id == id)
          return _currentBlocks[i][j][k];
      }
    }
  }

  return _currentBlocks[0][0][0];
}

void EditorDialog::initBlocks()
{
  int levelSize = 10;

  _currentBlocks.resize(levelSize);

  int id = 0;

  for (int i = 0; i < levelSize; ++i)
  {
    if (_currentBlocks.at(i).isEmpty())
      _currentBlocks[i].resize(levelSize);

    for (int j = 0; j < levelSize; ++j)
    {
      if (_currentBlocks.at(i).at(j).isEmpty())
        _currentBlocks[i][j].resize(levelSize);

      for (int k = 0; k < levelSize; ++k)
      {
        _currentBlocks[i][j][k] = Block();
        _currentBlocks[i][j][k]._id = id;
        _currentBlocks[i][j][k].setFace(Front, BlockFace(id, Front));
        _currentBlocks[i][j][k].setFace(Back, BlockFace(id, Back));
        _currentBlocks[i][j][k].setFace(Top, BlockFace(id, Top));
        _currentBlocks[i][j][k].setFace(Bottom, BlockFace(id, Bottom));
        _currentBlocks[i][j][k].setFace(Left, BlockFace(id, Left));
        _currentBlocks[i][j][k].setFace(Right, BlockFace(id, Right));
        ++id;
      }
    }
  }
}

void EditorDialog::updateView()
{
  Level newLevel;
  newLevel.setFinished(true);
  newLevel.setBlocks(_currentBlocks);
  emit levelChanged(newLevel);
}

void EditorDialog::onSaveClicked()
{
  QString filename = QFileDialog::getSaveFileName(this, "save");
  Level level;
  level.setBlocks(_currentBlocks);
  level.saveLevel(filename, _ui->nameEdit->text(), _ui->solutionEdit->text());
}

void EditorDialog::onColorClicked()
{
  QColorDialog dialog;
  dialog.exec();
  QColor selectedColor = dialog.selectedColor();
  QPalette newPalette = _ui->colorLabel->palette();
  newPalette.setColor(QPalette::Window, selectedColor);
  _ui->colorLabel->setPalette(newPalette);
  _currentColor = selectedColor;
}
