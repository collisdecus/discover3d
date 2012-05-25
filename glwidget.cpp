#include "glwidget.h"
#include "highscores.h"
#include "global.h"
#include "util.h"
#include "levelrenderer.h"
#include <QtCore/QSettings>
#include <QtGui/QMatrix4x4>
#include <QtGui/QMatrix3x3>
#include <QtGui/QMouseEvent>
#include <GL/glu.h>

GLWidget::GLWidget(QWidget* parent) :
  QGLWidget(parent),
  _arcBall(QVector3D(0, 1, 0)),
  _buttonDown(false),
  _lastMouseDelta(0),
  _textureManager(this),
  _started(false),
  _finished(false),
  _lastTime(0),
  _currentPenalty(15),
  _penaltySum(0),
  _lastPenalty(0)
{
  setMouseTracking(true);
  _drawTimer.setInterval(0);
  _levelRenderer = new LevelRenderer(&_textureManager);
}

GLWidget::~GLWidget()
{
  delete _levelRenderer;
}

void GLWidget::initializeGL()
{
  _textureManager.init();

  glClearColor(1.0, 1.0, 1.0, 0.0);
  glEnable(GL_TEXTURE_2D);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_FLAT);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  setupLight();

  glEnable(GL_COLOR_MATERIAL);
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
  glEnable(GL_NORMALIZE);
  _drawTimer.start();
  connect(&_drawTimer, SIGNAL(timeout()), this, SLOT(updateGL()));
  _penaltyTimeline.setCurveShape(QTimeLine::SineCurve);
}

void GLWidget::resizeGL(int width, int height)
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0f,(GLfloat)width/(GLfloat)height,0.1f,100.0f);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glViewport(0, 0, (GLint)width, (GLint)height);

  _ratio = (GLfloat)width / (GLfloat)height;
}

void GLWidget::paintGL()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  glTranslatef(0.0f,0.0f,-(qMax(6, _currentLevel.maximumExtent()) * 6.0f));

  QMatrix4x4 m;
  m.rotate(_arcBall.getRotation());
  Util::multMatrix(m);

  _levelRenderer->render(_currentLevel, DefaultMode);
  renderLevelTime();

  glFlush();
}

void GLWidget::renderLevelTime()
{
  if (!_started && !_finished)
    return;

  if (!_finished)
    _lastTime = _levelTimer.elapsed()/1000;

  glColor3f(0.3f, 0.3f, 0.3f);
  QFont font;
  font.setPixelSize(30);
  QString levelTime = Util::secondsToTimeString(_lastTime + _penaltySum);
  renderText(width() - 100, 40, levelTime, font);
  renderText(20, 40, _currentLevel.getName(), font);

  if (_finished)
  {
    QFont largeFont;
    largeFont.setPixelSize(50);
    QFontMetrics metrics(largeFont);
    int textWidth = metrics.width(_currentLevel.getSolution());
    renderText((width()/2) - (textWidth/2), height() - 30, _currentLevel.getSolution(), largeFont);
  }

  float penaltyColor = _penaltyTimeline.currentValue();
  glColor3f(1.0f, 1.0f - penaltyColor, 1.0f - penaltyColor);
  QString penaltyString = Util::secondsToTimeString(_lastPenalty);
  penaltyString.replace(0, 2, "+");
  renderText((width() / 2) - 50, 40, penaltyString, font);

  glColor3f(1.0f, 1.0f, 1.0f);
}

void GLWidget::setLevel(const Level& level, bool start /*= false*/)
{
  _currentLevel = level;
  if (start)
  {
    _levelTimer.start();
    _started = true;
    _finished = false;
    _penaltySum = 0;
    _currentPenalty = 15;
  }
}

Level GLWidget::getLevel() const
{
  return _currentLevel;
}

QPointF GLWidget::pixelPosToViewPos(const QPointF& point)
{
  return QPointF(2.0 * float(point.x()) / width() - 1.0, 1.0 - 2.0 * float(point.y()) / height());
}

void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
  _currentLevel.highlightBlock(pick(event->pos()));

  if(event->buttons() & Qt::LeftButton)
  {
    _arcBall.move(pixelPosToViewPos(event->posF()), QQuaternion());
    event->accept();
  }
  else
    _arcBall.release(pixelPosToViewPos(event->posF()), QQuaternion());

  if (_buttonDown)
  {
    if (!_lastMousePos.isNull())
      _lastMouseDelta += (event->pos() - _lastMousePos).manhattanLength();
    _lastMousePos = event->pos();
  }
}

void GLWidget::mousePressEvent(QMouseEvent* event)
{
  if(event->buttons() & Qt::LeftButton)
  {
     _arcBall.push(pixelPosToViewPos(event->posF()), QQuaternion());
     event->accept();
  }

  _buttonDown = true;
}

void GLWidget::mouseReleaseEvent(QMouseEvent* event)
{
  _buttonDown = false;

  if (_lastMouseDelta < 25)
    _currentLevel.blockClicked(pick(event->pos()), event->button());

  _lastMouseDelta = 0;
  _lastMousePos = QPoint();

  if(event->buttons() & Qt::LeftButton)
  {
    _arcBall.release(pixelPosToViewPos(event->posF()),QQuaternion());
    event->accept();
  }
}

void GLWidget::setupLight() const
{
  glEnable(GL_LIGHTING);

  GLfloat lightAmbient[]= { 0.7f, 0.7f, 0.7f, 1.0f };
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lightAmbient);

  GLfloat lightDiffuse[]= { 0.4f, 0.4f, 0.4f, 1.0f };
  GLfloat lightPosition[]= { 180.0f, 220.0f, 200.0f, 1.0f };
  GLfloat lightDirection[]= { 0.0f, 0.0f, 0.0f };

  glLightfv(GL_LIGHT1, GL_DIFFUSE, lightDiffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, lightPosition);
  glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, lightDirection);
  glEnable(GL_LIGHT1);
}

int GLWidget::pick(const QPoint &mousePos)
{
  GLint viewport[4];
  GLuint selectBuffer[512];

  glSelectBuffer(512, selectBuffer);
  glRenderMode(GL_SELECT);

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();

  glGetIntegerv(GL_VIEWPORT,viewport);
  gluPickMatrix(mousePos.x(), viewport[3] - mousePos.y(), 5, 5, viewport);
  gluPerspective(45, _ratio, 0.1, 100.0f);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  glTranslatef(0.0f, 0.0f, -(qMax(6, _currentLevel.maximumExtent()) * 6.0f));

  QMatrix4x4 matrix;
  matrix.rotate(_arcBall.getRotation());
  Util::multMatrix(matrix);

  _levelRenderer->render(_currentLevel, SelectMode);

  int hits;
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
  glFlush();

  hits = glRenderMode(GL_RENDER);

  if (hits == 0)
    return -1;

  unsigned int i, j;
  GLuint names, *ptr, minZ, *ptrNames, numberOfNames;
  numberOfNames = 0;
  ptrNames = 0;

  ptr = (GLuint *) selectBuffer;
  minZ = 0xffffffff;
  for (i = 0; i < (unsigned int)hits; i++)
  {
    names = *ptr;
    ptr++;

    if (*ptr < minZ)
    {
      numberOfNames = names;
      minZ = *ptr;
      ptrNames = ptr + 2;
    }
    ptr += names + 2;
  }
  ptr = ptrNames;

  for (j = 0; j < numberOfNames; j++, ptr++)
    return *ptr;

  return -1;
}

void GLWidget::addPenalty()
{
  _penaltySum += _currentPenalty;
  _lastPenalty = _currentPenalty;
  if (_currentPenalty != Global::maximumPenalty)
    _currentPenalty *= 2;
  _penaltyTimeline.stop();
  _penaltyTimeline.start();
}

void GLWidget::finish()
{
  _finished = true;
  _started = false;

  QSettings settings;
  QStringList levelsDone = settings.value("levelsDone").toStringList();
  if (!levelsDone.contains(_currentLevel.getName()))
    levelsDone.append(_currentLevel.getName());
  settings.setValue("levelsDone", levelsDone);

  Highscores highscores;
  if ((_lastTime + _penaltySum) < highscores.lowestScore(_currentLevel.getName()))
    highscores.enter(this, _currentLevel.getName(), _lastTime + _penaltySum);
}

