#include "QPointCloudRenderScreen.h"
#include "AppConstant.h"

#include <QSurfaceFormat>
#include <QOpenGLContext>
#include <QOpenGLFunctions>

#include <QQuickWindow>
#include <QQuickRenderControl>
#include <QQuickItem>

#include <QQmlEngine>
#include <QQmlComponent>
#include <QQmlContext>

#include <QTimer>

QPointCloudRenderScreen::QPointCloudRenderScreen(AppEngine *engine, QWindow *parent)
    : QWindow(parent)
    , m_context(nullptr)
    , m_renderer(nullptr)
    , m_camera(nullptr)
    , m_renderControl(nullptr)
    , m_quickWindow(nullptr)
    , m_qmlComponent(nullptr)
    , m_rootItem(nullptr)
{
    // set the window up
    setSurfaceType(QSurface::OpenGLSurface);

    QSurfaceFormat format;
    format.setMajorVersion(3);
    format.setMinorVersion(3);
    format.setProfile(QSurfaceFormat::CoreProfile);
    format.setDepthBufferSize(24);
    format.setStencilBufferSize(8);
    format.setSamples(4);

    setFormat(format);
    create();

    // create the GL context
    m_context = new QOpenGLContext(this);
    m_context->setFormat(format);
    if (!m_context->create())
        qFatal("Unable to create context");

    m_context->makeCurrent(this);

    // set up our stuff

    m_renderer = new QPointCloudRenderer(this);
    m_renderer->initialize(DEFS->POINT_CLOUD_EXAMPLE());

    m_camera = new QPointCloudCamera(this);

    // set up QtQuick

    m_renderControl = new QQuickRenderControl(this);
    m_quickWindow = new QQuickWindow(m_renderControl);
    m_quickWindow->setClearBeforeRendering(false);

    // try to "batch" multiple scene changed signals in one sync
    QTimer *sceneSyncTimer = new QTimer(this);
    sceneSyncTimer->setInterval(5);
    sceneSyncTimer->setSingleShot(true);
    connect(sceneSyncTimer, &QTimer::timeout,
            this, &QPointCloudRenderScreen::syncScene);

    connect(m_renderControl, &QQuickRenderControl::sceneChanged,
            sceneSyncTimer, static_cast<void (QTimer::*)()>(&QTimer::start));

    connect(m_renderControl, &QQuickRenderControl::renderRequested,
            this, &QPointCloudRenderScreen::draw);

    m_renderControl->initialize(m_context);

    if (!engine->incubationController())
        engine->setIncubationController(m_quickWindow->incubationController());

    engine->m_rootContext->setContextProperty("_camera", m_camera);
    m_qmlComponent = new QQmlComponent(engine, this);

    connect(m_qmlComponent, &QQmlComponent::statusChanged,
            this, &QPointCloudRenderScreen::onQmlComponentLoadingComplete);

    m_qmlComponent->loadUrl(DEFS->QML_OPENGL_RENDER_URL());

    // also, just for the sake of it, trigger a redraw every 500 ms no matter what
    QTimer *redrawTimer = new QTimer(this);
    connect(redrawTimer, &QTimer::timeout, this, &QPointCloudRenderScreen::draw);
    redrawTimer->start(500);
}

QPointCloudRenderScreen::~QPointCloudRenderScreen()
{
    m_context->makeCurrent(this);

    m_renderer->invalidate();
    delete m_renderer;

    delete m_rootItem;
    delete m_qmlComponent;
    delete m_renderControl;
    delete m_quickWindow;

    m_context->doneCurrent();
    delete m_context;
}

void QPointCloudRenderScreen::resizeEvent(QResizeEvent *e)
{
    // Simulate the "resize root item to follow window"
    updateRootItemSize();
    QWindow::resizeEvent(e);
}

void QPointCloudRenderScreen::syncScene()
{
    m_renderControl->polishItems();

    m_renderer->setCameraState(m_camera->state());

    m_renderControl->sync();

    draw();
}

void QPointCloudRenderScreen::draw()
{
    if (!isExposed())
        return;
    m_context->makeCurrent(this);
    m_context->functions()->glViewport(0, 0, width() * devicePixelRatio(), height() * devicePixelRatio());

    m_renderer->render();
    m_quickWindow->resetOpenGLState();

    m_renderControl->render();

    m_context->swapBuffers(this);
}

void QPointCloudRenderScreen::onQmlComponentLoadingComplete()
{
    if (m_qmlComponent->isLoading())
        return;
    if (m_qmlComponent->isError()) {
        const QList<QQmlError> errorList = m_qmlComponent->errors();
        foreach (const QQmlError &error, errorList)
            qWarning() << error.url() << error.line() << error;

        qFatal("Unable to load QML file");
    }

    QObject *rootObject = m_qmlComponent->create();
    m_rootItem = qobject_cast<QQuickItem *>(rootObject);
    if (!m_rootItem)
        qFatal("Did not load a Qt Quick scene");

    m_rootItem->setParentItem(m_quickWindow->contentItem());
}

void QPointCloudRenderScreen::updateRootItemSize()
{
    if (m_rootItem) {
        m_rootItem->setWidth(width());
        m_rootItem->setHeight(height());
    }

    m_quickWindow->setHeight(height());
    m_quickWindow->setWidth(width());
}

void QPointCloudRenderScreen::mousePressEvent(QMouseEvent *e)
{
    qApp->sendEvent(m_quickWindow, e);
    if (!e->isAccepted())
        QWindow::mousePressEvent(e);
}

void QPointCloudRenderScreen::mouseMoveEvent(QMouseEvent *e)
{
    qApp->sendEvent(m_quickWindow, e);
    if (!e->isAccepted())
        QWindow::mousePressEvent(e);
}

void QPointCloudRenderScreen::mouseReleaseEvent(QMouseEvent *e)
{
    qApp->sendEvent(m_quickWindow, e);
    if (!e->isAccepted())
        QWindow::mousePressEvent(e);
}


















