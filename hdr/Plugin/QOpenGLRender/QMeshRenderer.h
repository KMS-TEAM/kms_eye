#ifndef QMESHRENDER_H
#define QMESHRENDER_H

#include <QObject>
#include <QScopedPointer>

class QOpenGLBuffer;
class QOpenGLShaderProgram;
class QOpenGLVertexArrayObject;

class QMeshRenderer : public QObject
{
    Q_OBJECT
public:
    explicit QMeshRenderer(QObject *parent = nullptr);
    ~QMeshRenderer();

    enum CoordinateMirroring{
        DoNotMirrorCoordinates,
        MirrorYCoordinate
    };

    // All assume that the GL context is current.
    void initialize(CoordinateMirroring cm = DoNotMirrorCoordinates);
    void render();
    void invalidate();

    void setAzimuth(float azimuth);
    void setElevation(float elevation);
    void setDistance(float distance);

signals:

private:
    QScopedPointer<QOpenGLBuffer> m_positionsBuffer;
    QScopedPointer<QOpenGLBuffer> m_normalsBuffer;
    QScopedPointer<QOpenGLBuffer> m_indicesBuffer;
    QScopedPointer<QOpenGLShaderProgram> m_shaderProgram;
    QScopedPointer<QOpenGLVertexArrayObject> m_vao;

    int m_indicesCount;

    CoordinateMirroring m_coordinateMirroring;

    float m_azimuth;
    float m_elevation;
    float m_distance;
};

#endif // QMESHRENDER_H
