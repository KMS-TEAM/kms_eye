#include "AppEngine.h"
#include "AppConstant.h"
#include "Screen_Def.h"
#include "AppEnums.h"
#include "QConfig.h"
#include "AppModel.h"
#include "QMLHandler.h"

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

    // connect signal slots
    connect(QML_HANDLER, &QMLHandler::notifyQMLEvent, this, &AppEngine::slotReceiveEvent);

    // set context properties
    m_rootContext->setContextProperty("QmlConst", DEFS);
    m_rootContext->setContextProperty("QmlHandler", QML_HANDLER);
    m_rootContext->setContextProperty("QmlScreen", SCR_DEF);
    m_rootContext->setContextProperty("QmlModel", MODEL);
}

void AppEngine::startEngine(){
    this->load(SCR_DEF->QML_APP());
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
    case static_cast<int>(AppEnums::EVT_CLICK_SETTING_PATH):
        MODEL->setState(AppEnums::APP_STATE::STATE_RUNNING);
        CONSOLE << MODEL->settingPath();
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_DISPARITY_MAP):
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
    default:
        break;
    }
    // bla bla bla
}
