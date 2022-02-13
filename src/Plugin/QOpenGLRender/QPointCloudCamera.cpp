#include "QOpenGLRender/QPointCloudCamera.h"


const float CAMERA_STEP = 0.01;

QPointCloudCamera::QPointCloudCamera(QObject *parent)
    : QObject{parent}
    , m_xRotation(0)
    , m_yRotation(0)
    , m_zRotation(0)
    , m_frontClippingPlaneDistance(0)
    , m_rearClippingDistance(0)
{

}


void QPointCloudCamera::forward()
{
    m_position[2] += CAMERA_STEP;
    _notify();
}


void QPointCloudCamera::backward()
{
    m_position[2] -= CAMERA_STEP;
    _notify();
}


void QPointCloudCamera::left()
{
    m_position[0] += CAMERA_STEP;
    _notify();
}


void QPointCloudCamera::right()
{
    m_position[0] -= CAMERA_STEP;
    _notify();

}


void QPointCloudCamera::up()
{
    m_position[1] -= CAMERA_STEP;
    _notify();
}


void QPointCloudCamera::down()
{
    m_position[1] += CAMERA_STEP;
    _notify();

}


void QPointCloudCamera::setFrontCPDistance(double distance) {
    m_frontClippingPlaneDistance = distance;
    _notify();
}


void QPointCloudCamera::setRearCPDistance(double distance) {
    m_rearClippingDistance = distance;
    _notify();
}


void QPointCloudCamera::setPosition(const QVector3D& position) {
    m_position = position;
}


void QPointCloudCamera::setXRotation(int angle)
{
    angle = angle % (360 * RK);
    if (angle != m_xRotation) {
        m_xRotation = angle;
        emit xRotationChanged(angle);
        _notify();
    }
}


void QPointCloudCamera::setYRotation(int angle)
{
    angle = angle % (360 * RK);
    if (angle != m_yRotation) {
        m_yRotation = angle;
        emit yRotationChanged(angle);
        _notify();
    }
}


void QPointCloudCamera::setZRotation(int angle)
{
    angle = angle % (360 * RK);
    if (angle != m_zRotation) {
        m_zRotation = angle;
        emit zRotationChanged(angle);
        _notify();
    }
}


void QPointCloudCamera::rotate(int dx, int dy, int dz) {
    setXRotation(m_xRotation + dx);
    setYRotation(m_yRotation + dy);
    setZRotation(m_zRotation + dz);
}


QCameraState QPointCloudCamera::state() const {
    return QCameraState(
                m_position,
                QVector3D((float)m_xRotation/RK, (float)m_yRotation/RK, (float)m_zRotation/RK),
                m_frontClippingPlaneDistance,
                m_rearClippingDistance
                );
}

