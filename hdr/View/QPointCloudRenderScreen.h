#ifndef QPOINTCLOUDRENDERSCREEN_H
#define QPOINTCLOUDRENDERSCREEN_H

#include <QWindow>
#include <QQmlApplicationEngine>

#include "QOpenGLRender/QPointCloudCamera.h"
#include "QOpenGLRender/QPointCloudRenderer.h"
#include "AppEngine.h"

class QOpenGLContext;
class QQuickWindow;
class QQuickRenderControl;
class QQmlComponent;
class QQuickItem;

class QPointCloudRenderScreen : public QWindow
{
public:
    QPointCloudRenderScreen(AppEngine *engine = nullptr, QWindow *parent = nullptr);
    ~QPointCloudRenderScreen();

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

    QPointCloudRenderer *m_renderer;
    QPointCloudCamera *m_camera;

    QQuickRenderControl *m_renderControl;
    QQuickWindow *m_quickWindow;
    QQmlComponent *m_qmlComponent;
    QQuickItem *m_rootItem;
};

#endif // QPOINTCLOUDRENDERSCREEN_H
