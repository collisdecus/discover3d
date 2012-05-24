#ifndef GLWIDGET_H
#define GLWIDGET_H

#include "level.h"
#include "arcball.h"
#include "texturemanager.h"
#include <QtCore/QTimer>
#include <QtCore/QTime>
#include <QtCore/QTimeLine>
#include <QtOpenGL/QGLWidget>

class LevelRenderer;

//! Handles rendering & some input
class GLWidget : public QGLWidget
{
  Q_OBJECT

public:
  GLWidget(QWidget* parent = 0);
  ~GLWidget();

  void setLevel(const Level& level, bool start = false);
  Level getLevel() const;
  void addPenalty();
  void finish();

protected:
  void initializeGL();
  void resizeGL(int width, int height);
  void paintGL();
  void mouseMoveEvent(QMouseEvent* event);
  void mousePressEvent(QMouseEvent* event);
  void mouseReleaseEvent(QMouseEvent* event);

private:
  void setupLight() const;
  void renderLevelTime();
  int pick(const QPoint& mousePos);
  QPointF pixelPosToViewPos(const QPointF& point);

  Level _currentLevel;
  ArcBall _arcBall;
  float _ratio;
  QTimer _drawTimer;
  bool _buttonDown;
  int _lastMouseDelta;
  QPoint _lastMousePos;
  TextureManager _textureManager;
  QTime _levelTimer;
  bool _started;
  bool _finished;
  int _lastTime;
  int _currentPenalty;
  int _penaltySum;
  int _lastPenalty;
  QTimeLine _penaltyTimeline;
  LevelRenderer* _levelRenderer;
};

#endif // GLWIDGET_H
