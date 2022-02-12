#ifndef QRENDERSCREEN_H
#define QRENDERSCREEN_H

#include <QWindow>
#include <QQmlApplicationEngine>

#include "QOpenGLRender/QMeshRenderer.h"
#include "QOpenGLRender/QCameraControl.h"
#include "AppEngine.h"

class QMeshRenderer;
class QCameraControl;
class QOpenGLContext;
class QQuickWindow;
class QQuickRenderControl;
class QQmlComponent;
class QQuickItem;

class QRenderScreen : public QWindow
{
public:
    QRenderScreen(AppEngine *engine = nullptr, QWindow *parent = nullptr);
    ~QRenderScreen();

protected:
    void resizeEvent(QResizeEvent *e) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *e) Q_DECL_OVERRIDE;
    void mouseReleaseEvent(QMouseEvent *e) Q_DECL_OVERRIDE;

private:
    void syncScene();
    void draw();

    void onQmlComponentLoadingComplete();
    void updateRootItemSize();

    QOpenGLContext *m_context;

    QMeshRenderer *m_renderer;
    QCameraControl *m_camera;

    QQuickRenderControl *m_renderControl;
    QQuickWindow *m_quickWindow;
    QQmlComponent *m_qmlComponent;
    QQuickItem *m_rootItem;
};

#endif // QRENDERSCREEN_H
