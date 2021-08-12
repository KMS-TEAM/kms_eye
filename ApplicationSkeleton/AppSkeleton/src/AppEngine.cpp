#include "AppEngine.h"
#include <QQmlEngine>
#include "AppConstant.h"
#include "AppEnums.h"
#include "AppModel.h"
#include "QMLHandler.h"

AppConstant* AppConstant::self = nullptr;
AppEngine::AppEngine()
{
    m_rootContext = this->rootContext();
}

void AppEngine::initEngine()
{
    // connect signal slots
    connect(QML_HANDLER, &QMLHandler::notifyQMLEvent, this, &AppEngine::slotReceiveEvent);

    // set qml properties and Enums
    m_rootContext->setContextProperty("QmlHandler", QML_HANDLER);
    m_rootContext->setContextProperty("QmlModel", MODEL);
    m_rootContext->setContextProperty("QmlConst", DEF);
    qmlRegisterUncreatableType<AppEnums>("AppEnums", 1, 0, "Enums", "Cannot create object from enums!");
}

void AppEngine::startApplication()
{
    this->load(DEF->QML_MAIN_URL());
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
    case static_cast<int>(AppEnums::EVT_CLICK_BUTTON_A):
        MODEL->setCurrentPath("AAA/AAA/AAA");
        // do sth here, maybe call a function to process images
        // then use MODEL->setCurrentPath to re-set path
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_BUTTON_B):
        MODEL->setCurrentPath("BBB/BBB/BBB");
        // do sth here, maybe call a function to process images
        // then use MODEL->setCurrentPath to re-set path
        break;
    case static_cast<int>(AppEnums::EVT_CLICK_BUTTON_C):
        MODEL->setCurrentPath("CCC/CCC/CCC");
        // do sth here, maybe call a function to process images
        // then use MODEL->setCurrentPath to re-set path
        break;
    default:
        break;
    }
    // bla bla bla
}
