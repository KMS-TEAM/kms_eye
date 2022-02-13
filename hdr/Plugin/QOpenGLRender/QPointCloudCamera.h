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

    QVector3D position;
    QVector3D rotation;
    double frontClippingDistance;
    double rearClippingDistance;
};

class QPointCloudCamera : public QObject
{
    Q_OBJECT
    Q_PROPERTY(int xRotation READ xRotation WRITE setXRotation NOTIFY xRotationChanged)
    Q_PROPERTY(int yRotation READ yRotation WRITE setYRotation NOTIFY yRotationChanged)
    Q_PROPERTY(int zRotation READ zRotation WRITE setZRotation NOTIFY zRotationChanged)
    Q_PROPERTY(QCameraState state READ state WRITE setState NOTIFY stateChanged)
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

    int xRotation() const;
    int yRotation() const;
    int zRotation() const;

    void setFrontCPDistance(double distance);
    void setRearCPDistance(double distance);

    QCameraState state() const;


signals:
    void stateChanged(const QCameraState& newState);
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);


public slots:
    void setState(QCameraState state);
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

    void _notify() {emit stateChanged(state());}

signals:

};

#endif // QPOINTCLOUDCAMERA_H
