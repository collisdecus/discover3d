#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include "glwidget.h"

//! The main game window
class MainWindow : public QMainWindow
{
  Q_OBJECT
    
public:
  MainWindow(QWidget* parent = 0);
  ~MainWindow();

protected:
  void closeEvent(QCloseEvent* event);
  void keyPressEvent(QKeyEvent* event);

private slots:
  void onNewGameClicked();
  void onHighscoresClicked();
  void onSettingsClicked();
  void onAboutClicked();

  void onEditorSequence();
  void onEditorLevelChanged(const Level &level);

private:
  void setupMenus();
  GLWidget* _glWidget;
};

#endif // MAINWINDOW_H
