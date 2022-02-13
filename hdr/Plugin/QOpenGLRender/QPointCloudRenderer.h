#ifndef QPOINTCLOUDSCENE_H
#define QPOINTCLOUDSCENE_H

#include <QObject>
#include <QWindow>
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector3D>
#include <QSharedPointer>
#include <QVector>

#include "QPointCloudCamera.h"

class QPointCloudRenderer : public QWindow, protected QOpenGLFunctions
{
    Q_OBJECT
public:

    enum ColorAxisMode {COLOR_BY_ROW, COLOR_BY_Z};

    QPointCloudRenderer(const QString& plyFilePath, QWindow* parent = nullptr);
    ~QPointCloudRenderer();

public slots:
    void setPointSize(size_t size);
    void setColorAxisMode(ColorAxisMode value);
    void attachCamera(QSharedPointer<QPointCloudCamera> camera);
    void setPickpointEnabled(bool enabled);
    void clearPickedpoints();

signals:
    void pickpointsChanged(const QVector<QVector3D> points);

protected:
//    void initializeGL() Q_DECL_OVERRIDE;
//    void paintGL() Q_DECL_OVERRIDE;
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;

    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;


private slots:
    void _onCameraChanged(const QCameraState& state);

private:
    void _loadPLY(const QString& plyFilePath);
    void _cleanup();
    void _drawFrameAxis();
    QVector3D _unproject(int x, int y) const;
    QVector3D _pickPointFrom2D(const QPoint& pos) const;
    void _drawMarkerBox(const QVector3D& point, const QColor& color);

    float m_pointSize;
    ColorAxisMode m_colorMode;
    std::vector<std::pair<QVector3D, QColor> > m_axesLines;

    QPoint m_prevMousePosition;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_vertexBuffer;
    QScopedPointer<QOpenGLShaderProgram> m_shaders;

    QMatrix4x4 m_projectionMatrix;
    QMatrix4x4 m_cameraMatrix;
    QMatrix4x4 m_worldMatrix;

    QVector<float> m_pointsData;
    size_t m_pointsCount;
    QVector3D m_pointsBoundMin;
    QVector3D m_pointsBoundMax;
    QVector3D m_ray;

    QSharedPointer<QPointCloudCamera> m_currentCamera;

    bool m_pickpointEnabled;
    QVector<QVector3D> m_pickedPoints;
    QVector3D m_highlitedPoint;


};

#endif // QPOINTCLOUDSCENE_H
