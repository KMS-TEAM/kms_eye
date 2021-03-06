#include "AppEngine.h"
#include "AppConstant.h"
#include "Screen_Def.h"
#include "AppEnums.h"
#include "QConfig.h"
#include "AppModel.h"
#include "QMLHandler.h"
#include "Q3DPointCloudRender/QPointCloud.h"
#include "Q3DPointCloudRender/QPointCloudGeometry.h"
#include "Q3DPointCloudRender/QPointField.h"
#include "Q3DPointCloudRender/QPointCloudReader.h"
#include "QImageItem/QImageItem.h"
#include "QImageItem/QOpenCVImageProvider.h"
#include "QMeshRenderScreen.h"

#include <QVariant>

ScreenDef* ScreenDef::m_instance = nullptr;
QMutex ScreenDef::m_lock;
AppConstant* AppConstant::m_instance = nullptr;
QMutex AppConstant::m_lock;

AppEngine::AppEngine()
{
    m_rootContext = this->rootContext();
}

AppEngine::~AppEngine(){

}

void AppEngine::initEngine(){

    // register class
    qmlRegisterType<QConfig>("kms.team.qconfig",1,0,"QConfig");
    qmlRegisterUncreatableType<AppEnums>("QmlCustomItem", 1, 0, "ENUMS", "Uncreatable");
    qmlRegisterUncreatableType<AppEnums>("AppEnums", 1, 0, "Enums", "Cannot create object from enums!");

    // QPCL class
    qmlRegisterType<QPointCloudReader>("pcl", 1, 0, "PointcloudReader");
    qmlRegisterType<QPointcloud>("pcl", 1, 0, "Pointcloud");
    qmlRegisterType<QPointcloudGeometry>("pcl", 1, 0, "PointcloudGeometry");
    qmlRegisterUncreatableType<QPointfield>("pcl", 1, 0, "Pointfield", "Can not yet be created in qml, use PointcloudReader.");

    // QPixmap type
    qmlRegisterType<QImageItem>("MyImage", 1, 0, "QImageItem");
//    qmlRegisterType<QCameraControl>("CameraControls", 1, 0, "QCameraControl");

    QOpenCVImageProvider *liveImageProvider(new QOpenCVImageProvider);
    QOpenCVImageProvider *liveLeftImageProvider(new QOpenCVImageProvider);
    QOpenCVImageProvider *liveRightImageProvider(new QOpenCVImageProvider);

    // connect signal slots
    connect(QML_HANDLER, &QMLHandler::notifyQMLEvent, this, &AppEngine::slotReceiveEvent);

    // set context properties
    m_rootContext->setContextProperty("QmlConst", DEFS);
    m_rootContext->setContextProperty("QmlHandler", QML_HANDLER);
    m_rootContext->setContextProperty("QmlScreen", SCR_DEF);
    m_rootContext->setContextProperty("QmlModel", MODEL);

    m_rootContext->setContextProperty("liveImageProvider", liveImageProvider);
    m_rootContext->setContextProperty("liveLeftImageProvider", liveLeftImageProvider);
    m_rootContext->setContextProperty("liveRightImageProvider", liveRightImageProvider);

    this->addImageProvider("live", liveImageProvider);
    this->addImageProvider("liveLeft", liveLeftImageProvider);
    this->addImageProvider("liveRight", liveRightImageProvider);

    connect(MODEL, &AppModel::disparityImageChanged, liveImageProvider, &QOpenCVImageProvider::updateImage);
    connect(MODEL, &AppModel::currentLeftFrameChanged, liveLeftImageProvider, &QOpenCVImageProvider::updateImage);
    connect(MODEL, &AppModel::currentRightFrameChanged, liveRightImageProvider, &QOpenCVImageProvider::updateImage);

}

void AppEngine::startEngine(){
    this->load(SCR_DEF->QML_APP());
}

void AppEngine::meshRenderScreenRun(AppEngine *engine)
{
    QMeshRenderScreen *m_meshRenderScreen = new QMeshRenderScreen(engine);
    m_meshRenderScreen->resize(600, 600);
    m_meshRenderScreen->show();
}

void AppEngine::slotReceiveEvent(int event)
{
    CONSOLE << "Received event " << event;
    switch (event) {
    case static_cast<int>(AppEnums::EVT_NONE):
        CONSOLE << "Invalid event";
        // do sth here, maybe call a function to process images
        // then use MODEL->setCurrentPath to re-set path
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_IMAGE_SCREEN):
        CONSOLE << "Image Viewer";
        MODEL->setCurrentScreenID(AppEnums::VIEW_SCREEN::IMAGE_VIEWER_SCREEN);
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_PCL_SCREEN):
        CONSOLE << "PCL Viewer";
        MODEL->setCurrentScreenID(AppEnums::VIEW_SCREEN::PCL_VIEWER_SCREEN);
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_CAMERA_SCREEN):
        CONSOLE << "Camera Viewer";
        MODEL->setCurrentScreenID(AppEnums::VIEW_SCREEN::CAMERA_VIEWER_SCREEN);
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_SETTING_PATH):
        CONSOLE << MODEL->settingPath();
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_RECONSTRUCTION_SETTING_PATH):
        CONSOLE << MODEL->settingPath();
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_RUN):
        MODEL->setState(AppEnums::APP_STATE::STATE_RUNNING);
        MODEL->imageProcessing(AppEnums::ALGORITHM::SGBM);
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_RUN_RECONSTRUCTION):
        MODEL->runReconstruction();
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_STOP):
        MODEL->setState(AppEnums::APP_STATE::STATE_STOP);
        MODEL->imageProcessing(AppEnums::ALGORITHM::SGBM);
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_NEXT_IMAGE):
        MODEL->m_currentImageNumber = MODEL->m_currentImageNumber + 1;
        MODEL->setCurrentImageNumber(MODEL->m_currentImageNumber);
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_PREVIOUS_IMAGE):
        MODEL->m_currentImageNumber = MODEL->m_currentImageNumber - 1;
        if(MODEL->m_currentImageNumber <= 0) MODEL->m_currentImageNumber = 0;
        MODEL->setCurrentImageNumber(MODEL->m_currentImageNumber);
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_RESET):
        MODEL->m_currentImageNumber = 0;
        MODEL->setCurrentImageNumber(MODEL->m_currentImageNumber);
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_CAMERA_RUN):
        MODEL->cameraRun(DEFS->VIDEO_EXAMPLE_1(), DEFS->VIDEO_EXAMPLE_2());
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_OPENGL_RENDER):
        this->meshRenderScreenRun(this);
        break;
    default:
        break;
    }
    // bla bla bla
}
