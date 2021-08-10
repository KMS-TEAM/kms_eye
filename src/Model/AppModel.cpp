#include "AppModel.h"
#include "Screen_Def.h"
#include <QDebug>

AppModel* AppModel::m_instance = nullptr;
QMutex AppModel::m_lock;

AppModel::AppModel(QObject *parent) :
    QObject(parent)
{

}

AppModel *AppModel::getInstance(){
    m_lock.lock();
    if (nullptr == m_instance){
        m_instance = new AppModel();
    }
    m_lock.unlock();
    return m_instance;
}

void AppModel::qmlEventHandler(int event){
    switch (event) {
    case static_cast<int>(AppEnums::UserSettingPath):
    case static_cast<int>(AppEnums::UserClickDisparityMap):
    case static_cast<int>(AppEnums::UserClickNextImage):
    case static_cast<int>(AppEnums::UserClickPreviousImage):
    case static_cast<int>(AppEnums::UserClickReset):
    default:
        break;
    }
}
