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

class QPointCloudRenderer : public QObject, protected QOpenGLFunctions
{
    Q_OBJECT
public:

    enum ColorAxisMode {COLOR_BY_ROW, COLOR_BY_Z};

    QPointCloudRenderer(const QString& plyFilePath, QObject* parent = nullptr);
    ~QPointCloudRenderer();

    void initialize();
    void render();
    void invalidate();

public slots:
    void setPointSize(size_t size);
    void setColorAxisMode(ColorAxisMode value);

private:
    void _loadPLY(const QString& plyFilePath);
    void _drawFrameAxis();

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

};

#endif // QPOINTCLOUDSCENE_H
