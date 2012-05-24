#ifndef ARCBALL_H
#define ARCBALL_H

#include <QtCore/QPointF>
#include <QtGui/QVector3D>
#include <QtGui/QQuaternion>

//! Arc ball rotation of the level
class ArcBall
{
public:
  ArcBall(const QVector3D& axis);
  void push(const QPointF& point, const QQuaternion &transformation);
  void move(const QPointF& point, const QQuaternion &transformation);
  void release(const QPointF& point, const QQuaternion &transformation);
  QQuaternion getRotation() const;

private:
  QQuaternion _rotation;
  QVector3D _axis;
  QPointF _lastPosition;
  bool _pressed;
};

#endif // ARCBALL_H
