#include "mainwindow.h"
#include "aboutdialog.h"
#include "settingsdialog.h"
#include "newgamedialog.h"
#include "highscoresdialog.h"
#include "editordialog.h"
#include "global.h"
#include <QtCore/QSettings>
#include <QtCore/QCoreApplication>
#include <QtCore/QFile>
#include <QtGui/QMessageBox>
#include <QtGui/QKeyEvent>
#include <QtGui/QMenuBar>

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
{
  setWindowTitle(tr("Discover 3D"));
  setWindowIcon(QIcon(":/assets/logo_32.png"));
  setMenuBar(new QMenuBar(this));
  setMinimumSize(400, 300);

  _glWidget = new GLWidget(this);
  setCentralWidget(_glWidget);

  setupMenus();

  QCoreApplication::setOrganizationName("Collisdecus");
  QCoreApplication::setApplicationName(tr("Discover 3D"));

  QSettings settings;
  restoreGeometry(settings.value(SettingsKey::windowGeometry).toByteArray());

  if (!settings.contains(SettingsKey::markButton))
    settings.setValue(SettingsKey::markButton, (int)Qt::LeftButton);
}

MainWindow::~MainWindow()
{
}

void MainWindow::setupMenus()
{
  QMenu* gameMenu = menuBar()->addMenu(tr("Game"));
  QMenu* extrasMenu = menuBar()->addMenu(tr("Extras"));
  QMenu* helpMenu = menuBar()->addMenu(tr("Help"));

  gameMenu->addAction(tr("New game..."), this, SLOT(onNewGameClicked()));
  gameMenu->addSeparator();
  gameMenu->addAction(tr("Highscores"), this, SLOT(onHighscoresClicked()));
  gameMenu->addSeparator();
  gameMenu->addAction(tr("Exit"), this, SLOT(close()));
  extrasMenu->addAction(tr("Settings"), this, SLOT(onSettingsClicked()));
  helpMenu->addAction(tr("About Discover 3D"), this, SLOT(onAboutClicked()));
}

void MainWindow::closeEvent(QCloseEvent* event)
{
  QSettings settings;
  settings.setValue(SettingsKey::windowGeometry, saveGeometry());
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
  if (event->key() == Qt::Key_E && event->modifiers() == Qt::ControlModifier)
    onEditorSequence();
}

void MainWindow::onNewGameClicked()
{
  NewGameDialog dialog(this);
  if (dialog.exec() == QDialog::Accepted)
  {
    QString filename = dialog.getSelectedLevel();
    if (QFile::exists(filename))
      _glWidget->setLevel(Level(_glWidget, filename), true);
  }
}

void MainWindow::onHighscoresClicked()
{
  HighscoresDialog dialog(this);
  dialog.exec();
}

void MainWindow::onSettingsClicked()
{
  SettingsDialog dialog(this);
  dialog.exec();
}

void MainWindow::onAboutClicked()
{
  AboutDialog dialog(this);
  dialog.exec();
}

void MainWindow::onEditorSequence()
{
  EditorDialog* dialog = new EditorDialog(this);
  dialog->show();

  connect(dialog, SIGNAL(levelChanged(const Level&)), this, SLOT(onEditorLevelChanged(const Level&)));
}

void MainWindow::onEditorLevelChanged(const Level& level)
{
  _glWidget->setLevel(level);
  _glWidget->updateGL();
}
