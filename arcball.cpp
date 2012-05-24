#include "arcball.h"
#include <cmath>

ArcBall::ArcBall(const QVector3D &axis)
  : _axis(axis)
  , _pressed(false)
{
}

void ArcBall::push(const QPointF& point, const QQuaternion&)
{
  _rotation = getRotation();
  _pressed = true;
  _lastPosition = point;
}

void ArcBall::move(const QPointF& point, const QQuaternion& transformation)
{
  if (!_pressed)
      return;

  QVector3D lastPosition3D = QVector3D(_lastPosition.x(), _lastPosition.y(), 0.0f);
  float sqrZ = 1 - QVector3D::dotProduct(lastPosition3D, lastPosition3D);
  if (sqrZ > 0)
      lastPosition3D.setZ(sqrt(sqrZ));
  else
      lastPosition3D.normalize();

  QVector3D currentPosition3D = QVector3D(point.x(), point.y(), 0.0f);
  sqrZ = 1 - QVector3D::dotProduct(currentPosition3D, currentPosition3D);
  if (sqrZ > 0)
      currentPosition3D.setZ(sqrt(sqrZ));
  else
      currentPosition3D.normalize();

  _axis = QVector3D::crossProduct(lastPosition3D, currentPosition3D);
  float angle = 180 / 3.141 * asin(sqrt(QVector3D::dotProduct(_axis, _axis)));

  _axis.normalize();
  _axis = transformation.rotatedVector(_axis);
  _rotation = QQuaternion::fromAxisAndAngle(_axis, angle) * _rotation;

  _lastPosition = point;
}

void ArcBall::release(const QPointF& point, const QQuaternion &transformation)
{
  move(point, transformation);
  _pressed = false;
}

QQuaternion ArcBall::getRotation() const
{
  return _rotation;
}
