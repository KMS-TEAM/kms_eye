#include "AppEngine.h"
#include "Constants_Def.h"
#include "Screen_Def.h"
#include "AppEnums.h"
#include "QConfig.h"
#include "DirHelper.h"
#include "AppModel.h"

ScreenDef* ScreenDef::m_instance = nullptr;
QMutex ScreenDef::m_lock;
Constants_Def* Constants_Def::m_instance = nullptr;
QMutex Constants_Def::m_lock;

AppEngine::AppEngine(QObject *parent) : QObject{ parent }
{
    m_context = m_engine.rootContext();
}

AppEngine::~AppEngine(){

}

void AppEngine::prepareApplication(){

    // register class
    qmlRegisterType<DirHelper>("kms.team.dirhelper",1,0,"DirHelper");
    qmlRegisterType<QConfig>("kms.team.qconfig",1,0,"QConfig");
    qmlRegisterUncreatableType<AppEnums>("QmlCustomItem", 1, 0, "ENUMS", "Uncreatable");

    // set context properties
    m_context->setContextProperty("CONST", DEFS);
    m_context->setContextProperty("SCREEN", SCR_DEF);
    m_context->setContextProperty("AppModel", MODEL);
}

void AppEngine::runApplication(){
    m_engine.load(SCR_DEF->QML_APP());
}
