#ifndef QPOINTCLOUDCAMERA_H
#define QPOINTCLOUDCAMERA_H

#include <QObject>
#include <QVector3D>

struct QCameraState {
    QCameraState(const QVector3D& position_, const QVector3D& rotation_,
                 double frontClippingDistance_, double farClippingDistance_)
        : position(position_),
          rotation(rotation_),
          frontClippingDistance(frontClippingDistance_),
          rearClippingDistance(farClippingDistance_)
    {}

    const QVector3D position;
    const QVector3D rotation;
    const double frontClippingDistance;
    const double rearClippingDistance;
};

class QPointCloudCamera : public QObject
{
    Q_OBJECT
public:
    explicit QPointCloudCamera(QObject *parent = nullptr);

    enum RotationSTEP {RK = 1};

    void forward();
    void backward();
    void left();
    void right();
    void up();
    void down();
    void setPosition(const QVector3D& position);

    void rotate(int dx, int dy, int dz);

    void setFrontCPDistance(double distance);
    void setRearCPDistance(double distance);

    QCameraState state() const;


signals:
    void changed(const QCameraState& newState);
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);


public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);


private:
    double m_frontClippingPlaneDistance;
    double m_rearClippingDistance;
    QVector3D m_position;
    int m_xRotation;
    int m_yRotation;
    int m_zRotation;

    void _notify() {emit changed(state());}

signals:

};

#endif // QPOINTCLOUDCAMERA_H
