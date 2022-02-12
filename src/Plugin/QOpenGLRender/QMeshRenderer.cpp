#include "QOpenGLRender/QMeshRenderer.h"
#include "QOpenGLRender/QObjLoader.h"
#include "AppConstant.h"

#include <QOpenGLContext>
#include <QOpenGLFunctions>
#include <QOpenGLBuffer>
#include <QOpenGLTexture>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>

#include <QMatrix4x4>
#include <QDebug>

#include <cmath>
#include <QtMath>

QMeshRenderer::QMeshRenderer(QObject *parent)
    : QObject{parent}
    , m_positionsBuffer(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
    , m_normalsBuffer(new QOpenGLBuffer(QOpenGLBuffer::VertexBuffer))
    , m_indicesBuffer(new QOpenGLBuffer(QOpenGLBuffer::IndexBuffer))
    , m_shaderProgram()
    , m_vao(new QOpenGLVertexArrayObject)
    , m_indicesCount(0)
    , m_coordinateMirroring(DoNotMirrorCoordinates)
    , m_azimuth(0.0)
    , m_elevation(15.0)
    , m_distance(15.0)
{

}

QMeshRenderer::~QMeshRenderer()
{
    invalidate();
}

void QMeshRenderer::initialize(CoordinateMirroring cm)
{
    if (m_vao->isCreated())
        return; // already initialized

    m_coordinateMirroring = cm;

    QObjLoader loader;

    if (!loader.load("/home/lacie/Github/kms_eye/share/shader/trefoil.obj"))
        CONSOLE << "Could not load mesh";

    if (!m_vao->create())
        CONSOLE << "Unable to create VAO";

    m_vao->bind();

    const QVector<QVector3D> vertices = loader.vertices();
    if (!m_positionsBuffer->create())
        CONSOLE << "Unable to create position buffer";
    m_positionsBuffer->bind();
    m_positionsBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_positionsBuffer->allocate(vertices.constData(), vertices.size() * sizeof(QVector3D));

    const QVector<QVector3D> normals = loader.normals();
    m_normalsBuffer->create();
    m_normalsBuffer->bind();
    m_normalsBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_normalsBuffer->allocate(normals.constData(), normals.size() * sizeof(QVector3D));

    const QVector<unsigned int> indices = loader.indices();
    m_indicesCount = indices.size();
    if (!m_indicesBuffer->create())
        CONSOLE << "Unable to create index buffer";

    m_indicesBuffer->bind();
    m_indicesBuffer->setUsagePattern(QOpenGLBuffer::StaticDraw);
    m_indicesBuffer->allocate(indices.constData(), indices.size() * sizeof(unsigned int));

    m_shaderProgram.reset(new QOpenGLShaderProgram);
    if (!m_shaderProgram->create())
        CONSOLE << "Unable to create shader program";
    if (!m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "/home/lacie/Github/kms_eye/share/shader/phong.vert"))
        CONSOLE << "Vertex shader compilation failed";
    if (!m_shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "/home/lacie/Github/kms_eye/share/shader/phong.frag"))
        CONSOLE << "Fragment shader compilation failed";
    if (!m_shaderProgram->link())
        CONSOLE << "Shader program not linked";

    m_shaderProgram->bind();

    m_positionsBuffer->bind();
    m_shaderProgram->enableAttributeArray("vertexPosition");
    m_shaderProgram->setAttributeBuffer("vertexPosition", GL_FLOAT, 0, 3);

    m_normalsBuffer->bind();
    m_shaderProgram->enableAttributeArray("vertexNormal");
    m_shaderProgram->setAttributeBuffer("vertexNormal", GL_FLOAT, 0, 3);

    m_vao->release();
}

void QMeshRenderer::render()
{
    QOpenGLFunctions *functions = QOpenGLContext::currentContext()->functions();

    QMatrix4x4 modelMatrix;
    QMatrix4x4 viewMatrix;
    QMatrix4x4 projectionMatrix;

    modelMatrix.rotate(-90, 0, 1, 0);

    const float azimuthInRadians = qDegreesToRadians(m_azimuth);
    const float elevationInRadians = qDegreesToRadians(m_elevation);

    const QVector3D eyePosition(std::cos(elevationInRadians) * std::cos(azimuthInRadians),
                                std::sin(elevationInRadians),
                                -std::cos(elevationInRadians) * std::sin(azimuthInRadians));

    QVector3D upVector = qFuzzyCompare(m_elevation, 90.0f)
            ? QVector3D(-std::cos(azimuthInRadians), 0, std::sin(azimuthInRadians))
            : QVector3D(0, 1, 0);

    viewMatrix.lookAt(eyePosition * m_distance,
                      QVector3D(0, 0, 0),
                      upVector);

    GLint viewportSize[4];
    functions->glGetIntegerv(GL_VIEWPORT, viewportSize);

    projectionMatrix.perspective(30, float(viewportSize[2]) / viewportSize[3], 0.01, 1000);

    switch (m_coordinateMirroring) {
    case QMeshRenderer::DoNotMirrorCoordinates:
        break;
    case QMeshRenderer::MirrorYCoordinate:
        projectionMatrix.scale(1, -1, 1);
        break;
    }

    const QMatrix4x4 modelViewMatrix = viewMatrix * modelMatrix;
    const QMatrix4x4 modelViewProjectionMatrix = projectionMatrix * modelViewMatrix;

    functions->glClearColor(1.0, 1.0, 1.0, 1.0);
    functions->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    functions->glEnable(GL_DEPTH_TEST);

    m_shaderProgram->bind();

    m_shaderProgram->setUniformValue("modelViewMatrix", modelViewMatrix);
    m_shaderProgram->setUniformValue("normalMatrix", modelViewMatrix.normalMatrix());
    m_shaderProgram->setUniformValue("projectionMatrix", projectionMatrix);
    m_shaderProgram->setUniformValue("mvp", modelViewProjectionMatrix);

    m_shaderProgram->setUniformValue("light.position", QVector4D(0.0, 0.0, 0.0, 1.0));
    m_shaderProgram->setUniformValue("light.intensity", QVector3D(1.0, 1.0, 1.0));

    m_shaderProgram->setUniformValue("material.ka", QVector3D(0.1, 0.1, 0.1));
    m_shaderProgram->setUniformValue("material.kd", QVector3D(1.0, 0.1, 0.1));
    m_shaderProgram->setUniformValue("material.ks", QVector3D(1.0, 1.0, 1.0));
    m_shaderProgram->setUniformValue("material.shininess", 32.0f);

    m_vao->bind();
    functions->glDrawElements(GL_TRIANGLES, m_indicesCount, GL_UNSIGNED_INT, Q_NULLPTR);
    m_vao->release();
}

void QMeshRenderer::invalidate()
{
    m_positionsBuffer->destroy();
    m_normalsBuffer->destroy();
    m_indicesBuffer->destroy();
    m_shaderProgram.reset();
    m_vao->destroy();
}

void QMeshRenderer::setAzimuth(float azimuth)
{
    m_azimuth = azimuth;
}

void QMeshRenderer::setElevation(float elevation)
{
    m_elevation = elevation;
}

void QMeshRenderer::setDistance(float distance)
{
    m_distance = distance;
}
